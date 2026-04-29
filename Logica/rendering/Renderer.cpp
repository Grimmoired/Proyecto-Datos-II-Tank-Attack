#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& ventana) : ventana(ventana) {
    ventana.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
}

void Renderer::limpiar() {
    ventana.clear(sf::Color(20, 20, 20));
}

void Renderer::mostrar() {
    ventana.display();
}

void Renderer::dibujarMapa(const Mapa& mapa) {
    AssetManager& assets   = AssetManager::getInstance();
    const sf::Texture& atlas = assets.getAtlasTexture();
    float tam = mapa.getTamañoCasilla();

    for (int f = 0; f < mapa.getFilas(); f++) {
        for (int c = 0; c < mapa.getColumnas(); c++) {
            const Casilla& casilla = mapa.getCasilla(f, c);
            int semilla   = (f * 1000 + c);
            TipoCasilla tipo       = casilla.getTipo();

            sf::IntRect rectSuelo  = elegirTileSuelo(f, c, semilla, mapa.esDesierto());
            dibujarCasilla(casilla, atlas, rectSuelo, nullptr, tam);
            if (tipo == TipoCasilla::Obstaculo) {
                sf::IntRect r = assets.getSprite("crateMetal");
                dibujarCasilla(casilla, atlas, rectSuelo, &r, tam);
            } else if (tipo == TipoCasilla::DestruibleDebil) {
                sf::IntRect r = assets.getSprite("crateWood");
                dibujarCasilla(casilla, atlas, rectSuelo, &r, tam);
            } else if (tipo == TipoCasilla::DestruibleFuerte) {
                sf::IntRect r = elegirTileDestruibleFuerte(f, c, semilla);
                dibujarCasilla(casilla, atlas, rectSuelo, &r, tam);
            } else if (tipo == TipoCasilla::Barril) {
                sf::IntRect r = assets.getSprite("barrelRed_top");
                dibujarCasilla(casilla, atlas, rectSuelo, &r, tam);
            }
        }
    }
}

void Renderer::dibujarCasilla(const Casilla& casilla, const sf::Texture& atlas, const sf::IntRect& rectSuelo, const sf::IntRect* rectEncima, float tamañoCasilla) {
    float px = casilla.getPixelX();
    float py = casilla.getPixelY();
    
    sf::Sprite spSuelo(atlas, rectSuelo);
    spSuelo.setScale(tamañoCasilla / rectSuelo.width,
                     tamañoCasilla / rectSuelo.height);
    spSuelo.setPosition(px, py);
    ventana.draw(spSuelo);
    
    if (rectEncima != nullptr) {
        sf::Sprite spEncima(atlas, *rectEncima);
        spEncima.setScale(tamañoCasilla / rectEncima->width,
                          tamañoCasilla / rectEncima->height);
        spEncima.setPosition(px, py);
        ventana.draw(spEncima);
    }
}

sf::IntRect Renderer::elegirTileSuelo(int fila, int columna,
                                       unsigned int semilla, bool desierto) {
    AssetManager& assets = AssetManager::getInstance();
    int variante = (fila * 7 + columna * 13 + semilla) % 2;
    if (desierto)
        return variante == 0 ? assets.getSprite("tileSand1") : assets.getSprite("tileSand2");
    else
        return variante == 0 ? assets.getSprite("tileGrass1") : assets.getSprite("tileGrass2");
}

sf::IntRect Renderer::elegirTileDestruibleFuerte(int fila, int columna, unsigned int semilla) {
    AssetManager& assets = AssetManager::getInstance();
    int variante = (fila * 3 + columna * 7 + semilla) % 2;
    return variante == 0 ? assets.getSprite("treeBrown_large") : assets.getSprite("treeGreen_large");
}