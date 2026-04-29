#include "Mapa.h"
#include <stdexcept>

Mapa::Mapa(int filas, int columnas, float tamañoCasilla, bool tematicaDesierto)
    : filas(filas), columnas(columnas), tamañoCasilla(tamañoCasilla),
      desierto(tematicaDesierto),
      casillas(filas, columnas),
      matrizAdyacencia(filas * columnas, filas * columnas, 0),
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

bool Mapa::todosConectados() const {
    int inicio      = -1;
    int totalLibres = 0;
    int n           = filas * columnas;

    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            if (casillas[f][c].getTipo() != TipoCasilla::Obstaculo) {
                if (inicio == -1) inicio = aIndice(f, c);
                totalLibres++;
            }

    if (totalLibres == 0) return false;

    bool* visitado = new bool[n];
    for (int i = 0; i < n; i++) visitado[i] = false;

    bfsConectividad(inicio, visitado);

    int visitados = 0;
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            if (casillas[f][c].getTipo() != TipoCasilla::Obstaculo &&
                visitado[aIndice(f, c)])
                visitados++;

    delete[] visitado;
    return visitados == totalLibres;
}

void Mapa::bfsConectividad(int inicio, bool* visitado) const {
    int n     = filas * columnas;
    int* cola = new int[n];
    int cabeza = 0;
    int fin    = 0;

    cola[fin++]      = inicio;
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
            if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue;
            int idx = aIndice(nf, nc);
            if (!visitado[idx] &&
                casillas[nf][nc].getTipo() != TipoCasilla::Obstaculo) {
                visitado[idx] = true;
                cola[fin++]   = idx;
            }
        }
    }
    delete[] cola;
}

void Mapa::construirMatrizAdyacencia() {
    matrizAdyacencia.rellenar(0);

    const int df[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (casillas[f][c].getTipo() == TipoCasilla::Obstaculo) continue;
            int idxActual = aIndice(f, c);
            for (int d = 0; d < 4; d++) {
                int nf = f + df[d];
                int nc = c + dc[d];
                if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue;
                if (casillas[nf][nc].getTipo() == TipoCasilla::Obstaculo) continue;
                matrizAdyacencia[idxActual][aIndice(nf, nc)] = 1;
            }
        }
    }
}

int   Mapa::aIndice(int f, int c)                  const { return f * columnas + c; }
void  Mapa::aCoordenadas(int idx, int& f, int& c)  const { f = idx / columnas; c = idx % columnas; }
bool  Mapa::hayConexion(int a, int b)              const { return matrizAdyacencia[a][b] == 1; }
bool  Mapa::esAccesible()                          const { return todosConectados(); }
bool  Mapa::esDesierto()                           const { return desierto; }
int   Mapa::getFilas()                             const { return filas; }
int   Mapa::getColumnas()                          const { return columnas; }
float Mapa::getTamañoCasilla()                    const { return tamañoCasilla; }

Casilla&       Mapa::getCasilla(int f, int c)       { return casillas[f][c]; }
const Casilla& Mapa::getCasilla(int f, int c) const { return casillas[f][c]; }

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