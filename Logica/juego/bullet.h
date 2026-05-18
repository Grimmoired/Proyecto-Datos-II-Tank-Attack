#ifndef UNTITLED_BULLET_H
#define UNTITLED_BULLET_H
#include<SFML/Graphics.hpp>
#include "../mapa/Mapa.h"
#include "../juego/Tanque.h"

class bullet {
public:
    bullet(float x[] , float r, sf::Color c, int maxhits, Tanque* T);
    sf::CircleShape create_circle();
    void aim(const sf::RenderWindow& window);
    void update_coords(float delta);
    void collition_check(Mapa& M);
    void draw( sf::RenderWindow& window);

    struct par {
        float first = 0.0, second = 0.0;
    };
    sf::CircleShape shape;
    float radius;
    float pos[2];
    float v[2];
    par dir[8];
    sf::Color color;
    int wall_hits;
    int max_hits;
    bool isalive = true;
    sf::Clock cooldownClock;
    bool canDMG = false;
    Tanque* tirador;
};
#endif //UNTITLED_BULLET_H