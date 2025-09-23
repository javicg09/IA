#ifndef NODO_H
#define NODO_H

#include <vector>

class Nodo {
public:
    Nodo();
    explicit Nodo(int origen);

    // Camino actual (ids 1..N)
    const std::vector<int>& getCamino() const;
    double getCoste() const;

    // Añade un paso al camino con coste de arista w
    void anadirPaso(int v, double w);

    // ¿Ya contiene el vértice v en esta rama? (evita ciclos)
    bool contiene(int v) const;

private:
    std::vector<int> camino;
    double coste;
};

#endif // NODO_H
