//
// Created by Bryan on 5/22/2026.
//

#pragma once
#include <SFML/Graphics.hpp>

class AnimacionExplosion {
public:
    AnimacionExplosion();
    void iniciar(float pixelX, float pixelY, float ancho, float alto);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana) const;
    bool estaActiva() const;

private:
    static const int totalFrames = 10;
    static const char* nombresFrames[totalFrames];
    float x, y;
    float ancho, alto;
    int frameActual;
    float tiempoAcumulado;
    float duracionPorFrame;
    bool activa;
    sf::Sprite sprite;
};