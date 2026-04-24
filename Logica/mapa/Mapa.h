#pragma once
#include "Casilla.h"
#include <random>
#include "estructuras/ArregloDinamico.h"

class Mapa {
public:
    Mapa(int filas, int columnas, float tamañoCasilla);

    void generar();
    bool esAccesible() const;

    Casilla&       getCasilla(int fila, int columna);
    const Casilla& getCasilla(int fila, int columna) const;

    int   getFilas() const;
    int   getColumnas() const;
    float getTamañoCasilla() const;

    bool hayConexion(int indiceA, int indiceB) const;
    
    int  aIndice(int fila, int columna) const;
    void aCoordenadas(int indice, int& fila, int& columna) const;

    ArregloDinamico<Casilla*> getVecinos(int fila, int columna);

private:
    int   filas;
    int   columnas;
    float tamañoCasilla;
    ArregloDinamico<Casilla> casillas;          // tamaño: filas * columnas
    ArregloDinamico<int>     matrizAdyacencia;  // tamaño: (filas*columnas)^2
    void inicializarCasillas();
    void colocarObstaculos();
    void construirMatrizAdyacencia();
    void garantizarAccesibilidad();
    bool todosConectados() const;
    void bfsConectividad(int inicio, bool* visitado) const;

    std::mt19937 generador;
};