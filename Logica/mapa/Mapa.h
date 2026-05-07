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
    Casilla& getCasilla(int fila, int columna);
    const Casilla& getCasilla(int fila, int columna) const;
    int getFilas() const;
    int getColumnas() const;
    float getTamañoCasilla() const;
    bool esDesierto() const;
    ArregloDinamico<Casilla*> getVecinos(int fila, int columna);
    bool sonAdyacentes(int f1, int c1, int f2, int c2) const;

private:
    int filas;
    int columnas;
    float tamañoCasilla;
    bool desierto;
    Matriz<Casilla> casillas;
    void inicializarCasillas();
    void colocarObstaculos();
    void garantizarAccesibilidad();
    bool posicionEstrategicaParaBarril(int f, int c) const;
    bool todosConectados() const;
    void bfsConectividad(int f, int c, bool** visitado) const;
    std::mt19937 generador;
};