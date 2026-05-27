#pragma once
#include <SFML/Graphics.hpp>
#include "../juego/Tanque.h"

class PantallaVictoria {
public:
    PantallaVictoria(sf::RenderWindow& ventana);
    void ejecutar(int ganador, Tanque* tanques, int cantTanques);

private:
    sf::RenderWindow& ventana;
    sf::Sprite fondoSprite;
    sf::Texture fondoTextura;
    sf::RectangleShape panel;
    sf::Text textTitulo;
    sf::Text textGanador;
    sf::Text textStats;
    sf::Text textVolver;
    sf::RectangleShape panelVolver;
    void inicializar(int ganador, Tanque* tanques, int cantTanques);
    void dibujar();
    sf::Text crearTexto(const std::string& str, unsigned int tam, sf::Color color, float x, float y);
};