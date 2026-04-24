#pragma once
#include <SFML/Graphics.hpp>

enum class TipoCasilla {
    Suelo,
    Obstaculo
};

class Casilla {
public:
    Casilla();
    Casilla(int fila, int columna, TipoCasilla tipo);

    int          getFila() const;
    int          getColumna() const;
    TipoCasilla  getTipo() const;
    bool         esObstaculo() const;
    bool         estaOcupada() const;

    void setTipo(TipoCasilla tipo);
    void setOcupada(bool ocupada);
    void setPosicionPixel(float x, float y);

    sf::Vector2f getPosicionPixel() const;

private:
    int          fila;
    int          columna;
    TipoCasilla  tipo;
    bool         ocupada;
    sf::Vector2f posicionPixel;
};