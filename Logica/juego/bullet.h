#ifndef UNTITLED_BULLET_H
#define UNTITLED_BULLET_H
#include<SFML/Graphics.hpp>
#include "GameManager.h"
#include <cmath>
#include "../mapa/Mapa.h"
#include "../mapa/Casilla.h"
#include "../estructuras/ArregloDinamico.h"
#include "../estructuras/pair.h"
#include "../juego/Tanque.h"
#include "../assets/AssetManager.h"


using path = ArregloDinamico<pair<int,int>>;

class bullet {
public:
    bullet(float x[] , float r, sf::Color c, int maxhits, Tanque* T);
    void aim(const sf::RenderWindow& window);
    void update_coords(float delta);
    void collition_check(Mapa& M);
    void draw( sf::RenderWindow& window);
    void setRutaGuiada(const path& ruta, float tamCasilla, int danio);
    void setDanio(int danio);
    void setSpriteColor(ColorTanque color);
    struct par {
        float first = 0.0, second = 0.0;
    };

    sf::CircleShape shape;
    float radius;
    float pos[2];
    float v[2];
    bool isalive = true;
    Tanque* tirador;
    sf::Sprite spriteShape;
    bool usarSprite;
    bool reboteSonido;
    bool destruyeObstaculoSonido;
    bool golpeTanqueSonido;
    bool destruyeTanqueSonido;
    bool tieneRuta;
    path rutaGuiada;
    int pasoRuta;
    bool destruyoBarril;
    int  barrilDestruidoFila;
    int  barrilDestruidoColumna;

private:
    sf::CircleShape create_circle();
    par dir[8];
    sf::Color color;
    int wall_hits;
    int max_hits;
    sf::Clock cooldownClock;
    bool canDMG = false;
    float tamCasilla;
    int danio;
};

#endif //UNTITLED_BULLET_H