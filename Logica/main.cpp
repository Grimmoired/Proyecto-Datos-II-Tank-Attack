#include <SFML/Graphics.hpp>
#include "assets/AssetManager.h"
#include "mapa/Mapa.h"
#include "rendering/Renderer.h"
#include "ui/PantallaMenu.h"
#include "ui/PantallaInstrucciones.h"
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
    const int columnas = 1920 / tamañoCasilla;
    const int filas    = 1080 / tamañoCasilla;

    bool ejecutando = true;
    while (ejecutando && ventana.isOpen()) {
        PantallaMenu menu(ventana);
        OpcionMenu opcion = menu.ejecutar();

        if (!ventana.isOpen()) break;

        if (opcion == OpcionMenu::Salir) {
            ejecutando = false;
        } else if (opcion == OpcionMenu::Instrucciones) {
            PantallaInstrucciones instrucciones(ventana);
            instrucciones.ejecutar();
        } else if (opcion == OpcionMenu::Jugar) {
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
            ejecutando = false;
        }
    }

    return 0;
}