#pragma once
#include <SFML/Graphics.hpp>
#include "../mapa/Mapa.h"
#include "../assets/AssetManager.h"

class Renderer {
public:
    Renderer(sf::RenderWindow& ventana);

    void dibujarMapa(const Mapa& mapa);
    void limpiar();
    void mostrar();

private:
    sf::RenderWindow& ventana;

    void dibujarCasilla(const Casilla& casilla, const sf::Texture& atlas,
                        const sf::IntRect& rect, float tamañoCasilla);
    sf::IntRect elegirTileSuelo(int fila, int columna, unsigned int semilla);
    sf::IntRect elegirTileObstaculo(int fila, int columna, unsigned int semilla);
};