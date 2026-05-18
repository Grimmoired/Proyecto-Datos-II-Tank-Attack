#pragma once
#include <SFML/Graphics.hpp>
#include "../estructuras/Cola.h"
#include "PowerUp.h"

enum class ColorTanque {
    Azul,
    Rojo,
    Verde,
    Amarillo
};

enum class DireccionTanque {
    Arriba,
    Abajo,
    Izquierda,
    Derecha
};

class Tanque {
public:
    static const int maxPowerUps = 3;
    Tanque();
    Tanque(int fila, int columna, ColorTanque color, int jugador, int indice);
    void inicializarSprite(const sf::Texture& atlas, const sf::IntRect& rectCuerpo, float tamanioCasilla);
    void dibujar(sf::RenderWindow& ventana) const;
    void dibujarResaltado(sf::RenderWindow& ventana, float tamanioCasilla, float tiempoTotal) const;
    int getFila() const;
    int getColumna() const;
    ColorTanque getColor() const;
    int getJugador() const;
    int getIndice() const;
    bool estaVivo() const;
    int getCantidadPowerUps()const;
    float getVidaPorcentaje() const;

    void setFila(int f);
    void setColumna(int c);
    void setDireccion(DireccionTanque dir);
    void doDMG(int cantidad);
    bool agregarPowerUp(TipoPowerUp tipo);
    bool consumirPowerUp();
    PowerUp* tomarPowerUpPendiente();
    bool tienePowerUpPendiente() const;
    void getTiposPowerUps(TipoPowerUp* tipos, int maxSlots) const;
    Tanque& operator=(Tanque&& otro) noexcept;  

private:
    int fila;
    int columna;
    ColorTanque color;
    DireccionTanque direccion;
    int jugador;
    int indice;
    bool vivo;
    int vida;
    TipoPowerUp powerUpsEnCola[3];
    int cantidadPUCola;

    Cola<PowerUp>   colaPowerUps;
    bool            powerUpPendiente;
    PowerUp         pendiente;

    sf::Sprite spriteCuerpo;
    float rotacionPorDireccion(DireccionTanque dir) const;
};