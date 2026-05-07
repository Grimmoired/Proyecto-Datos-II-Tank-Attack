#include "Mapa.h"
#include <stdexcept>

Mapa::Mapa(int filas, int columnas, float tamañoCasilla, bool tematicaDesierto)
    : filas(filas), columnas(columnas), tamañoCasilla(tamañoCasilla),
      desierto(tematicaDesierto),
      casillas(filas, columnas),
      generador(std::random_device{}())
{
    if (filas < 5 || columnas < 5)
        throw std::invalid_argument("El mapa debe tener al menos 5x5 casillas");
    inicializarCasillas();
}

void Mapa::generar() {
    colocarObstaculos();
    garantizarAccesibilidad();
}

void Mapa::inicializarCasillas() {
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++) {
            casillas[f][c] = Casilla(f, c, TipoCasilla::Suelo);
            casillas[f][c].setPixel(c * tamañoCasilla, f * tamañoCasilla);
        }
}

static bool esZonaSpawn(int f, int c, int filas, int columnas) {
    return (f < 2 && c < 2)                     ||
           (f < 2 && c >= columnas - 2)          ||
           (f >= filas - 2 && c < 2)             ||
           (f >= filas - 2 && c >= columnas - 2);
}

bool Mapa::posicionEstrategicaParaBarril(int f, int c) const {
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    int vecinosObstaculo   = 0;
    int vecinosTransitable = 0;
    for (int d = 0; d < 4; d++) {
        int nf = f + df[d];
        int nc = c + dc[d];
        if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue;
        if (casillas[nf][nc].getTipo() == TipoCasilla::Suelo)
            vecinosTransitable++;
        else
            vecinosObstaculo++;
    }
    return vecinosObstaculo >= 1 && vecinosTransitable >= 1;
}

void Mapa::colocarObstaculos() {
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            casillas[f][c].setTipo(TipoCasilla::Suelo);
    std::uniform_real_distribution<float> prob(0.0f, 1.0f);
    const float probObstaculo        = 0.12f;
    const float probDestruibleDebil  = 0.07f;
    const float probDestruibleFuerte = 0.06f;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (esZonaSpawn(f, c, filas, columnas)) continue;
            float r = prob(generador);
            if (r < probObstaculo)
                casillas[f][c].setTipo(TipoCasilla::Obstaculo);
            else if (r < probObstaculo + probDestruibleDebil)
                casillas[f][c].setTipo(TipoCasilla::DestruibleDebil);
            else if (r < probObstaculo + probDestruibleDebil + probDestruibleFuerte)
                casillas[f][c].setTipo(TipoCasilla::DestruibleFuerte);
        }
    }
    
    const float probBarril = 0.03f;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (esZonaSpawn(f, c, filas, columnas)) continue;
            if (casillas[f][c].getTipo() != TipoCasilla::Suelo) continue;
            if (prob(generador) < probBarril &&
                posicionEstrategicaParaBarril(f, c))
                casillas[f][c].setTipo(TipoCasilla::Barril);
        }
    }
}

void Mapa::garantizarAccesibilidad() {
    std::uniform_int_distribution<int> randFila(0, filas - 1);
    std::uniform_int_distribution<int> randCol(0, columnas - 1);
    int intentos = 0;
    while (!todosConectados() && intentos < 2000) {
        int f = randFila(generador);
        int c = randCol(generador);
        if (casillas[f][c].getTipo() == TipoCasilla::Obstaculo)
            casillas[f][c].setTipo(TipoCasilla::Suelo);
        intentos++;
    }
}

void Mapa::bfsConectividad(int fIniciales, int cIniciales, bool** visitado) const {
    int* colaF = new int[filas * columnas];
    int* colaC = new int[filas * columnas];
    int cabeza = 0;
    int fin    = 0;
    colaF[fin]   = fIniciales;
    colaC[fin]   = cIniciales;
    fin++;
    visitado[fIniciales][cIniciales] = true;
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (cabeza < fin) {
        int f = colaF[cabeza];
        int c = colaC[cabeza];
        cabeza++;
        for (int d = 0; d < 4; d++) {
            int nf = f + df[d];
            int nc = c + dc[d];
            if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue;
            if (visitado[nf][nc]) continue;
            if (casillas[nf][nc].getTipo() == TipoCasilla::Obstaculo) continue;
            visitado[nf][nc] = true;
            colaF[fin] = nf;
            colaC[fin] = nc;
            fin++;
        }
    }
    delete[] colaF;
    delete[] colaC;
}

bool Mapa::todosConectados() const {
    int fIniciales = -1;
    int cIniciales = -1;
    int totalLibres = 0;
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            if (casillas[f][c].getTipo() != TipoCasilla::Obstaculo) {
                if (fIniciales == -1) {fIniciales = f; cIniciales = c; }
                totalLibres++;
            }
    if (totalLibres == 0) return false;
    bool** visitado = new bool*[filas];
    for (int i = 0; i < filas; i++) {
        visitado[i] = new bool[columnas];
        for (int j = 0; j < columnas; j++)
            visitado[i][j] = false;
    }
    bfsConectividad(fIniciales, cIniciales, visitado);
    int visitados = 0;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (casillas[f][c].getTipo() != TipoCasilla::Obstaculo && visitado[f][c])
                visitados++;
        }
    }
    for (int i = 0; i < filas; i++)
        delete[] visitado[i];
    delete[] visitado;
    return visitados == totalLibres;
}

bool Mapa::sonAdyacentes(int f1, int c1, int f2, int c2) const {
    int df = f1 - f2;
    int dc = c1 - c2;
    if (df < 0) df = -df;
    if (dc < 0) dc = -dc;
    return (df == 1 && dc == 0) || (df == 0 && dc == 1);
}

ArregloDinamico<Casilla*> Mapa::getVecinos(int fila, int columna) {
    ArregloDinamico<Casilla*> vecinos(4);
    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int nf = fila + df[d];
        int nc = columna + dc[d];
        if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue;
        if (casillas[nf][nc].esTransitable())
            vecinos.agregar(&casillas[nf][nc]);
    }
    return vecinos;
}

bool  Mapa::esAccesible()    const { return todosConectados(); }
bool  Mapa::esDesierto()     const { return desierto; }
int   Mapa::getFilas()       const { return filas; }
int   Mapa::getColumnas()    const { return columnas; }
float Mapa::getTamañoCasilla() const { return tamañoCasilla; }
Casilla&       Mapa::getCasilla(int f, int c)       {return casillas[f][c]; }
const Casilla& Mapa::getCasilla(int f, int c) const {return casillas[f][c]; }