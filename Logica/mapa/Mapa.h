#pragma once
#include "Casilla.h"
#include "../estructuras/Matriz.h"
#include "../estructuras/ArregloDinamico.h"
#include <random>

class Mapa {
public:
    Mapa(int filas, int columnas, float tamañoCasilla, bool tematicaDesierto);

    void generar();
    bool esAccesible() const;

    Casilla&       getCasilla(int fila, int columna);
    const Casilla& getCasilla(int fila, int columna) const;

    int   getFilas()          const;
    int   getColumnas()       const;
    float getTamañoCasilla() const;
    bool  esDesierto()        const;

    bool hayConexion(int indiceA, int indiceB) const;

    int  aIndice(int fila, int columna) const;
    void aCoordenadas(int indice, int& fila, int& columna) const;

    ArregloDinamico<Casilla*> getVecinos(int fila, int columna);

private:
    int   filas;
    int   columnas;
    float tamañoCasilla;
    bool  desierto;

    Matriz<Casilla> casillas;
    Matriz<int>     matrizAdyacencia;

    void inicializarCasillas();
    void colocarObstaculos();
    void construirMatrizAdyacencia();
    void garantizarAccesibilidad();
    bool posicionEstrategicaParaBarril(int f, int c) const;

    bool todosConectados()                        const;
    void bfsConectividad(int inicio, bool* vis)   const;

    std::mt19937 generador;
};