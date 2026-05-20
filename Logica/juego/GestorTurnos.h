#pragma once
#include "Tanque.h"
#include "EstadoJuego.h"
#include "PowerUp.h"

class GestorTurnos {
public:
    GestorTurnos();
    void inicializar(Tanque* tanques, int cantidad);
    void siguienteTurno();
    Tanque* getTanqueActual();
    int getJugadorActual()  const;
    EstadoJuego getEstado()         const;
    void setEstado(EstadoJuego estado);
    bool puedeMover() const;
    bool puedeAtacar() const;
    void usarMovimiento();
    void usarAtaque();
    PowerUp* aplicarPowerUpPendiente();
    bool powerUpUsado();
    bool tienePrecisionMovimiento() const;
    bool tienePrecisionAtaque() const;
    bool tienePoderAtaque() const;
    void activarDobleTurno();
    void activarPrecisionMovimiento();
    void activarPrecisionAtaque();
    void activarPoderAtaque();
    void consumirFlagAtaque();
    void consumirFlagMovimiento();

private:
    Tanque* tanques;
    int cantidad;
    int ordenTurnos[8];
    int cantidadTurnos;
    int turnoActual;
    EstadoJuego estado;
    bool movimientoUsado;
    bool ataqueUsado;
    int siguienteVivo(int desde) const;
    void construirOrden();
    bool flagDobleTurno;
    int  turnosDobleRestantes;
    bool flagPrecisionMovimiento;
    bool flagPrecisionAtaque;
    bool flagPoderAtaque;
};