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
    pasoRuta = 0;
    tieneRuta = false;
    tamCasilla = 0.f;
    danio = 25;
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
    if (tieneRuta && pasoRuta < rutaGuiada.size()) {
        pair<int,int> objetivo = rutaGuiada[pasoRuta];
        float ox = objetivo.second * tamCasilla + tamCasilla / 2.f;
        float oy = objetivo.first  * tamCasilla + tamCasilla / 2.f;
        float dx = ox - pos[0];
        float dy = oy - pos[1];
        float dist = std::sqrt(dx*dx + dy*dy);

        if (dist < delta * 300.f) {
            pos[0] = ox;
            pos[1] = oy;
            pasoRuta++;
        } else {
            pos[0] += (dx / dist) * delta * 300.f;
            pos[1] += (dy / dist) * delta * 300.f;
        }

        if (pasoRuta >= rutaGuiada.size())
            isalive = false;
        return;
    }
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
    float lim_der = size * (float)M.getColumnas();
    float lim_inf = size * (float)M.getFilas();
    if (pos[0] <= 0 || pos[0] >= lim_der) flip_x = true;
    if (pos[1] <= 0 || pos[1] >= lim_inf) flip_y = true;
    int fila    = (int)(pos[1] / size);
    int columna = (int)(pos[0] / size);

    if (fila >= 0 && fila < M.getFilas() &&
        columna >= 0 && columna < M.getColumnas()) {
        Casilla& C = M.getCasilla(fila, columna);
        if (C.estaOcupada() && C.getTanque() != nullptr &&
            C.getTanque() != tirador) {
            C.getTanque()->doDMG(danio);
            isalive = false;
            return;
        }
    }

    for (int k = 0; k < 8; k++) {
        float p1 = pos[0] + dir[k].first;
        float p2 = pos[1] + dir[k].second;
        if (p1 < 0 || p1 >= lim_der || p2 < 0 || p2 >= lim_inf) continue;
        int pf = (int)(p2 / size);
        int pc = (int)(p1 / size);
        if (pf < 0 || pf >= M.getFilas() || pc < 0 || pc >= M.getColumnas()) continue;
        Casilla& casillaColision = M.getCasilla(pf, pc);

        if (casillaColision.estaOcupada() && casillaColision.getTanque() != nullptr &&
            casillaColision.getTanque() != tirador) {
            casillaColision.getTanque()->doDMG(danio);
            isalive = false;
            return;
        }

        if (casillaColision.getTipo() != TipoCasilla::Suelo) {
            float cx = pc * size + size / 2.f;
            float cy = pf * size + size / 2.f;
            float ddx = std::abs(pos[0] - cx);
            float ddy = std::abs(pos[1] - cy);
            if (ddx >= ddy) flip_x = true;
            else            flip_y = true;
            
            if (casillaColision.getTipo() != TipoCasilla::Obstaculo)
                casillaColision.recibirDanio(danio);
        }
    }

    if (flip_x) v[0] = -v[0];
    if (flip_y) v[1] = -v[1];

    if (flip_x || flip_y) {
        wall_hits++;
        tirador = nullptr;
    }
    if (wall_hits >= max_hits) isalive = false;
}

void bullet::setDanio(int d) {
    danio = d;
}

void bullet::setRutaGuiada(const path& ruta, float tam, int d) {
    for (int i = 0; i < ruta.size(); i++)
        rutaGuiada.agregar(ruta[i]);
    pasoRuta  = 0;
    tieneRuta = true;
    tamCasilla = tam;
    danio      = d;
}
