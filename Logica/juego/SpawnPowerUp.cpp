#include "SpawnPowerUp.h"
#include "../assets/AssetManager.h"
#include <random>
#include <cmath>

SpawnPowerUp::SpawnPowerUp() : cantActivos(0), cantHistorial(0), inicializado(false) {
    for (int i = 0; i < maxEnMapa; i++)
        powerUps[i].activo = false;
}

const char* SpawnPowerUp::nombreTexturaPorTipo(TipoPowerUp tipo) {
    switch (tipo) {
        case TipoPowerUp::DobleTurno: return "powerupDobleTurno";
        case TipoPowerUp::PrecisionMovimiento: return "powerupPrecisionMovimiento";
        case TipoPowerUp::PrecisionAtaque: return "powerupPrecisionAtaque";
        case TipoPowerUp::PoderAtaque: return "powerupAtaque";
    }
    return "powerupDobleTurno";
}

void SpawnPowerUp::inicializar() {
    AssetManager& assets = AssetManager::getInstance();
    TipoPowerUp tipos[4] = {
        TipoPowerUp::DobleTurno,
        TipoPowerUp::PrecisionMovimiento,
        TipoPowerUp::PrecisionAtaque,
        TipoPowerUp::PoderAtaque
    };

    for (int i = 0; i < 4; i++) {
        const sf::Texture& tex = assets.getPowerUpTexture(nombreTexturaPorTipo(tipos[i]));
        spritePorTipo[i].setTexture(tex);
        float escX = 36.f / tex.getSize().x;
        float escY = 36.f / tex.getSize().y;
        spritePorTipo[i].setScale(escX, escY);
    }
    inicializado = true;
}

bool SpawnPowerUp::posicionValida(int f, int c, Mapa& mapa, Tanque* tanques, int cantTanques) const {
    if (mapa.getCasilla(f, c).getTipo() != TipoCasilla::Suelo) return false;
    if (mapa.getCasilla(f, c).estaOcupada()) return false;

    for (int i = 0; i < maxEnMapa; i++)
        if (powerUps[i].activo &&
            powerUps[i].fila == f &&
            powerUps[i].columna == c) return false;

    for (int t = 0; t < cantTanques; t++) {
        if (!tanques[t].estaVivo()) continue;
        int tf = tanques[t].getFila();
        int tc = tanques[t].getColumna();
        int df = f - tf; if (df < 0) df = -df;
        int dc = c - tc; if (dc < 0) dc = -dc;
        if (df == 0 && dc <= 3) return false;
        if (dc == 0 && df <= 3) return false;
        if (df > 0 && dc > 0 && df <= 2 && dc <= 2) return false;
    }
    return true;
}

TipoPowerUp SpawnPowerUp::elegirTipo() {
    static std::mt19937 rng(std::random_device{}());
    TipoPowerUp todos[4] = {
        TipoPowerUp::DobleTurno,
        TipoPowerUp::PrecisionMovimiento,
        TipoPowerUp::PrecisionAtaque,
        TipoPowerUp::PoderAtaque
    };
    
    TipoPowerUp Elegibles[4];
    int cantElegibles = 0;

    for (int i = 0; i < 4; i++) {
        bool enHistorial = false;
        for (int h = 0; h < cantHistorial; h++)
            if (historial[h] == todos[i]) { enHistorial = true; break; }
        if (!enHistorial)
            Elegibles[cantElegibles++] = todos[i];
    }
    
    if (cantElegibles == 0) {
        for (int i = 0; i < 4; i++) Elegibles[i] = todos[i];
        cantElegibles = 4;
    }

    std::uniform_int_distribution<int> dist(0, cantElegibles - 1);
    TipoPowerUp elegido = Elegibles[dist(rng)];
    if (cantHistorial < 2) {
        historial[cantHistorial++] = elegido;
    } else {
        historial[0] = historial[1];
        historial[1] = elegido;
    }
    return elegido;
}

void SpawnPowerUp::intentarSpawn(Mapa& mapa, Tanque* tanques, int cantTanques) {
    if (cantActivos >= maxEnMapa) return;
    int slot = -1;
    for (int i = 0; i < maxEnMapa; i++)
        if (!powerUps[i].activo) { slot = i; break; }
    if (slot == -1) return;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> randF(0, mapa.getFilas()    - 1);
    std::uniform_int_distribution<int> randC(0, mapa.getColumnas() - 1);

    for (int intento = 0; intento < 200; intento++) {
        int f = randF(rng);
        int c = randC(rng);
        if (posicionValida(f, c, mapa, tanques, cantTanques)) {
            powerUps[slot].tipo = elegirTipo();
            powerUps[slot].fila = f;
            powerUps[slot].columna = c;
            powerUps[slot].activo = true;
            cantActivos++;
            return;
        }
    }
}

PowerUpEnMapa* SpawnPowerUp::recoger(int fila, int columna) {
    for (int i = 0; i < maxEnMapa; i++) {
        if (powerUps[i].activo && powerUps[i].fila == fila && powerUps[i].columna == columna) {powerUps[i].activo = false;
            cantActivos--;
            return &powerUps[i];
        }
    }
    return nullptr;
}

void SpawnPowerUp::dibujar(sf::RenderWindow& ventana, float tamanioCasilla) {
    if (!inicializado) return;
    for (int i = 0; i < maxEnMapa; i++) {
        if (!powerUps[i].activo) continue;
        int indiceSprite = (int)powerUps[i].tipo;
        sf::Sprite& sp   = spritePorTipo[indiceSprite];
        float px = powerUps[i].columna * tamanioCasilla + (tamanioCasilla - 36.f) / 2.f;
        float py = powerUps[i].fila    * tamanioCasilla + (tamanioCasilla - 36.f) / 2.f;
        sp.setPosition(px, py);
        ventana.draw(sp);
    }
}

int SpawnPowerUp::getCantidadActivos() const { return cantActivos; }