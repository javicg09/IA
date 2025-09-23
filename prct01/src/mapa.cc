#include "../lib/mapa.h"
#include <fstream>
#include <limits>

Mapa::Mapa() : n(0), m(0) {}

void Mapa::reiniciar(int n_) {
    n = n_;
    m = 0;
    adj.assign(n + 1, std::vector< std::pair<int,double> >());
}

void Mapa::anadirAristaNoDirigida(int u, int v, double w) {
    if (u < 1 || v < 1 || u > n || v > n) return;
    adj[u].push_back(std::make_pair(v, w));
    adj[v].push_back(std::make_pair(u, w));
    m++;
}

bool Mapa::cargar(const std::string& fichero) {
    std::ifstream in(fichero.c_str());
    if (!in) return false;

    int N;
    if (!(in >> N)) return false;
    reiniciar(N);

    // Leemos d(i,j) para i<j
    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            double d;
            if (!(in >> d)) return false;
            if (d >= 0.0) {
                anadirAristaNoDirigida(i, j, d);
            }
        }
    }
    return true;
}

int Mapa::numNodos() const { return n; }
int Mapa::numAristas() const { return m; }

const std::vector< std::pair<int,double> >& Mapa::vecinos(int u) const {
    return adj[u];
}

double Mapa::costeCamino(const std::vector<int>& camino) const {
    double c = 0.0;
    for (size_t k = 1; k < camino.size(); ++k) {
        int u = camino[k - 1], v = camino[k];
        bool ok = false;
        const std::vector< std::pair<int,double> >& vec = adj[u];
        for (size_t t = 0; t < vec.size(); ++t) {
            if (vec[t].first == v) {
                c += vec[t].second;
                ok = true;
                break;
            }
        }
        if (!ok) return std::numeric_limits<double>::infinity();
    }
    return c;
}
