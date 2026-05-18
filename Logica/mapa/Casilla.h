#pragma once
class Tanque;
enum class TipoCasilla {
    Suelo,
    Obstaculo,
    DestruibleDebil,
    DestruibleFuerte,
    Barril
};

class Casilla {
public:
    Casilla();
    Casilla(int fila, int columna, TipoCasilla tipo);

    int getFila() const;
    int getColumna() const;
    TipoCasilla getTipo() const;
    bool esObstaculo() const;
    bool esTransitable() const;
    bool estaOcupada() const;
    float getPixelX() const;
    float getPixelY() const;
    int getVida() const;
    Tanque* getTanque() const;

    void setTipo(TipoCasilla tipo);
    void setOcupada(bool ocupada);
    void setTanque(Tanque* tanque);
    void setPixel(float x, float y);
    void recibirDanio(int cantidad);

private:
    int fila;
    int columna;
    TipoCasilla tipo;
    bool ocupada;
    float pixelX;
    float pixelY;
    int vida;
    Tanque* tanqueOcupante; 
};