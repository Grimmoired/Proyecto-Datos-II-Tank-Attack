#include <SFML/Graphics.hpp>
#include "assets/AssetManager.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tank Attack!");
    window.setFramerateLimit(60);

    try {
        AssetManager::getInstance().loadAll();
        std::cout << "Assets cargados correctamente" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color(30, 30, 30));
        window.display();
    }

    return 0;
}