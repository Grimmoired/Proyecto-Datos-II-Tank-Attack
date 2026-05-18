#pragma once
#include "PowerUp.h"
#include "../mapa/Mapa.h"
#include "../juego/Tanque.h"

struct PowerUpEnMapa {
    TipoPowerUp tipo;
    int fila;
    int columna;
    bool activo;

    PowerUpEnMapa() : tipo(TipoPowerUp::DobleTurno),
                      fila(0), columna(0), activo(false) {}
};

class SpawnPowerUp {
public:
    static const int maxEnMapa = 3;
    SpawnPowerUp();
    void intentarSpawn(Mapa& mapa, Tanque* tanques, int cantTanques);
    PowerUpEnMapa* recoger(int fila, int columna);
    void dibujar(sf::RenderWindow& ventana, float tamanioCasilla);
    void inicializar();
    int getCantidadActivos() const;

private:
    PowerUpEnMapa   powerUps[maxEnMapa];
    int             cantActivos;
    TipoPowerUp     historial[2];
    int             cantHistorial;
    bool posicionValida(int f, int c, Mapa& mapa, Tanque* tanques, int cantTanques) const;
    TipoPowerUp elegirTipo();
    sf::Sprite spritePorTipo[4];
    bool inicializado;
    static const char* nombreTexturaPorTipo(TipoPowerUp tipo);
};