#include "Tanque.h"
#include <cmath>
#include <utility>

Tanque::Tanque() : fila(0), columna(0), color(ColorTanque::Azul), direccion(DireccionTanque::Arriba), jugador(1), indice(0), vivo(true), vida(100), cantidadPUCola(0), powerUpPendiente(false), flagPrecisionAtaque(false),
tanquesEnemigosDestruidos(0), danioTotalTanques(0), danioTotalObstaculos(0), obstaculosDestruidos(0), powerUpsUsados(0) {}

Tanque::Tanque(int fila, int columna, ColorTanque color, int jugador, int indice) : fila(fila), columna(columna), color(color), direccion(DireccionTanque::Arriba), jugador(jugador), indice(indice), vivo(true), vida(100),
cantidadPUCola(0),powerUpPendiente(false), flagPrecisionAtaque(false), tanquesEnemigosDestruidos(0), danioTotalTanques(0), danioTotalObstaculos(0), obstaculosDestruidos(0), powerUpsUsados(0) {}

Tanque& Tanque::operator=(Tanque&& otro) noexcept {
    if (this == &otro) return *this;
    fila = otro.fila;
    columna = otro.columna;
    color = otro.color;
    direccion = otro.direccion;
    jugador = otro.jugador;
    indice = otro.indice;
    vivo = otro.vivo;
    vida = otro.vida;
    powerUpPendiente = otro.powerUpPendiente;
    pendiente = otro.pendiente;
    cantidadPUCola = otro.cantidadPUCola;
    spriteCuerpo = otro.spriteCuerpo;
    colaPowerUps = std::move(otro.colaPowerUps);
    for (int i = 0; i < cantidadPUCola; i++)
        powerUpsEnCola[i] = otro.powerUpsEnCola[i];
    tanquesEnemigosDestruidos = otro.tanquesEnemigosDestruidos;
    danioTotalTanques = otro.danioTotalTanques;
    danioTotalObstaculos = otro.danioTotalObstaculos;
    obstaculosDestruidos = otro.obstaculosDestruidos;
    powerUpsUsados = otro.powerUpsUsados;
    return *this;
}

float Tanque::rotacionPorDireccion(DireccionTanque dir) const {
    switch (dir) {
        case DireccionTanque::Abajo: return 0.f;
        case DireccionTanque::Izquierda: return 90.f;
        case DireccionTanque::Arriba: return 180.f;
        case DireccionTanque::Derecha: return 270.f;
    }
    return 0.f;
}

void Tanque::inicializarSprite(const sf::Texture& atlas, const sf::IntRect& rectCuerpo, float tamanioCasilla) {
    spriteCuerpo.setTexture(atlas);
    spriteCuerpo.setTextureRect(rectCuerpo);
    spriteCuerpo.setScale(tamanioCasilla / rectCuerpo.width, tamanioCasilla / rectCuerpo.height);
    spriteCuerpo.setOrigin(rectCuerpo.width  / 2.f, rectCuerpo.height / 2.f);
    spriteCuerpo.setPosition(columna * tamanioCasilla + tamanioCasilla / 2.f, fila    * tamanioCasilla + tamanioCasilla / 2.f);
    spriteCuerpo.setRotation(rotacionPorDireccion(direccion));
}

void Tanque::setDireccion(DireccionTanque dir) {
    direccion = dir;
    spriteCuerpo.setRotation(rotacionPorDireccion(dir));
}

void Tanque::dibujar(sf::RenderWindow& ventana) const {
    if (!vivo) return;
    ventana.draw(spriteCuerpo);
}

void Tanque::dibujarResaltado(sf::RenderWindow& ventana, float tamanioCasilla, float tiempoTotal) const {
    if (!vivo) return;
    float pulso = (std::sin(tiempoTotal * 4.f) + 1.f) / 2.f;
    sf::Uint8 alpha = (sf::Uint8)(120 + pulso * 135);
    sf::RectangleShape outerBox;
    outerBox.setSize({tamanioCasilla + 8, tamanioCasilla + 8});
    outerBox.setPosition(columna * tamanioCasilla - 4, fila    * tamanioCasilla - 4);
    outerBox.setFillColor(sf::Color::Transparent);
    outerBox.setOutlineColor(sf::Color(0, 255, 255, alpha));
    outerBox.setOutlineThickness(4);
    ventana.draw(outerBox);

    sf::RectangleShape innerBox;
    innerBox.setSize({tamanioCasilla - 4, tamanioCasilla - 4});
    innerBox.setPosition(columna * tamanioCasilla + 2, fila    * tamanioCasilla + 2);
    innerBox.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(pulso * 40)));
    innerBox.setOutlineColor(sf::Color(255, 255, 255, (sf::Uint8)(pulso * 180)));
    innerBox.setOutlineThickness(2);
    ventana.draw(innerBox);
    ventana.draw(spriteCuerpo);
}

void Tanque::doDMG(int cantidad) {
    if (!vivo) return;
    vida -= cantidad;
    if (vida < 0) vida = 0;
    if (vida == 0) vivo = false;
}

bool Tanque::agregarPowerUp(TipoPowerUp tipo) {
    if (colaPowerUps.size() >= maxPowerUps) return false;
    colaPowerUps.encolar(PowerUp(tipo));
    if (cantidadPUCola < maxPowerUps)
        powerUpsEnCola[cantidadPUCola++] = tipo;
    return true;
}

bool Tanque::consumirPowerUp() {
    if (colaPowerUps.vacia()) return false;
    pendiente = colaPowerUps.desencolar();
    pendiente.pendiente = true;
    powerUpPendiente = true;
    for (int i = 0; i < cantidadPUCola - 1; i++)
        powerUpsEnCola[i] = powerUpsEnCola[i + 1];
    if (cantidadPUCola > 0) cantidadPUCola--;
    return true;
}

PowerUp* Tanque::tomarPowerUpPendiente() {
    if (!powerUpPendiente) return nullptr;
    powerUpPendiente = false;
    return &pendiente;
}

bool Tanque::tienePowerUpPendiente() const {
    return powerUpPendiente;
}

void Tanque::getTiposPowerUps(TipoPowerUp* tipos, int maxSlots) const {
    for (int i = 0; i < maxSlots && i < cantidadPUCola; i++)
        tipos[i] = powerUpsEnCola[i];
}

void Tanque::actualizarPosicionSprite(float tamCasilla) {
    spriteCuerpo.setPosition(columna * tamCasilla + tamCasilla/2.0f, fila * tamCasilla + tamCasilla/2.0f);
}

int Tanque::getRango() const {
    if (color == ColorTanque::Azul || color == ColorTanque::Rojo)
        return rangoAzulRojo;
    return rangoVerdeAmarillo;
}

void Tanque::setPosicionSprite(float x, float y) {
    spriteCuerpo.setPosition(x, y);
}

int Tanque::getFila() const { return fila; }
int Tanque::getColumna() const { return columna; }
ColorTanque Tanque::getColor() const { return color; }
int Tanque::getJugador() const { return jugador; }
int Tanque::getIndice() const { return indice; }
bool Tanque::estaVivo() const { return vivo; }
float Tanque::getVidaPorcentaje() const { return vida / 100.f; }
int  Tanque::getCantidadPowerUps()const { return colaPowerUps.size(); }
void Tanque::setFila(int f) { fila    = f; }
void Tanque::setColumna(int c) { columna = c; }
void Tanque::activarPrecisionAtaque() { flagPrecisionAtaque = true; }
bool Tanque::tienePrecisionAtaque() const { return flagPrecisionAtaque; }
void Tanque::consumirPrecisionAtaque() { flagPrecisionAtaque = false; }

void Tanque::registrarDestruccionTanque() { tanquesEnemigosDestruidos++; }
void Tanque::registrarDanioTanque(int d) { danioTotalTanques += d; }
void Tanque::registrarDanioObstaculo(int d) { danioTotalObstaculos += d; }
void Tanque::registrarObstaculoDestruido() { obstaculosDestruidos++; }
void Tanque::registrarPowerUpUsado() { powerUpsUsados++; }
int Tanque::getTanquesDestruidos() const { return tanquesEnemigosDestruidos; }
int Tanque::getDanioTotalTanques() const { return danioTotalTanques; }
int Tanque::getDanioTotalObstaculos() const { return danioTotalObstaculos; }
int Tanque::getObstaculosDestruidos() const { return obstaculosDestruidos; }
int Tanque::getPowerUpsUsados() const { return powerUpsUsados; }