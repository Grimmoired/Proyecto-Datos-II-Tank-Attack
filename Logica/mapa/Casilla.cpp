#include "Casilla.h"

Casilla::Casilla()
    : fila(0), columna(0), tipo(TipoCasilla::Suelo), ocupada(false) {}

Casilla::Casilla(int fila, int columna, TipoCasilla tipo)
    : fila(fila), columna(columna), tipo(tipo), ocupada(false) {}

int Casilla::getFila() const { return fila; }
int Casilla::getColumna() const { return columna; }
TipoCasilla Casilla::getTipo() const { return tipo; }
bool Casilla::esObstaculo() const { return tipo == TipoCasilla::Obstaculo; }
bool Casilla::estaOcupada() const { return ocupada; }

void Casilla::setTipo(TipoCasilla t) { tipo = t; }
void Casilla::setOcupada(bool o) { ocupada = o; }
void Casilla::setPosicionPixel(float x, float y) { posicionPixel = {x, y}; }
sf::Vector2f Casilla::getPosicionPixel() const { return posicionPixel; }