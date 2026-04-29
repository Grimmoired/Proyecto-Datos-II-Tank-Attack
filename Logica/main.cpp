#include <SFML/Graphics.hpp>
#include <random>
#include "assets/AssetManager.h"
#include "mapa/Mapa.h"
#include "rendering/Renderer.h"
#include "ui/PantallaMenu.h"
#include "ui/PantallaInstrucciones.h"
#include <iostream>

int main() {
    const int anchoVentana  = 1920;
    const int altoVentana   = 1080;
    const int tamanioCasilla = 55;
    const int columnasMapa  = anchoVentana  / tamanioCasilla; // 34
    const int filasMapa     = altoVentana   / tamanioCasilla; // 19

    sf::RenderWindow ventana(sf::VideoMode(anchoVentana, altoVentana),
                              "Tank Attack!");
    ventana.setFramerateLimit(60);

    try {
        AssetManager::getInstance().loadAll();
    } catch (const std::exception& e) {
        std::cerr << "Error cargando assets: " << e.what() << std::endl;
        return -1;
    }

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
            AssetManager::getInstance().getMusic("juego").setLoop(true);
            AssetManager::getInstance().getMusic("juego").play();
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> moneda(0, 1);
            bool esDesierto = moneda(rng) == 1;

            Mapa mapa(filasMapa, columnasMapa,
                      (float)tamanioCasilla, esDesierto);
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