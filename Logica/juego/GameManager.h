#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <utility>
#include "Tanque.h"
#include "GestorTurnos.h"
#include "SpawnPowerUp.h"
#include "EstadoJuego.h"
#include "../pathfinding/pathfinder.h"
#include "../mapa/Mapa.h"
#include "../rendering/Renderer.h"
#include "../ui/HUD.h"
#include "../estructuras/ArregloDinamico.h"
#include "../estructuras/pair.h"
class bullet;


using path = ArregloDinamico<pair<int,int>>;
class GameManager {
public:
    GameManager(sf::RenderWindow& ventana);
    ~GameManager();
    bool ejecutar();

private:
    static const int anchoVentana    = 1920;
    static const int altoVentana     = 1080;
    static const int altoHUD         = 200;
    static const int tamanioCasilla  = 55;
    static const int columnasMapa    = anchoVentana / tamanioCasilla;
    static const int filasMapa       = (altoVentana - altoHUD) / tamanioCasilla;
    static const int cantTanques     = 8;
    static const int turnosPorRonda  = 8;
    pathfinder* finder;
    bool precisionAtaqueActiva;

    pathfinder* pf;
    path RutaActual;
    int pasoRuta;
    bool moviendoPorRuta;
    float acumuladorRuta;

    std::mt19937 rng;

    sf::RenderWindow& ventana;
    Mapa* mapa;
    bool esDesierto;
    Tanque tanques[8];
    GestorTurnos gestor;
    int turnoEnRonda;
    bullet* bala;
    bool balaActiva;
    SpawnPowerUp spawnPU;
    Renderer* renderer;
    HUD* hud;
    DatosJugador datosJ1;
    DatosJugador datosJ2;
    float dtActual;

    const char* etiquetasJ1[4];
    const char* etiquetasJ2[4];
    sf::Color barrasJ1[4];
    sf::Color textosJ1[4];
    sf::Color barrasJ2[4];
    sf::Color textosJ2[4];
    float tiempoRestante;
    sf::Clock reloj;
    sf::Clock relojTotal;
    bool          salirAlMenu;
    void inicializarMapa();
    void inicializarTanques();
    void inicializarHUD();
    void inicializarColoresHUD();

    bool spawnValido(int f, int c) const;
    std::pair<int,int> buscarSpawn(int fBase, int cBase, int dirF, int dirC) const;
    void procesarEventos();
    void procesarClickHUD(int mx, int my);
    void procesarClickMapa(int mx, int my);
    void procesarTeclado(sf::Keyboard::Key tecla);
    void avanzarTurno();
    void actualizarDatosHUD();
    void renderizar(float tiempoTotal);
    static const char* nombreSpritePorColor(ColorTanque color);
    void limpiarTanquesMuertos();
    void avanzarPasoRuta();
    ArregloDinamico<pair<int,int>> casillasMovimiento;
    bool mostrandoCasillasMovimiento;
};