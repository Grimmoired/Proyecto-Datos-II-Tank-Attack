#include "Mapa.h"
#include <stdexcept>

Mapa::Mapa(int filas, int columnas, float tamañoCasilla)
    : filas(filas),
      columnas(columnas),
      tamañoCasilla(tamañoCasilla),
      casillas(filas * columnas),
      matrizAdyacencia(filas * columnas * filas * columnas),
      generador(std::random_device{}())
{
    if (filas < 5 || columnas < 5)
        throw std::invalid_argument("El mapa debe tener al menos 5x5 casillas");

    inicializarCasillas();
}

void Mapa::generar() {
    colocarObstaculos();
    garantizarAccesibilidad();
    construirMatrizAdyacencia();
}

void Mapa::inicializarCasillas() {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int idx = aIndice(f, c);
            casillas[idx] = Casilla(f, c, TipoCasilla::Suelo);

            float px = c * tamañoCasilla;
            float py = f * tamañoCasilla;
            casillas[idx].setPosicionPixel(px, py);
        }
    }
}

void Mapa::colocarObstaculos() {
    // Reset a suelo
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            casillas[aIndice(f,c)].setTipo(TipoCasilla::Suelo);
    std::uniform_real_distribution<float> prob(0.0f, 1.0f);
    const float probabilidadObstaculo = 0.20f;
    auto esEsquina = [&](int f, int c) {
        return (f < 2 && c < 2) ||
               (f < 2 && c >= columnas - 2) ||
               (f >= filas - 2 && c < 2) ||
               (f >= filas - 2 && c >= columnas - 2);
    };
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!esEsquina(f, c) && prob(generador) < probabilidadObstaculo)
                casillas[aIndice(f,c)].setTipo(TipoCasilla::Obstaculo);
        }
    }
}

void Mapa::garantizarAccesibilidad() {
    int intentos = 0;
    std::uniform_int_distribution<int> randFila(0, filas - 1);
    std::uniform_int_distribution<int> randCol(0, columnas - 1);
    while (!todosConectados() && intentos < 1000) {
        int f = randFila(generador);
        int c = randCol(generador);
        if (casillas[aIndice(f,c)].esObstaculo())
            casillas[aIndice(f,c)].setTipo(TipoCasilla::Suelo);
        intentos++;
    }
}

bool Mapa::todosConectados() const {
    int inicio = -1;
    int totalLibres = 0;
    int n = filas * columnas;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int idx = aIndice(f,c);
            if (!casillas[idx].esObstaculo()) {
                if (inicio == -1) inicio = idx;
                totalLibres++;
            }
        }
    }
    if (totalLibres == 0) return false;
    bool* visitado = new bool[n];
    for (int i = 0; i < n; i++) visitado[i] = false;
    bfsConectividad(inicio, visitado);
    int visitados = 0;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int idx = aIndice(f,c);
            if (!casillas[idx].esObstaculo() && visitado[idx])
                visitados++;
        }
    }
    delete[] visitado;
    return visitados == totalLibres;
}

void Mapa::bfsConectividad(int inicio, bool* visitado) const {
    int n = filas * columnas;
    int* cola = new int[n];
    int cabeza = 0;
    int fin = 0;
    cola[fin++] = inicio;
    visitado[inicio] = true;
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    while (cabeza < fin) {
        int actual = cola[cabeza++];
        int f, c;
        aCoordenadas(actual, f, c);
        for (int d = 0; d < 4; d++) {
            int nf = f + df[d];
            int nc = c + dc[d];
            if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas)
                continue;
            int idx = aIndice(nf, nc);
            if (!visitado[idx] && !casillas[idx].esObstaculo()) {
                visitado[idx] = true;
                cola[fin++] = idx;
            }
        }
    }
    delete[] cola;
}

void Mapa::construirMatrizAdyacencia() {
    int n = filas * columnas;
    for (int i = 0; i < n * n; i++)
        matrizAdyacencia[i] = 0;
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {

            int idxActual = aIndice(f,c);

            if (casillas[idxActual].esObstaculo()) continue;

            for (int d = 0; d < 4; d++) {
                int nf = f + df[d];
                int nc = c + dc[d];

                if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas)
                    continue;

                int idxVecino = aIndice(nf,nc);

                if (casillas[idxVecino].esObstaculo()) continue;

                matrizAdyacencia[idxActual * n + idxVecino] = 1;
            }
        }
    }
}

int Mapa::aIndice(int fila, int columna) const {
    return fila * columnas + columna;
}

void Mapa::aCoordenadas(int indice, int& fila, int& columna) const {
    fila = indice / columnas;
    columna = indice % columnas;
}

bool Mapa::hayConexion(int a, int b) const {
    int n = filas * columnas;
    return matrizAdyacencia[a * n + b] == 1;
}

Casilla& Mapa::getCasilla(int fila, int columna) {
    return casillas[aIndice(fila,columna)];
}

const Casilla& Mapa::getCasilla(int fila, int columna) const {
    return casillas[aIndice(fila,columna)];
}

int   Mapa::getFilas() const { return filas; }
int   Mapa::getColumnas() const { return columnas; }
float Mapa::getTamañoCasilla() const { return tamañoCasilla; }
ArregloDinamico<Casilla*> Mapa::getVecinos(int fila, int columna) {
    ArregloDinamico<Casilla*> vecinos(4);
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int nf = fila + df[d];
        int nc = columna + dc[d];
        if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas)
            continue;
        int idx = aIndice(nf,nc);
        if (!casillas[idx].esObstaculo())
            vecinos.agregar(&casillas[idx]);
    }
    return std::move(vecinos);
}

bool Mapa::esAccesible() const {
    return todosConectados();
}