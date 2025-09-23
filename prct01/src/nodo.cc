#include "../lib/nodo.h"
#include <algorithm>

Nodo::Nodo() : coste(0.0) {}

Nodo::Nodo(int origen) : coste(0.0) {
    camino.push_back(origen);
}

const std::vector<int>& Nodo::getCamino() const {
    return camino;
}

double Nodo::getCoste() const {
    return coste;
}

void Nodo::anadirPaso(int v, double w) {
    camino.push_back(v);
    coste += w;
}

bool Nodo::contiene(int v) const {
    return std::find(camino.begin(), camino.end(), v) != camino.end();
}
