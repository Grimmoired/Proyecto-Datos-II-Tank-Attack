#include "bullet.h"

bullet::bullet(float x[], float r, sf::Color c, int maxhits, Tanque* T) {
    radius = r;
    color = c;
    pos[0] = x[0];
    pos[1] = x[1];
    v[0] = 0.0; v[1] = 0.0;
    tirador = T;
    shape = bullet::create_circle( );
    wall_hits = 0;
    max_hits = maxhits;
    par dir_copy[8] = {
        {radius + 0.2f, 0}, {-radius - 0.2f, 0}, {0, radius +0.2f},
        {0, -radius -0.2f},
        {radius*0.9f, radius*0.9f}, {radius*0.9f, -radius*0.9f},
        {-radius*0.9f, radius*0.9f}, {-radius*0.9f, -radius*0.9f}
    };
    for (int i = 0; i < 8; i++) {
        dir[i] = dir_copy[i];
    }
}

sf::CircleShape bullet::create_circle() {
    sf::CircleShape ans(radius);
    ans.setFillColor(color);
    ans.setPosition({pos[0],pos[1]});

    return ans;
}

void bullet::aim(const sf::RenderWindow& window) {
    sf::Vector2i p = sf::Mouse::getPosition(window);
    auto x = (float) p.x;
    auto y = (float) p.y;

    x = x - pos[0];
    y = y - pos[1];
    const float norm = std::sqrt(x*x + y*y);

    x = x/norm;
    y = y/norm;

    v[0] = x;
    v[1] = y;
}

void bullet::update_coords(const float delta) {
    pos[0] += v[0] * delta;
    pos[1] += v[1] * delta;

}
void bullet::draw( sf::RenderWindow & window) {
    window.draw(shape);
}
void bullet::collition_check(Mapa& M) {
    bool flip_x = false;
    bool flip_y = false;
    float size = M.getTamanioCasilla();
    float lim_derecha = size * (float) M.getColumnas();
    float lim_inferior = size * (float) M.getFilas();
    bool i_bounced = false;
    for (par& d : dir) {
        float p1 = pos[0] + d.first, p2 = pos[1] + d.second;

        if (p1 <= 0 || p1 >= lim_derecha) flip_x = true;
        if (p2 <= 0 || p2 >= lim_inferior) flip_y = true;



        if (p1 >= 0 && p1 < lim_derecha && p2 >= 0 && p2 < lim_inferior) {
            int idx = (int)(p1 / size), jdx = (int)(p2 / size);
            Casilla& C = M.getCasilla(jdx, idx);
            if (C.getTipo() != TipoCasilla::Suelo || (C.getTanque() != tirador && C.getTanque() != nullptr)) {
                float cx = idx * size + size / 2.0f;
                float cy = jdx * size + size / 2.0f;
                float dx = std::abs(pos[0] - cx);
                float dy = std::abs(pos[1] - cy);

                if (dx >= dy) {
                    flip_x = true;
                    i_bounced = true;
                }
                else {
                    flip_y = true;
                    i_bounced = true;
                }
            }
            float elapsed = cooldownClock.getElapsedTime().asSeconds();
            if (elapsed >= 0.6f) {
                canDMG = true;
            } else {
                canDMG = false;
            }

            if (canDMG) {
                if (C.estaOcupada() && C.getTanque() != tirador) {
                    Tanque* T = C.getTanque();
                    T -> doDMG(5);
                    canDMG = false;
                    cooldownClock.restart();
                } else if (C.getTipo() != TipoCasilla::Suelo && C.getTipo() != TipoCasilla::Obstaculo) {
                    C.recibirDanio(5);
                    canDMG = false;
                    cooldownClock.restart();
                }
                if (i_bounced) break;

            }

        }

    }

    if (flip_x) {
        v[0] = -v[0];
    }

    if (flip_y) {
        v[1] = -v[1];
    }

    if (flip_x || flip_y) {
        wall_hits++;
        tirador = nullptr;
    }

    if (wall_hits >= max_hits) {
        isalive = false;
    }



}
