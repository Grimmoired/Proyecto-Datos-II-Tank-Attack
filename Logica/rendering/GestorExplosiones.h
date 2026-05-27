//
// Created by Bryan on 5/22/2026.
//

#pragma once
#include "AnimacionExplosion.h"

class GestorExplosiones {
public:
    GestorExplosiones();
    void agregarExplosion(float pixelX, float pixelY, float ancho, float alto);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana) const;
private:
    static const int maxExplosiones = 16;
    AnimacionExplosion explosiones[maxExplosiones];
};