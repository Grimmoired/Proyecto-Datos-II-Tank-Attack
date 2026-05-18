#pragma once

enum class TipoPowerUp {
    DobleTurno,
    PrecisionMovimiento,
    PrecisionAtaque,
    PoderAtaque
};

struct PowerUp {
    TipoPowerUp tipo;
    bool pendiente;
    PowerUp() : tipo(TipoPowerUp::DobleTurno), pendiente(false) {}
    explicit PowerUp(TipoPowerUp t) : tipo(t), pendiente(false) {}
};