#include "GestorTurnos.h"

GestorTurnos::GestorTurnos()
    : tanques(nullptr), cantidad(0), cantidadTurnos(0), turnoActual(0),
      estado(EstadoJuego::esperandoAccion),
      movimientoUsado(false), ataqueUsado(false) {
    for (int i = 0; i < 8; i++) ordenTurnos[i] = i;
}

void GestorTurnos::construirOrden() {
    int j1[4], j2[4];
    int c1 = 0, c2 = 0;
    for (int i = 0; i < cantidad; i++) {
        if (tanques[i].getJugador() == 1) j1[c1++] = i;
        else                              j2[c2++] = i;
    }
    cantidadTurnos = 0;
    int maxPares = c1 < c2 ? c1 : c2;
    for (int i = 0; i < maxPares; i++) {
        ordenTurnos[cantidadTurnos++] = j1[i];
        ordenTurnos[cantidadTurnos++] = j2[i];
    }
    for (int i = maxPares; i < c1; i++)
        ordenTurnos[cantidadTurnos++] = j1[i];
    for (int i = maxPares; i < c2; i++)
        ordenTurnos[cantidadTurnos++] = j2[i];
}

void GestorTurnos::inicializar(Tanque* arr, int cant) {
    tanques = arr;
    cantidad = cant;
    turnoActual = 0;
    estado = EstadoJuego::esperandoAccion;
    movimientoUsado = false;
    ataqueUsado = false;
    flagDobleTurno = false;
    turnosDobleRestantes = 0;
    flagPrecisionMovimiento = false;
    flagPrecisionAtaque = false;
    flagPoderAtaque = false;
    construirOrden();
}

int GestorTurnos::siguienteVivo(int desde) const {
    int sig = (desde + 1) % cantidadTurnos;
    int intentos = 0;
    while (!tanques[ordenTurnos[sig]].estaVivo() && intentos < cantidadTurnos) {
        sig = (sig + 1) % cantidadTurnos;
        intentos++;
    }
    return sig;
}

void GestorTurnos::siguienteTurno() {
    if (flagDobleTurno && turnosDobleRestantes > 0) {
        turnosDobleRestantes--;
        if (turnosDobleRestantes == 0)
            flagDobleTurno = false;
    } else {
        turnoActual = siguienteVivo(turnoActual);
    }
    estado = EstadoJuego::esperandoAccion;
    movimientoUsado = false;
    ataqueUsado = false;
}

void GestorTurnos::activarDobleTurno() {
    flagDobleTurno = true;
    turnosDobleRestantes = 2;
}

void GestorTurnos::activarPrecisionMovimiento() {
    flagPrecisionMovimiento = true;
}

void GestorTurnos::activarPrecisionAtaque() {
    flagPrecisionAtaque = true;
}

void GestorTurnos::activarPoderAtaque() {
    flagPoderAtaque = true;
}

void GestorTurnos::consumirFlagMovimiento() {
    flagPrecisionMovimiento = false;
}

void GestorTurnos::consumirFlagAtaque() {
    flagPrecisionAtaque = false;
    flagPoderAtaque = false;
}

bool GestorTurnos::tienePrecisionMovimiento() const {
    return flagPrecisionMovimiento;
}
bool GestorTurnos::tienePrecisionAtaque()     const {
    return flagPrecisionAtaque;
}
bool GestorTurnos::tienePoderAtaque()         const {
    return flagPoderAtaque;
}

void GestorTurnos::usarMovimiento() {
    movimientoUsado = true;
    estado = ataqueUsado ? EstadoJuego::ambasAccionesUsadas : EstadoJuego::movimientoUsado;
}

void GestorTurnos::usarAtaque() {
    ataqueUsado = true;
    estado = movimientoUsado ? EstadoJuego::ambasAccionesUsadas : EstadoJuego::ataqueUsado;
}

bool GestorTurnos::puedeMover() const {
    return estado == EstadoJuego::esperandoAccion || estado == EstadoJuego::ataqueUsado;
}

bool GestorTurnos::puedeAtacar() const {
    return estado == EstadoJuego::esperandoAccion || estado == EstadoJuego::movimientoUsado;
}

Tanque* GestorTurnos::getTanqueActual() {
    return &tanques[ordenTurnos[turnoActual]];
}

int GestorTurnos::getJugadorActual() const {
    return tanques[ordenTurnos[turnoActual]].getJugador();
}

bool GestorTurnos::powerUpUsado() {
    Tanque* t = getTanqueActual();
    if (!t->consumirPowerUp()) return false;
    estado = EstadoJuego::ambasAccionesUsadas;
    return true;
}

PowerUp* GestorTurnos::aplicarPowerUpPendiente() {
    return getTanqueActual()->tomarPowerUpPendiente();
}

EstadoJuego GestorTurnos::getEstado() const {return estado;}
void GestorTurnos::setEstado(EstadoJuego e) {estado = e;}