#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <utility>
#include <string>

class Mapa {
public:
    Mapa();

    // Cargar grafo desde fichero
    bool cargar(const std::string& fichero);

    int numNodos() const;
    int numAristas() const;

    // Vecinos de u como (v, w)
    const std::vector< std::pair<int,double> >& vecinos(int u) const;

    // Coste de un camino
    double costeCamino(const std::vector<int>& camino) const;

private:
    void reiniciar(int n);
    void Arista(int u, int v, double w);

    int n; // número de nodos (1..n)
    int m; // número de aristas no dirigidas
    std::vector< std::vector< std::pair<int,double> > > adj;
};

#endif // MAPA_H
