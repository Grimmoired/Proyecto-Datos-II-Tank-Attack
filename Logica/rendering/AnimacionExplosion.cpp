//
// Created by Bryan on 5/22/2026.
//

#include "AnimacionExplosion.h"
#include "../assets/AssetManager.h"

const char* AnimacionExplosion::nombresFrames[10] = {
    "explosion1", "explosion2", "explosionSmoke1",
    "explosion3", "explosionSmoke2", "explosion4",
    "explosionSmoke3", "explosion5", "explosionSmoke4",
    "explosionSmoke5"
};

AnimacionExplosion::AnimacionExplosion() : x(0), y(0), ancho(0), alto(0), frameActual(0), tiempoAcumulado(0), duracionPorFrame(0.07f), activa(false) {}

void AnimacionExplosion::iniciar(float pixelX, float pixelY, float a, float b) {
    x = pixelX;
    y = pixelY;
    ancho = a;
    alto = b;
    frameActual = 0;
    tiempoAcumulado = 0;
    activa = true;

    AssetManager& assets = AssetManager::getInstance();
    sf::IntRect rect = assets.getSprite(nombresFrames[0]);
    sprite.setTexture(assets.getAtlasTexture());
    sprite.setTextureRect(rect);
    sprite.setScale(ancho / rect.width, alto / rect.height);
    sprite.setPosition(x, y);
}

void AnimacionExplosion::actualizar(float dt) {
    if (!activa) return;
    tiempoAcumulado += dt;
    if (tiempoAcumulado >= duracionPorFrame) {
        tiempoAcumulado = 0;
        frameActual++;
        if (frameActual >= totalFrames) {
            activa = false;
            return;
        }
        AssetManager& assets = AssetManager::getInstance();
        sf::IntRect rect = assets.getSprite(nombresFrames[frameActual]);
        sprite.setTextureRect(rect);
        sprite.setScale(ancho / rect.width, alto / rect.height);
        sprite.setPosition(x, y);
    }
}

void AnimacionExplosion::dibujar(sf::RenderWindow& ventana) const {
    if (!activa) return;
    ventana.draw(sprite);
}

bool AnimacionExplosion::estaActiva() const { return activa; }