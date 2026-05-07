#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <utility>
#include <sstream>
#include "assets/AssetManager.h"
#include "mapa/Mapa.h"
#include "rendering/Renderer.h"
#include "ui/PantallaMenu.h"
#include "ui/PantallaInstrucciones.h"
#include "ui/HUD.h"
#include "juego/EstadoJuego.h"
#include "juego/Tanque.h"
#include "juego/GestorTurnos.h"
#include <iostream>

static const char* nombreCuerpo(ColorTanque color) {
    switch (color) {
        case ColorTanque::Azul:     return "tank_blue";
        case ColorTanque::Rojo:     return "tank_red";
        case ColorTanque::Verde:    return "tank_green";
        case ColorTanque::Amarillo: return "tank_sand";
    }
    return "tank_blue";
}

int main() {
    const int anchoVentana   = 1920;
    const int altoVentana    = 1080;
    const int altoHUD        = 200;
    const int tamañoCasilla = 55;
    const int columnasMapa   = anchoVentana / tamañoCasilla;
    const int filasMapa      = (altoVentana - altoHUD) / tamañoCasilla;
    sf::RenderWindow ventana(sf::VideoMode(anchoVentana, altoVentana), "Tank Attack!");
    ventana.setFramerateLimit(60);

    try {
        AssetManager::getInstance().loadAll();
    } catch (const std::exception& e) {
        std::cerr << "Error cargando assets: " << e.what() << std::endl;
        return -1;
    }

    bool ejecutando = true;
    while (ejecutando && ventana.isOpen()) {
        PantallaMenu menu(ventana);
        OpcionMenu opcion = menu.ejecutar();
        if (!ventana.isOpen()) break;
        if (opcion == OpcionMenu::Salir) {
            ejecutando = false;
        } else if (opcion == OpcionMenu::Instrucciones) {
            PantallaInstrucciones instrucciones(ventana);
            instrucciones.ejecutar();
        } else if (opcion == OpcionMenu::Jugar) {
            
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> moneda(0, 1);
            bool esDesierto = moneda(rng) == 1;
            Mapa mapa(filasMapa, columnasMapa, (float)tamañoCasilla, esDesierto);
            mapa.generar();
            AssetManager& assets     = AssetManager::getInstance();
            const sf::Texture& atlas = assets.getAtlasTexture();
            
            auto spawnValido = [&](int f, int c) -> bool {
                if (f < 0 || f >= filasMapa || c < 0 || c >= columnasMapa) return false;
                if (mapa.getCasilla(f, c).getTipo() != TipoCasilla::Suelo) return false;
                if (mapa.getCasilla(f, c).estaOcupada()) return false;
                const int df[] = {-1, 1, 0, 0};
                const int dc[] = {0, 0, -1, 1};
                for (int d = 0; d < 4; d++) {
                    int nf = f + df[d];
                    int nc = c + dc[d];
                    if (nf < 0 || nf >= filasMapa || nc < 0 || nc >= columnasMapa) continue;
                    if (mapa.getCasilla(nf, nc).getTipo() != TipoCasilla::Suelo) return false;
                    if (mapa.getCasilla(nf, nc).estaOcupada()) return false;
                }
                return true;
            };

            auto buscarSpawn = [&](int fBase, int cBase, int dirF, int dirC) -> std::pair<int,int> {
                for (int dist = 0; dist < 10; dist++) {
                    int f = fBase + dirF * dist;
                    int c = cBase + dirC * dist;
                    if (f < 0) f = 0;
                    if (c < 0) c = 0;
                    if (f >= filasMapa)   f = filasMapa   - 1;
                    if (c >= columnasMapa) c = columnasMapa - 1;
                    if (spawnValido(f, c)) return {f, c};
                    for (int lat = 1; lat <= 4; lat++) {
                        if (spawnValido(f, c + lat)) return {f, c + lat};
                        if (spawnValido(f, c - lat)) return {f, c - lat};
                        if (spawnValido(f + lat, c)) return {f + lat, c};
                        if (spawnValido(f - lat, c)) return {f - lat, c};
                    }
                }
                
                for (int f = fBase; f >= 0 && f < filasMapa; f += dirF) {
                    for (int c = cBase; c >= 0 && c < columnasMapa; c += dirC) {
                        if (spawnValido(f, c)) return {f, c};
                    }
                }
                return {fBase, cBase};
            };
            
            auto [f0, c0] = buscarSpawn(0, 0,  1,  1);
            mapa.getCasilla(f0, c0).setOcupada(true);
            auto [f1, c1] = buscarSpawn(0, c0 + 4,  1,  1);
            mapa.getCasilla(f1, c1).setOcupada(true);
            auto [f2, c2] = buscarSpawn(f0 + 4, 0,  1,  1);
            mapa.getCasilla(f2, c2).setOcupada(true);
            auto [f3, c3] = buscarSpawn(f0 + 4, c0 + 4,  1,  1);
            mapa.getCasilla(f3, c3).setOcupada(true);
            
            auto [f4, c4] = buscarSpawn(filasMapa-1, columnasMapa-1, -1, -1);
            mapa.getCasilla(f4, c4).setOcupada(true);
            auto [f5, c5] = buscarSpawn(filasMapa-1, c4 - 4, -1, -1);
            mapa.getCasilla(f5, c5).setOcupada(true);
            auto [f6, c6] = buscarSpawn(f4 - 4, columnasMapa-1, -1, -1);
            mapa.getCasilla(f6, c6).setOcupada(true);
            auto [f7, c7] = buscarSpawn(f4 - 4, c4 - 4, -1, -1);
            mapa.getCasilla(f7, c7).setOcupada(true);
            
            Tanque tanques[8];
            tanques[0] = Tanque(f0, c0, ColorTanque::Azul,     1, 0);
            tanques[1] = Tanque(f1, c1, ColorTanque::Azul,     1, 1);
            tanques[2] = Tanque(f2, c2, ColorTanque::Rojo,     1, 2);
            tanques[3] = Tanque(f3, c3, ColorTanque::Rojo,     1, 3);
            tanques[4] = Tanque(f4, c4, ColorTanque::Verde,    2, 0);
            tanques[5] = Tanque(f5, c5, ColorTanque::Verde,    2, 1);
            tanques[6] = Tanque(f6, c6, ColorTanque::Amarillo, 2, 2);
            tanques[7] = Tanque(f7, c7, ColorTanque::Amarillo, 2, 3);

            for (int i = 0; i < 8; i++) {
                sf::IntRect rectCuerpo = assets.getSprite(nombreCuerpo(tanques[i].getColor()));
                tanques[i].inicializarSprite(atlas, rectCuerpo, (float)tamañoCasilla);
            }
            GestorTurnos gestor;
            gestor.inicializar(tanques, 8);
            Renderer renderer(ventana);
            HUD hud(ventana);

            DatosJugador j1, j2;
            j1.nombre      = "JUGADOR 1";
            j1.colorNombre = sf::Color(96, 144, 208);
            j2.nombre      = "JUGADOR 2";
            j2.colorNombre = sf::Color(96, 192, 80);
            const char* etiquetasJ1[4] = {"AZL1","AZL2","ROJ1","ROJ2"};
            const char* etiquetasJ2[4] = {"VRD1","VRD2","AMR1","AMR2"};

            sf::Color barrasJ1[4] = {
                sf::Color(48,112,192), sf::Color(48,112,192),
                sf::Color(192,48,48),  sf::Color(192,48,48)
            };
            sf::Color textosJ1[4] = {
                sf::Color(160,192,255), sf::Color(160,192,255),
                sf::Color(255,144,144), sf::Color(255,144,144)
            };
            sf::Color barrasJ2[4] = {
                sf::Color(48,160,48),  sf::Color(48,160,48),
                sf::Color(176,144,32), sf::Color(176,144,32)
            };
            sf::Color textosJ2[4] = {
                sf::Color(144,255,144), sf::Color(144,255,144),
                sf::Color(255,224,128), sf::Color(255,224,128)
            };

            float tiempoRestante = 300.0f;
            sf::Clock reloj;
            sf::Clock relojTotal;

            while (ventana.isOpen()) {
                float dt = reloj.restart().asSeconds();
                float tiempoTotal = relojTotal.getElapsedTime().asSeconds();
                tiempoRestante -= dt;

                sf::Event evento;
                while (ventana.pollEvent(evento)) {
                    if (evento.type == sf::Event::Closed)
                        ventana.close();

                    if (evento.type == sf::Event::MouseButtonPressed) {
                        int mx = evento.mouseButton.x;
                        int my = evento.mouseButton.y;
                        if (hud.clickEnMover(mx, my) && gestor.puedeMover())
                            gestor.setEstado(EstadoJuego::modoMover);
                        if (hud.clickEnAtacar(mx, my) && gestor.puedeAtacar())
                            gestor.setEstado(EstadoJuego::modoAtacar);
                        if (hud.clickEnFinTurno(mx, my))
                            gestor.siguienteTurno();
                    }

                    if (evento.type == sf::Event::KeyPressed) {
                        switch (evento.key.code) {
                            case sf::Keyboard::Up:
                            case sf::Keyboard::Down:
                            case sf::Keyboard::Left:
                            case sf::Keyboard::Right:
                                break;
                            case sf::Keyboard::LShift:
                            case sf::Keyboard::RShift:
                                break;
                            case sf::Keyboard::M:
                                if (gestor.puedeMover())
                                    gestor.setEstado(EstadoJuego::modoMover);
                                break;
                            case sf::Keyboard::A:
                                if (gestor.puedeAtacar())
                                    gestor.setEstado(EstadoJuego::modoAtacar);
                                break;
                            case sf::Keyboard::Return:
                                gestor.siguienteTurno();
                                break;
                            case sf::Keyboard::Escape:
                                ejecutando = false;
                                ventana.close();
                                break;
                            default: break;
                        }
                    }
                }

                for (int i = 0; i < 4; i++) {
                    j1.tanques[i] = {etiquetasJ1[i], barrasJ1[i], textosJ1[i], 1.0f, 0, tanques[i].estaVivo()};
                    j2.tanques[i] = {etiquetasJ2[i], barrasJ2[i], textosJ2[i], 1.0f, 0, tanques[i+4].estaVivo()};
                }
                hud.actualizar(j1, j2, tiempoRestante, gestor.getJugadorActual(), gestor.getEstado());
                Tanque* activo = gestor.getTanqueActual();
                renderer.limpiar();
                renderer.dibujarMapa(mapa);

                for (int i = 0; i < 8; i++) {
                    if (&tanques[i] == activo)
                        tanques[i].dibujarResaltado(ventana, (float)tamañoCasilla, tiempoTotal);
                    else
                        tanques[i].dibujar(ventana);
                }
                hud.dibujar();
                renderer.mostrar();
            }
            ejecutando = false;
        }
    }
    return 0;
}