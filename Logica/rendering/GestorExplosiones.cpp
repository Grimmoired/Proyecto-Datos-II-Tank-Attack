//
// Created by Bryan on 5/22/2026.
//

#include "GestorExplosiones.h"

GestorExplosiones::GestorExplosiones() {}

void GestorExplosiones::agregarExplosion(float px, float py, float ancho, float alto) {
    for (int i = 0; i < maxExplosiones; i++) {
        if (!explosiones[i].estaActiva()) {
            explosiones[i].iniciar(px, py, ancho, alto);
            return;
        }
    }
}

void GestorExplosiones::actualizar(float dt) {
    for (int i = 0; i < maxExplosiones; i++)
        explosiones[i].actualizar(dt);
}

void GestorExplosiones::dibujar(sf::RenderWindow& ventana) const {
    for (int i = 0; i < maxExplosiones; i++)
        explosiones[i].dibujar(ventana);
}