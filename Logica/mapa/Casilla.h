#pragma once

enum class TipoCasilla {
    Obstaculo,       // 0 — indestructible (caja metalica)
    Suelo,           // 1 — transitable
    DestruibleDebil, // 2 — 1 punto de vida (caja de madera)
    DestruibleFuerte,// 3 — 2 puntos de vida (arbol)
    Barril           // 4 — Rompe todas las casillas adyacentes (Barril rojo)
};

class Casilla {
public:
    Casilla();
    Casilla(int fila, int columna, TipoCasilla tipo);

    int         getFila()    const;
    int         getColumna() const;
    TipoCasilla getTipo()    const;
    bool        esObstaculo()    const;
    bool        esTransitable()  const;
    bool        estaOcupada()    const;
    float       getPixelX()  const;
    float       getPixelY()  const;

    void setTipo(TipoCasilla tipo);
    void setOcupada(bool ocupada);
    void setPixel(float x, float y);

private:
    int         fila;
    int         columna;
    TipoCasilla tipo;
    bool        ocupada;
    float       pixelX;
    float       pixelY;
};