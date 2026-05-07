#include "Tanque.h"
#include <cmath>

Tanque::Tanque(): fila(0), columna(0), color(ColorTanque::Azul), direccion(DireccionTanque::Arriba), jugador(1), indice(0), vivo(true) {}
Tanque::Tanque(int fila, int columna, ColorTanque color, int jugador, int indice): fila(fila), columna(columna), color(color), direccion(DireccionTanque::Arriba), jugador(jugador), indice(indice), vivo(true) {}

float Tanque::rotacionPorDireccion(DireccionTanque dir) const {
    switch (dir) {
        case DireccionTanque::Abajo: return 0.f;
        case DireccionTanque::Izquierda: return 90.f;
        case DireccionTanque::Arriba: return 180.f;
        case DireccionTanque::Derecha: return 270.f;
    }
    return 0.f;
}

void Tanque::inicializarSprite(const sf::Texture& atlas, const sf::IntRect& rectCuerpo, float tamañoCasilla) {
    spriteCuerpo.setTexture(atlas);
    spriteCuerpo.setTextureRect(rectCuerpo);
    spriteCuerpo.setScale(tamañoCasilla / rectCuerpo.width, tamañoCasilla / rectCuerpo.height);
    spriteCuerpo.setOrigin(rectCuerpo.width  / 2.f, rectCuerpo.height / 2.f);
    spriteCuerpo.setPosition(columna * tamañoCasilla + tamañoCasilla / 2.f, fila * tamañoCasilla + tamañoCasilla / 2.f);
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

void Tanque::dibujarResaltado(sf::RenderWindow& ventana, float tamañoCasilla, float tiempoTotal) const {
    if (!vivo) return;
    float pulso = (std::sin(tiempoTotal * 4.f) + 1.f) / 2.f;
    sf::Uint8 alpha = (sf::Uint8)(120 + pulso * 135);
    sf::RectangleShape outerBox;
    outerBox.setSize({tamañoCasilla + 8, tamañoCasilla + 8});
    outerBox.setPosition(columna * tamañoCasilla - 4, fila    * tamañoCasilla - 4);
    outerBox.setFillColor(sf::Color::Transparent);
    outerBox.setOutlineColor(sf::Color(0, 255, 255, alpha));
    outerBox.setOutlineThickness(4);
    ventana.draw(outerBox);
    
    sf::RectangleShape innerBox;
    innerBox.setSize({tamañoCasilla - 4, tamañoCasilla - 4});
    innerBox.setPosition(columna * tamañoCasilla + 2, fila    * tamañoCasilla + 2);
    innerBox.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(pulso * 40)));
    innerBox.setOutlineColor(sf::Color(255, 255, 255, (sf::Uint8)(pulso * 180)));
    innerBox.setOutlineThickness(2);
    ventana.draw(innerBox);
    ventana.draw(spriteCuerpo);
}

int Tanque::getFila()    const {return fila; }
int Tanque::getColumna() const {return columna; }
ColorTanque Tanque::getColor() const { return color; }
int Tanque::getJugador() const {return jugador; }
int Tanque::getIndice()  const {return indice; }
bool Tanque::estaVivo()   const { return vivo; }
void Tanque::setFila(int f)     {fila = f; }
void Tanque::setColumna(int c)  {columna = c; }