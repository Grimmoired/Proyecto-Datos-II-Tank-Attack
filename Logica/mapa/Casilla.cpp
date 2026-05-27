#include "Casilla.h"

static int vidaInicial(TipoCasilla tipo) {
    switch (tipo) {
        case TipoCasilla::DestruibleDebil:  return 1;
        case TipoCasilla::DestruibleFuerte: return 4;
        case TipoCasilla::Barril:           return 1;
        default:                            return 0;
    }
}

Casilla::Casilla() : fila(0), columna(0), tipo(TipoCasilla::Suelo), ocupada(false), pixelX(0.f), pixelY(0.f), vida(0), tanqueOcupante(nullptr) {}

Casilla::Casilla(int fila, int columna, TipoCasilla tipo) : fila(fila), columna(columna), tipo(tipo), ocupada(false), pixelX(0.f), pixelY(0.f), vida(vidaInicial(tipo)), tanqueOcupante(nullptr) {}

int Casilla::getFila() const { return fila; }
int Casilla::getColumna()  const { return columna; }
TipoCasilla Casilla::getTipo()  const { return tipo; }
bool Casilla::estaOcupada() const { return ocupada; }
float Casilla::getPixelX() const { return pixelX; }
float Casilla::getPixelY() const { return pixelY; }
int   Casilla::getVida() const { return vida; }
Tanque* Casilla::getTanque() const { return tanqueOcupante; }

bool Casilla::esObstaculo() const {
    return tipo == TipoCasilla::Obstaculo;
}

bool Casilla::esTransitable() const {
    return tipo == TipoCasilla::Suelo && !ocupada;
}

void Casilla::setTipo(TipoCasilla t) {
    tipo = t;
    vida = vidaInicial(t);
}

void Casilla::setOcupada(bool o) {
    ocupada = o;
    if (!o) tanqueOcupante = nullptr;
}

void Casilla::setTanque(Tanque* t) {
    tanqueOcupante = t;
    ocupada = (t != nullptr);
}

void Casilla::setPixel(float x, float y) { pixelX = x; pixelY = y; }
void Casilla::recibirDanio(int cantidad) {
    if (tipo == TipoCasilla::Obstaculo) return;
    vida -= cantidad;
    if (vida <= 0) {
        vida = 0;
        tipo = TipoCasilla::Suelo;
    }
}