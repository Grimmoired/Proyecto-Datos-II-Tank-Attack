#pragma once
#include <SFML/Graphics.hpp>

enum class ColorTanque {
    Azul,
    Rojo,
    Verde,
    Amarillo
};

enum class DireccionTanque {
    Arriba,
    Abajo,
    Izquierda,
    Derecha
};

class Tanque {
public:
    Tanque();
    Tanque(int fila, int columna, ColorTanque color, int jugador, int indice);
    void inicializarSprite(const sf::Texture& atlas, const sf::IntRect& rectCuerpo, float tamañoCasilla);
    void dibujar(sf::RenderWindow& ventana) const;
    void dibujarResaltado(sf::RenderWindow& ventana, float tamañoCasilla, float tiempoTotal) const;
    int getFila()    const;
    int getColumna() const;
    ColorTanque getColor()   const;
    int getJugador() const;
    int getIndice()  const;
    bool estaVivo()   const;
    void setFila(int f);
    void setColumna(int c);
    void setDireccion(DireccionTanque dir);

private:
    int fila;
    int columna;
    ColorTanque color;
    DireccionTanque direccion;
    int jugador;
    int indice;
    bool vivo;
    sf::Sprite spriteCuerpo;
    float rotacionPorDireccion(DireccionTanque dir) const;
};