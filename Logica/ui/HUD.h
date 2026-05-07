#pragma once
#include <SFML/Graphics.hpp>
#include "../juego/EstadoJuego.h"

struct DatosTanque {
    const char* etiqueta;
    sf::Color colorBarra;
    sf::Color colorTexto;
    float vidaPorcentaje;
    int powerUpsEnCola;
    bool vivo;
};

struct DatosJugador {
    const char* nombre;
    sf::Color colorNombre;
    DatosTanque tanques[4];
};

class HUD {
public:
    HUD(sf::RenderWindow& ventana);
    void actualizar(const DatosJugador& j1, const DatosJugador& j2, float tiempoRestante, int turnoActual, EstadoJuego estado);
    void dibujar();
    bool clickEnMover   (int mouseX, int mouseY) const;
    bool clickEnAtacar  (int mouseX, int mouseY) const;
    bool clickEnFinTurno(int mouseX, int mouseY) const;
private:
    sf::RenderWindow& ventana;
    static const int altoHUD = 200;
    static const int anchoVentana = 1920;
    static const int altoVentana = 1080;
    static const int yHUD = altoVentana - altoHUD;
    static const int xZonaJ1 = 0;
    static const int anchoZonaJ1 = 560;
    static const int xZonaCentral = 560;
    static const int anchoZonaCentral = 800;
    static const int xZonaJ2 = 1360;
    static const int anchoZonaJ2 = 560;

    struct FilaTanque {
        sf::Text etiqueta;
        sf::RectangleShape barraFondo;
        sf::RectangleShape barraVida;
        sf::Text porcentaje;
        sf::RectangleShape slotsPU[3];
    };
    FilaTanque filasJ1[4];
    FilaTanque filasJ2[4];
    sf::Text nombreJ1;
    sf::Text nombreJ2;
    sf::RectangleShape fondoHUD;
    sf::RectangleShape divisorIzq;
    sf::RectangleShape divisorDer;
    sf::Text textoTiempoLabel;
    sf::Text textoTiempo;
    sf::Text textoTurno;

    sf::RectangleShape botonMover;
    sf::RectangleShape botonAtacar;
    sf::RectangleShape botonFinTurno;
    sf::Text textoMover;
    sf::Text textoAtacar;
    sf::Text textoFinTurno;
    sf::Text estadoMover;
    sf::Text estadoAtacar;

    sf::Text crearTexto(const char* str, unsigned int tam, sf::Color color, float x, float y);
    sf::RectangleShape crearBoton(float x, float y, float w, float h, sf::Color relleno, sf::Color borde);
    void construirFilas();
    void actualizarFila(FilaTanque& fila, const DatosTanque& datos);
    void actualizarBotones(EstadoJuego estado);
    void dibujarFila(const FilaTanque& fila);
    bool dentroDeRect(const sf::RectangleShape& rect, int mx, int my) const;
};