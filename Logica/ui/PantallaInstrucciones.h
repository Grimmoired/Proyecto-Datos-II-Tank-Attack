#pragma once
#include <SFML/Graphics.hpp>

class PantallaInstrucciones {
public:
    PantallaInstrucciones(sf::RenderWindow& ventana);
    void ejecutar();

private:
    sf::RenderWindow& ventana;
    sf::Sprite fondoSprite;
    sf::Texture fondoTextura;
    sf::RectangleShape panel;
    sf::Text textTitulo;
    sf::Text textSecControles;
    sf::Text textMover;
    sf::Text textDisparar;
    sf::Text textPowerUp;

    sf::RectangleShape cajaA;
    sf::Text labelA;
    sf::RectangleShape cajaS;
    sf::Text labelS;
    sf::RectangleShape cajaLMB;
    sf::RectangleShape cajaRMB;
    sf::Text labelLMB;
    sf::Text labelRMB;
    sf::RectangleShape cajaShift;
    sf::Text labelShift;

    sf::Text textSecReglas;
    sf::Text textReglas;
    sf::Text textSecPowerUps;
    sf::Sprite spritePU1;
    sf::Sprite spritePU2;
    sf::Sprite spritePU3;
    sf::Sprite spritePU4;
    sf::Text descPU1;
    sf::Text descPU2;
    sf::Text descPU3;
    sf::Text descPU4;
    sf::Text textCreditos;
    sf::Text textVolver;
    sf::RectangleShape panelVolver;

    void inicializar();
    void dibujar();
    void procesarEventos(bool& corriendo);
    sf::Text crearTexto(const std::string& str, unsigned int tam, sf::Color color, float x, float y);
    sf::RectangleShape crearCajaTecla(float x, float y, float ancho, float alto);
    sf::Text crearLabelTecla(const std::string& label, sf::RectangleShape& caja);
    sf::Sprite crearSpritePU(const std::string& nombre, float x, float y, float altoDeseado);
};