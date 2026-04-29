#include "Casilla.h"

Casilla::Casilla()
    : fila(0), columna(0), tipo(TipoCasilla::Suelo),
      ocupada(false), pixelX(0.f), pixelY(0.f) {}

Casilla::Casilla(int fila, int columna, TipoCasilla tipo)
    : fila(fila), columna(columna), tipo(tipo),
      ocupada(false), pixelX(0.f), pixelY(0.f) {}

int         Casilla::getFila()       const { return fila; }
int         Casilla::getColumna()    const { return columna; }
TipoCasilla Casilla::getTipo()       const { return tipo; }
bool        Casilla::estaOcupada()   const { return ocupada; }
float       Casilla::getPixelX()     const { return pixelX; }
float       Casilla::getPixelY()     const { return pixelY; }

bool Casilla::esObstaculo() const {
    return tipo == TipoCasilla::Obstaculo;
}

bool Casilla::esTransitable() const {
    return tipo == TipoCasilla::Suelo;
}

void Casilla::setTipo(TipoCasilla t)    { tipo    = t; }
void Casilla::setOcupada(bool o)        { ocupada = o; }
void Casilla::setPixel(float x, float y){ pixelX  = x; pixelY = y; }