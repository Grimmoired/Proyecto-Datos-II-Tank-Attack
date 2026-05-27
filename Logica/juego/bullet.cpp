#include "bullet.h"

bullet::bullet(float x[], float r, sf::Color c, int maxhits, Tanque* T) {
    radius = r;
    color = c;
    pos[0] = x[0];
    pos[1] = x[1];
    v[0] = 0.f; v[1] = 0.f;
    tirador = T;
    wall_hits = 0;
    max_hits = maxhits;
    pasoRuta = 0;
    tieneRuta = false;
    tamCasilla = 0.f;
    danio = 25;
    usarSprite = false;
    reboteSonido = false;
    destruyeObstaculoSonido = false;
    golpeTanqueSonido = false;
    destruyeTanqueSonido = false;
    destruyoBarril = false;
    barrilDestruidoFila = -1;
    barrilDestruidoColumna = -1;

    try {
        AssetManager& assets = AssetManager::getInstance();
        sf::IntRect rect = assets.getSprite("bulletBlue2");
        spriteShape.setTexture(assets.getAtlasTexture());
        spriteShape.setTextureRect(rect);
        float escX = 14.f / rect.width;
        float escY = 14.f / rect.height;
        spriteShape.setScale(escX, escY); // 14 x 14 pixeles
        spriteShape.setOrigin(rect.width / 2.f, rect.height / 2.f);
        spriteShape.setPosition(pos[0], pos[1]);
        usarSprite = true;
    } catch (...) {
        shape = create_circle();
    }
    if (!usarSprite)
        shape = create_circle();

    par dir_copy[8] = {
        {radius + 0.2f, 0}, {-radius - 0.2f, 0},
        {0, radius + 0.2f}, {0, -radius - 0.2f},
        {radius*0.9f,  radius*0.9f},  {radius*0.9f,  -radius*0.9f},
        {-radius*0.9f, radius*0.9f},  {-radius*0.9f, -radius*0.9f}
    };
    for (int i = 0; i < 8; i++) dir[i] = dir_copy[i];
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
        float velocidadGuiada = 400.f; // pixeles por segundo
        float avance = velocidadGuiada * (delta / 400.f);
        pair<int,int> objetivo = rutaGuiada[pasoRuta];
        float ox = objetivo.second * tamCasilla + tamCasilla / 2.f;
        float oy = objetivo.first  * tamCasilla + tamCasilla / 2.f;
        float ddx = ox - pos[0];
        float ddy = oy - pos[1];
        float dist = std::sqrt(ddx*ddx + ddy*ddy);

        if (dist < avance) {
            pos[0] = ox;
            pos[1] = oy;
            pasoRuta++;
        } else {
            pos[0] += (ddx / dist) * avance;
            pos[1] += (ddy / dist) * avance;
        }
        if (pasoRuta >= rutaGuiada.size())
            isalive = false;
        return;
    }
    pos[0] += v[0] * delta;
    pos[1] += v[1] * delta;
}

void bullet::draw(sf::RenderWindow& window) {
    if (usarSprite) {
        window.draw(spriteShape);
    } else {
        window.draw(shape);
    }
}

void bullet::collition_check(Mapa& M) {
    bool flip_x = false;
    bool flip_y = false;
    float size = M.getTamanioCasilla();
    float lim_der = size * (float)M.getColumnas();
    float lim_inf = size * (float)M.getFilas();
    if (pos[0] <= 0 || pos[0] >= lim_der) flip_x = true;
    if (pos[1] <= 0 || pos[1] >= lim_inf) flip_y = true;
    int fila = (int)(pos[1] / size);
    int columna = (int)(pos[0] / size);

    if (fila >= 0 && fila < M.getFilas() &&
        columna >= 0 && columna < M.getColumnas()) {
        Casilla& C = M.getCasilla(fila, columna);
        if (C.estaOcupada() && C.getTanque() != nullptr && C.getTanque() != tirador) {
            int vidaAntes = (int)(C.getTanque()->getVidaPorcentaje() * 100);
            C.getTanque()->doDMG(danio);
            if (tirador != nullptr) {
                tirador->registrarDanioTanque(danio);
                if (!C.getTanque()->estaVivo())
                    tirador->registrarDestruccionTanque();
            }
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
            if (casillaColision.getTanque()->estaVivo())
                golpeTanqueSonido = true;
            casillaColision.getTanque()->doDMG(danio);
            if (tirador != nullptr) {
                tirador->registrarDanioTanque(danio);
                if (!casillaColision.getTanque()->estaVivo()) {
                    tirador->registrarDestruccionTanque();
                    destruyeTanqueSonido = true;
                }
            }
            isalive = false;
            return;
        }

        if (casillaColision.getTipo() != TipoCasilla::Suelo) {
            float cx  = pc * size + size / 2.f;
            float cy  = pf * size + size / 2.f;
            float ddx = std::abs(pos[0] - cx);
            float ddy = std::abs(pos[1] - cy);
            if (ddx >= ddy) flip_x = true;
            else flip_y = true;
            if (casillaColision.getTipo() != TipoCasilla::Obstaculo) {
                bool eraBarril = (casillaColision.getTipo() == TipoCasilla::Barril);
                bool vivoAntes = (casillaColision.getVida() > 0);
                casillaColision.recibirDanio(1);
                if (tirador != nullptr)
                    tirador->registrarDanioObstaculo(1);

                if (vivoAntes && casillaColision.getTipo() == TipoCasilla::Suelo) {
                    if (tirador != nullptr)
                        tirador->registrarObstaculoDestruido();
                    destruyeObstaculoSonido = true;

                    if (eraBarril) {
                        destruyoBarril      = true;
                        barrilDestruidoFila        = pf;
                        barrilDestruidoColumna     = pc;
                    }
                }
            }
        }
    }

    if (flip_x) v[0] = -v[0];
    if (flip_y) v[1] = -v[1];
    if (flip_x || flip_y) {
        wall_hits++;
        reboteSonido = true;
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
    danio = d;
}

void bullet::setSpriteColor(ColorTanque color) {
    const char* nombre;
    sf::Color tinte;

    switch (color) {
        case ColorTanque::Azul:
            nombre = "bulletBlue2";
            tinte  = sf::Color(80, 140, 255);
            break;
        case ColorTanque::Rojo:
            nombre = "bulletRed2";
            tinte  = sf::Color(255, 80, 80);
            break;
        case ColorTanque::Verde:
            nombre = "bulletGreen2";
            tinte  = sf::Color(80, 255, 120);
            break;
        case ColorTanque::Amarillo:
            nombre = "bulletSand2";
            tinte  = sf::Color(255, 220, 60);
            break;
        default:
            nombre = "bulletBlue2";
            tinte  = sf::Color(80, 140, 255);
            break;
    }

    try {
        AssetManager& assets = AssetManager::getInstance();
        sf::IntRect rect = assets.getSprite(nombre);
        spriteShape.setTexture(assets.getAtlasTexture());
        spriteShape.setTextureRect(rect);
        spriteShape.setColor(tinte);
        float escX = 16.f / rect.width;
        float escY = 16.f / rect.height;
        spriteShape.setScale(escX, escY);
        spriteShape.setOrigin(rect.width / 2.f, rect.height / 2.f);
        usarSprite = true;
    } catch (...) {
        usarSprite = false;
    }
}