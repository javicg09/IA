// control de estados por ramas
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>

#include "lib/mapa.h"
#include "lib/nodo.h"

// --- FUNCIONES ---
void PrintHelp() {
  std::cout
      << "— Práctica 1: Búsquedas no informadas\n\n"
      << "Uso:\n"
      << "  ./p01_busquedas filein.txt fileout.txt opcode\n\n"
      << "Parámetros:\n"
      << "  filein.txt   Fichero de entrada\n"
      << "  fileout.txt  Fichero de salida\n"
      << "  opcode       Parámetro de búsqueda (1 o 2)";
}

// Informe con datos pedidos
static void EscribirInforme(const std::string& fichero,
                            const Mapa& G,
                            int origen, int destino,
                            const std::string& estrategia,
                            const std::vector<int>& generados,
                            const std::vector<int>& inspeccionados,
                            const std::vector<int>& camino,
                            double coste) {
  std::ofstream out(fichero.c_str());
  out.setf(std::ios::fixed);

  out << "Numero de nodos del grafo: " << G.numNodos() << "\n";
  out << "Numero de aristas del grafo: " << G.numAristas() << "\n\n";
  out << "Vertice origen: " << origen << "\n";
  out << "Vertice destino: " << destino << "\n\n";
  out << "Estrategia: " << estrategia << "\n\n";

  out << "Nodos generados (en orden de descubrimiento): ";
  for (size_t i = 0; i < generados.size(); ++i) {
    if (i) out << ", ";
    out << generados[i];
  }
  out << "\n";

  out << "Nodos inspeccionados (orden de expansion): ";
  for (size_t i = 0; i < inspeccionados.size(); ++i) {
    if (i) out << ", ";
    out << inspeccionados[i];
  }
  out << "\n\n";

  if (!camino.empty()) {
    out << "Camino: ";
    for (size_t i = 0; i < camino.size(); ++i) {
      if (i) out << " - ";
      out << camino[i];
    }
    out << "\n";
    out << std::setprecision(2);
    out << "Costo: " << coste << "\n";
  } else {
    out << "Camino: no encontrado\n";
  }
}

// BFS por ramas (cola de caminos)
static bool BFS(const Mapa& G, int origen, int destino,
                std::vector<int>& generados,
                std::vector<int>& inspeccionados,
                std::vector<int>& caminoFinal,
                double& costeFinal) {
  std::queue<Nodo> frontera;   // cola de ramas
  std::set<int> vistos;        // para registrar "nodos generados" 1ª vez

  Nodo ini(origen);
  frontera.push(ini);
  vistos.insert(origen);
  generados.push_back(origen);

  while (!frontera.empty()) {
    Nodo actual = frontera.front();
    frontera.pop();

    int u = actual.getCamino().back();
    inspeccionados.push_back(u);

    if (u == destino) {
      caminoFinal = actual.getCamino();
      costeFinal = G.costeCamino(caminoFinal);
      return true;
    }

    const std::vector< std::pair<int,double> >& vec = G.vecinos(u);
    for (size_t i = 0; i < vec.size(); ++i) {
      int v = vec[i].first;
      double w = vec[i].second;

      if (actual.contiene(v)) continue; // evita ciclos en la rama

      Nodo sig = actual;
      sig.anadirPaso(v, w);
      frontera.push(sig);

      if (vistos.find(v) == vistos.end()) {
        vistos.insert(v);
        generados.push_back(v);
      }
    }
  }
  return false;
}

// DFS por ramas (pila de caminos)
static bool DFS(const Mapa& G, int origen, int destino,
                std::vector<int>& generados,
                std::vector<int>& inspeccionados,
                std::vector<int>& caminoFinal,
                double& costeFinal) {
  std::vector<Nodo> pila;      // pila de ramas (LIFO)
  std::set<int> vistos;

  Nodo ini(origen);
  pila.push_back(ini);
  vistos.insert(origen);
  generados.push_back(origen);

  while (!pila.empty()) {
    Nodo actual = pila.back();
    pila.pop_back();

    int u = actual.getCamino().back();
    inspeccionados.push_back(u);

    if (u == destino) {
      caminoFinal = actual.getCamino();
      costeFinal = G.costeCamino(caminoFinal);
      return true;
    }

    const std::vector< std::pair<int,double> >& vec = G.vecinos(u);
    // Insertamos en orden inverso para que el primero del vector se expanda antes
    for (int i = (int)vec.size() - 1; i >= 0; --i) {
      int v = vec[i].first;
      double w = vec[i].second;

      if (actual.contiene(v)) continue;

      Nodo sig = actual;
      sig.anadirPaso(v, w);
      pila.push_back(sig);

      if (vistos.find(v) == vistos.end()) {
        vistos.insert(v);
        generados.push_back(v);
      }
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    PrintHelp();
    return 0;
  }
  if (argc != 4){
    std::cerr << "Error: Formato incorrecto, usar --help para más información.\n";
    return 1;
  }

  const std::string file_in  = argv[1];
  const std::string file_out = argv[2];

  int opcode = 0;
  {
    std::istringstream iss(argv[3]);
    if (!(iss >> opcode) || (opcode != 1 && opcode != 2)) {
      std::cerr << "Error: opcode debe ser 1 (BFS) o 2 (DFS).\n";
      return 1;
    }
  }

  // Cargar grafo
  Mapa G;
  if (!G.cargar(file_in)) {
    std::cerr << "Error leyendo el fichero de entrada: " << file_in << "\n";
    return 1;
  }

  std::cout << "Grafo cargado. N=" << G.numNodos()
            << ", aristas=" << G.numAristas() << "\n";

  // Pedir origen y destino
  int origen = 1, destino = G.numNodos();
  std::cout << "Vertice origen (1.." << G.numNodos() << "): ";
  std::cin >> origen;
  std::cout << "Vertice destino (1.." << G.numNodos() << "): ";
  std::cin >> destino;

  // Ejecutar búsqueda
  std::vector<int> generados, inspeccionados, camino;
  double coste = 0.0;
  bool exito = false;
  std::string estrategia;

  if (opcode == 1) {
    estrategia = "Busqueda en amplitud (BFS)";
    exito = BFS(G, origen, destino, generados, inspeccionados, camino, coste);
  } else {
    estrategia = "Busqueda en profundidad (DFS)";
    exito = DFS(G, origen, destino, generados, inspeccionados, camino, coste);
  }

  // Feedback por consola
  if (exito) {
    std::cout << "[OK] Camino: ";
    for (size_t i = 0; i < camino.size(); ++i) {
      if (i) std::cout << " - ";
      std::cout << camino[i];
    }
    std::cout << "  (coste=" << std::fixed << std::setprecision(2) << coste << ")\n";
  } else {
    std::cout << "No se encontro camino.\n";
  }

  // Guardar informe
  EscribirInforme(file_out, G, origen, destino, estrategia,
                  generados, inspeccionados, camino, coste);

  std::cout << "Informe escrito en " << file_out << "\n";
  return 0;
}

