#include <SFML/Graphics.hpp>
#include "assets/AssetManager.h"
#include "mapa/Mapa.h"
#include "rendering/Renderer.h"
#include <iostream>

int main() {
    sf::RenderWindow ventana(sf::VideoMode(1920, 1080), "Tank Attack!");
    ventana.setFramerateLimit(60);

    try {
        AssetManager::getInstance().loadAll();
    } catch (const std::exception& e) {
        std::cerr << "Error cargando assets: " << e.what() << std::endl;
        return -1;
    }
    
    const int tamañoCasilla = 60;
    const int columnas = 1920 / tamañoCasilla; // 32 casillas
    const int filas    = 1080 / tamañoCasilla; // 18 casillas

    Mapa mapa(filas, columnas, (float)tamañoCasilla);
    mapa.generar();

    Renderer renderer(ventana);

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento))
            if (evento.type == sf::Event::Closed)
                ventana.close();

        renderer.limpiar();
        renderer.dibujarMapa(mapa);
        renderer.mostrar();
    }

    return 0;
}

