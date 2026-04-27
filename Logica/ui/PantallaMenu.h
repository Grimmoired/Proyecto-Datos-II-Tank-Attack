#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

enum class OpcionMenu {
    Jugar,
    Instrucciones,
    Salir,
    Ninguna
};

class PantallaMenu {
public:
    PantallaMenu(sf::RenderWindow& ventana);
    OpcionMenu ejecutar();
private:
    sf::RenderWindow& ventana;
    sf::Sprite      fondoSprite;
    sf::Texture     fondoTextura;
    sf::Text        titulo;
    sf::Text        opcionJugar;
    sf::Text        opcionInstrucciones;
    sf::Text        opcionSalir;
    sf::Text        instruccion;
    sf::RectangleShape panelTitulo;
    sf::RectangleShape panelBoton1;
    sf::RectangleShape panelBoton2;
    sf::RectangleShape panelBoton3;

    struct SpriteTanque {
        sf::Sprite cuerpo;
        sf::Sprite bala;
    };
    std::vector<SpriteTanque> decoracion;
    int        opcionSeleccionada;
    sf::Clock  relojParpadeo;
    bool       visible;

    void inicializar();
    void inicializarDecoracion();
    void procesarEventos(bool& corriendo, OpcionMenu& resultado);
    void actualizar();
    void dibujar();
    void centrarTexto(sf::Text& texto, float anchoContenedor,
                      float xContenedor, float yContenedor,
                      float altoContenedor);
    sf::Sprite crearSprite(const std::string& nombre, float escala,
                           float x, float y, float rotacion,
                           float origenX, float origenY);
};