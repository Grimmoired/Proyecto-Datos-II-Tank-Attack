#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& ventana)
    : ventana(ventana) {
    ventana.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
}

void Renderer::limpiar() {
    ventana.clear(sf::Color(20, 20, 20));
}

void Renderer::mostrar() {
    ventana.display();
}

void Renderer::dibujarMapa(const Mapa& mapa) {
    AssetManager& assets = AssetManager::getInstance();
    const sf::Texture& atlas = assets.getAtlasTexture();

    for (int f = 0; f < mapa.getFilas(); f++) {
        for (int c = 0; c < mapa.getColumnas(); c++) {
            const Casilla& casilla = mapa.getCasilla(f, c);
            unsigned int semilla = f * 1000 + c;
            sf::IntRect rectSuelo = elegirTileSuelo(f, c, semilla);
            dibujarCasilla(casilla, atlas, rectSuelo, mapa.getTamañoCasilla());
            if (casilla.esObstaculo()) {
                sf::IntRect rectObs = elegirTileObstaculo(f, c, semilla);
                dibujarCasilla(casilla, atlas, rectObs, mapa.getTamañoCasilla());
            }
        }
    }
}

void Renderer::dibujarCasilla(const Casilla& casilla, const sf::Texture& atlas, const sf::IntRect& rect, float tamañoCasilla) {
    sf::Sprite sprite(atlas, rect);
    sf::Vector2f pos = casilla.getPosicionPixel();
    float escalaX = tamañoCasilla / rect.width;
    float escalaY = tamañoCasilla / rect.height;
    sprite.setScale(escalaX, escalaY);
    sprite.setPosition(pos);
    ventana.draw(sprite);
}

sf::IntRect Renderer::elegirTileSuelo(int fila, int columna, unsigned int semilla) {
    AssetManager& assets = AssetManager::getInstance();
    int variante = (fila * 7 + columna * 13 + semilla) % 2;
    if (variante == 0) return assets.getSprite("tileSand1");
    return assets.getSprite("tileSand2");
}

sf::IntRect Renderer::elegirTileObstaculo(int fila, int columna, unsigned int semilla) {
    AssetManager& assets = AssetManager::getInstance();
    int variante = (fila * 3 + columna * 7 + semilla) % 3;
    if (variante == 0) return assets.getSprite("treeBrown_large");
    if (variante == 1) return assets.getSprite("treeGreen_large");
    return assets.getSprite("crateMetal");
}