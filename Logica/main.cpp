#include <SFML/Graphics.hpp>
#include "assets/AssetManager.h"
#include "ui/PantallaMenu.h"
#include "ui/PantallaInstrucciones.h"
#include "juego/GameManager.h"
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
            GameManager juego(ventana);
            bool volverAlMenu = juego.ejecutar();
            if (!volverAlMenu) ejecutando = false;
        }
    }

    return 0;
}