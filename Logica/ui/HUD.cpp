#include "HUD.h"
#include "../juego/EstadoJuego.h"
#include "../assets/AssetManager.h"
#include <sstream>
#include <iomanip>
#include <cstring>

HUD::HUD(sf::RenderWindow& ventana) : ventana(ventana) {
    fondoHUD.setSize({(float)anchoVentana, (float)altoHUD});
    fondoHUD.setPosition(0, (float)yHUD);
    fondoHUD.setFillColor(sf::Color(18, 14, 8));
    fondoHUD.setOutlineColor(sf::Color(176, 128, 48));
    fondoHUD.setOutlineThickness(2);
    divisorIzq.setSize({2, (float)altoHUD - 20});
    divisorIzq.setPosition((float)xZonaCentral, (float)yHUD + 10);
    divisorIzq.setFillColor(sf::Color(176, 128, 48, 120));
    divisorDer.setSize({2, (float)altoHUD - 20});
    divisorDer.setPosition((float)(xZonaCentral + anchoZonaCentral), (float)yHUD + 10);
    divisorDer.setFillColor(sf::Color(176, 128, 48, 120));

    nombreJ1 = crearTexto("-- JUGADOR 1 --", 28, sf::Color(96, 144, 208), (float)xZonaJ1 + 20, (float)yHUD + 14);
    nombreJ2 = crearTexto("-- JUGADOR 2 --", 28, sf::Color(96, 192, 80), (float)xZonaJ2 + 20, (float)yHUD + 14);
    textoTiempoLabel = crearTexto("TIEMPO RESTANTE", 22, sf::Color(192, 160, 64), 0, 0);
    {
        sf::FloatRect b = textoTiempoLabel.getLocalBounds();
        textoTiempoLabel.setOrigin(b.left + b.width / 2.f, 0);
        textoTiempoLabel.setPosition((float)xZonaCentral + 400, (float)yHUD + 14);
    }

    textoTiempo = crearTexto("05:00", 56, sf::Color(255, 221, 96), 0, 0);
    {
        sf::FloatRect b = textoTiempo.getLocalBounds();
        textoTiempo.setOrigin(b.left + b.width / 2.f, 0);
        textoTiempo.setPosition((float)xZonaCentral + 400, (float)yHUD + 40);
    }

    textoTurno = crearTexto("Turno: JUGADOR 1", 24,
                             sf::Color(128, 192, 64), 0, 0);
    {
        sf::FloatRect b = textoTurno.getLocalBounds();
        textoTurno.setOrigin(b.left + b.width / 2.f, 0);
        textoTurno.setPosition((float)xZonaCentral + 400, (float)yHUD + 104);
    }

    float yBotones = (float)yHUD + 134;
    float xBase    = (float)xZonaCentral + (anchoZonaCentral - 268) / 2.f;
    botonMover    = crearBoton(xBase,       yBotones, 80,  30, sf::Color(26, 58, 26),  sf::Color(64, 160, 64));
    botonAtacar   = crearBoton(xBase + 90,  yBotones, 80,  30, sf::Color(58, 26, 26),  sf::Color(160, 64, 64));
    botonFinTurno = crearBoton(xBase + 180, yBotones, 88,  30, sf::Color(42, 32, 16),  sf::Color(160, 128, 48));
    textoMover    = crearTexto("Mover", 22, sf::Color(128, 255, 128), 0, 0);
    textoAtacar   = crearTexto("Atacar", 22, sf::Color(255, 128, 128), 0, 0);
    textoFinTurno = crearTexto("Finalizar turno",20, sf::Color(255, 192, 64),  0, 0);
    auto centrar = [](sf::Text& txt, const sf::RectangleShape& btn) {
        sf::FloatRect b = txt.getLocalBounds();
        txt.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        txt.setPosition(btn.getPosition().x + btn.getSize().x / 2.f, btn.getPosition().y + btn.getSize().y / 2.f);
    };

    centrar(textoMover,    botonMover);
    centrar(textoAtacar,   botonAtacar);
    centrar(textoFinTurno, botonFinTurno);
    estadoMover  = crearTexto("disponible", 16, sf::Color(64, 128, 64), 0, yBotones + 34);
    estadoAtacar = crearTexto("disponible", 16, sf::Color(64, 128, 64), 0, yBotones + 34);
    {
        auto centrarLabel = [](sf::Text& t, const sf::RectangleShape& btn) {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width / 2.f, 0);
            t.setPosition(btn.getPosition().x + btn.getSize().x / 2.f, t.getPosition().y);
        };
        centrarLabel(estadoMover,  botonMover);
        centrarLabel(estadoAtacar, botonAtacar);
    }
    construirFilas();
}

sf::Text HUD::crearTexto(const char* str, unsigned int tam, sf::Color color, float x, float y) {
    sf::Text t;
    t.setFont(AssetManager::getInstance().getFuente());
    t.setString(str);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition(x, y);
    return t;
}

sf::RectangleShape HUD::crearBoton(float x, float y, float w, float h, sf::Color relleno, sf::Color borde) {
    sf::RectangleShape btn;
    btn.setSize({w, h});
    btn.setPosition(x, y);
    btn.setFillColor(relleno);
    btn.setOutlineColor(borde);
    btn.setOutlineThickness(2);
    return btn;
}

void HUD::construirFilas() {
    struct DefColor {
        sf::Color barra;
        sf::Color texto;
    };
    DefColor defsJ1[4] = {
        {sf::Color(48, 112, 192), sf::Color(160, 192, 255)},
        {sf::Color(48, 112, 192), sf::Color(160, 192, 255)},
        {sf::Color(192, 48,  48), sf::Color(255, 144, 144)},
        {sf::Color(192, 48,  48), sf::Color(255, 144, 144)}
    };
    DefColor defsJ2[4] = {
        {sf::Color(48, 160, 48),  sf::Color(144, 255, 144)},
        {sf::Color(48, 160, 48),  sf::Color(144, 255, 144)},
        {sf::Color(176, 144, 32), sf::Color(255, 224, 128)},
        {sf::Color(176, 144, 32), sf::Color(255, 224, 128)}
    };
    const char* etiquetasJ1[4] = {"AZL 1", "AZL 2", "ROJ 1", "ROJ 2"};
    const char* etiquetasJ2[4] = {"VRD 1", "VRD 2", "AMR 1", "AMR 2"};

    auto construirFila = [&](FilaTanque& fila, const char* etiqueta, const DefColor& def, float xBase, float yFila) {
        fila.colorOriginal = def.barra;
        fila.etiqueta = crearTexto(etiqueta, 22, def.texto, xBase + 8, yFila + 2);
        fila.barraFondo.setSize({110, 16});
        fila.barraFondo.setPosition(xBase + 72, yFila + 2);
        fila.barraFondo.setFillColor(sf::Color(20, 16, 8));
        fila.barraFondo.setOutlineColor(sf::Color(60, 50, 30));
        fila.barraFondo.setOutlineThickness(1);
        fila.barraVida.setSize({110, 16});
        fila.barraVida.setPosition(xBase + 72, yFila + 2);
        fila.barraVida.setFillColor(def.barra);
        fila.porcentaje = crearTexto("100%", 18, def.texto, xBase + 188, yFila + 2);

        for (int s = 0; s < 3; s++) {
            fila.slotOcupado[s] = false;
            fila.slotsPU[s].setSize({18, 16});
            fila.slotsPU[s].setPosition(xBase + 240 + s * 22, yFila + 2);
            fila.slotsPU[s].setFillColor(sf::Color(24, 20, 12));
            fila.slotsPU[s].setOutlineColor(sf::Color(48, 40, 20));
            fila.slotsPU[s].setOutlineThickness(1);
        }
    };

    float yBase = (float)yHUD + 42;
    float sep   = 36;
    for (int i = 0; i < 4; i++) {
        float yFila = yBase + i * sep + (i >= 2 ? 8 : 0);
        construirFila(filasJ1[i], etiquetasJ1[i], defsJ1[i], (float)xZonaJ1 + 8, yFila);
        construirFila(filasJ2[i], etiquetasJ2[i], defsJ2[i], (float)xZonaJ2 + 8, yFila);
    }
}

void HUD::actualizarFila(FilaTanque& fila, const DatosTanque& datos) {
    if (!datos.vivo) {
        fila.barraVida.setSize({0, 16});
        fila.porcentaje.setString("---");
        fila.etiqueta.setFillColor(sf::Color(60, 50, 40));
        for (int s = 0; s < 3; s++) {
            fila.slotsPU[s].setFillColor(sf::Color(24, 20, 12));
            fila.slotsPU[s].setOutlineColor(sf::Color(48, 40, 20));
            fila.slotOcupado[s] = false;
        }
        return;
    }

    float pct = datos.vidaPorcentaje;
    if (pct < 0) pct = 0;
    if (pct > 1) pct = 1;
    fila.barraVida.setSize({110.f * pct, 16});
    if (pct > 0.5f)
        fila.barraVida.setFillColor(fila.colorOriginal);
    else if (pct > 0.25f)
        fila.barraVida.setFillColor(sf::Color(192, 144, 32));
    else
        fila.barraVida.setFillColor(sf::Color(192, 48, 48));

    int pctInt = (int)(pct * 100.f);
    std::ostringstream ss;
    ss << pctInt << "%";
    fila.porcentaje.setString(ss.str());
    AssetManager& assets = AssetManager::getInstance();

    auto nombrePU = [](TipoPowerUp tipo) -> const char* {
        switch (tipo) {
            case TipoPowerUp::DobleTurno:
                return "powerupDobleTurno";
            case TipoPowerUp::PrecisionMovimiento:
                return "powerupPrecisionMovimiento";
            case TipoPowerUp::PrecisionAtaque:
                return "powerupPrecisionAtaque";
            case TipoPowerUp::PoderAtaque:
                return "powerupAtaque";
        }
        return "powerupDobleTurno";
    };

    for (int s = 0; s < 3; s++) {
        if (s < datos.powerUpsEnCola) {
            fila.slotsPU[s].setFillColor(sf::Color(42, 64, 20));
            fila.slotsPU[s].setOutlineColor(sf::Color(96, 160, 48));
            fila.slotOcupado[s] = true;
            const sf::Texture& tex = assets.getPowerUpTexture(nombrePU(datos.tiposPU[s]));
            fila.spritesPU[s].setTexture(tex);
            float esc = 14.f / tex.getSize().y;
            fila.spritesPU[s].setScale(esc,esc);
            fila.spritesPU[s].setPosition(fila.slotsPU[s].getPosition().x + 2, fila.slotsPU[s].getPosition().y + 1);
        } else {
            fila.slotsPU[s].setFillColor(sf::Color(24, 20, 12));
            fila.slotsPU[s].setOutlineColor(sf::Color(48, 40, 20));
            fila.slotOcupado[s] = false;
        }
    }
}

void HUD::actualizarBotones(EstadoJuego estado) {
    bool puedeMover  = (estado == EstadoJuego::esperandoAccion || estado == EstadoJuego::ataqueUsado);
    bool puedeAtacar = (estado == EstadoJuego::esperandoAccion || estado == EstadoJuego::movimientoUsado);
    botonMover.setFillColor(puedeMover ? sf::Color(26, 58, 26) : sf::Color(20, 20, 20));
    botonMover.setOutlineColor(puedeMover ? sf::Color(64, 160, 64) : sf::Color(50, 50, 50));
    textoMover.setFillColor(puedeMover ? sf::Color(128, 255, 128) : sf::Color(60, 80, 60));
    estadoMover.setString(puedeMover ? "disponible" : "usado");
    estadoMover.setFillColor(puedeMover ? sf::Color(64, 128, 64) : sf::Color(100, 50, 50));
    botonAtacar.setFillColor(puedeAtacar ? sf::Color(58, 26, 26) : sf::Color(20, 20, 20));
    botonAtacar.setOutlineColor(puedeAtacar ? sf::Color(160, 64, 64) : sf::Color(50, 50, 50));
    textoAtacar.setFillColor(puedeAtacar ? sf::Color(255, 128, 128) : sf::Color(80, 60, 60));
    estadoAtacar.setString(puedeAtacar ? "disponible" : "usado");
    estadoAtacar.setFillColor(puedeAtacar ? sf::Color(64, 128, 64) : sf::Color(100, 50, 50));

    auto centrarLabel = [](sf::Text& t, const sf::RectangleShape& btn) {
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width / 2.f, 0);
        t.setPosition(btn.getPosition().x + btn.getSize().x / 2.f, t.getPosition().y);
    };
    centrarLabel(estadoMover,  botonMover);
    centrarLabel(estadoAtacar, botonAtacar);
}

void HUD::actualizar(const DatosJugador& j1, const DatosJugador& j2, float tiempoRestante, int turnoActual, EstadoJuego estado) {
    if (tiempoRestante < 0) tiempoRestante = 0;
    int minutos  = (int)tiempoRestante / 60;
    int segundos = (int)tiempoRestante % 60;

    std::ostringstream ss;
    ss << (minutos  < 10 ? "0" : "") << minutos
       << ":"
       << (segundos < 10 ? "0" : "") << segundos;
    textoTiempo.setString(ss.str());
    textoTiempo.setFillColor(tiempoRestante < 60.f ? sf::Color(255, 80, 80) : sf::Color(255, 221, 96));
    {
        sf::FloatRect b = textoTiempo.getLocalBounds();
        textoTiempo.setOrigin(b.left + b.width / 2.f, 0);
        textoTiempo.setPosition((float)xZonaCentral + 400, (float)yHUD + 30);
    }

    std::string nombreTurno = (turnoActual == 1) ? j1.nombre : j2.nombre;
    textoTurno.setString(("TURNO: " + nombreTurno).c_str());
    textoTurno.setFillColor(turnoActual == 1 ? sf::Color(96, 160, 255) : sf::Color(96, 208, 64));
    {
        sf::FloatRect b = textoTurno.getLocalBounds();
        textoTurno.setOrigin(b.left + b.width / 2.f, 0);
        textoTurno.setPosition((float)xZonaCentral + 400, (float)yHUD + 94);
    }
    for (int i = 0; i < 4; i++) {
        actualizarFila(filasJ1[i], j1.tanques[i]);
        actualizarFila(filasJ2[i], j2.tanques[i]);
    }
    actualizarBotones(estado);
}

void HUD::dibujarFila(const FilaTanque& fila) {
    ventana.draw(fila.etiqueta);
    ventana.draw(fila.barraFondo);
    ventana.draw(fila.barraVida);
    ventana.draw(fila.porcentaje);
    for (int s = 0; s < 3; s++) {
        ventana.draw(fila.slotsPU[s]);
        if (fila.slotOcupado[s])
            ventana.draw(fila.spritesPU[s]);
    }
}

void HUD::dibujar() {
    ventana.draw(fondoHUD);
    ventana.draw(divisorIzq);
    ventana.draw(divisorDer);
    ventana.draw(nombreJ1);
    ventana.draw(nombreJ2);
    for (int i = 0; i < 4; i++) {
        dibujarFila(filasJ1[i]);
        dibujarFila(filasJ2[i]);
    }

    ventana.draw(textoTiempoLabel);
    ventana.draw(textoTiempo);
    ventana.draw(textoTurno);
    ventana.draw(botonMover);
    ventana.draw(textoMover);
    ventana.draw(botonAtacar);
    ventana.draw(textoAtacar);
    ventana.draw(botonFinTurno);
    ventana.draw(textoFinTurno);
    ventana.draw(estadoMover);
    ventana.draw(estadoAtacar);
}

bool HUD::dentroDeRect(const sf::RectangleShape& rect,
                        int mx, int my) const {
    float x = rect.getPosition().x;
    float y = rect.getPosition().y;
    float w = rect.getSize().x;
    float h = rect.getSize().y;
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

bool HUD::clickEnMover   (int mx, int my) const { return dentroDeRect(botonMover,    mx, my); }
bool HUD::clickEnAtacar  (int mx, int my) const { return dentroDeRect(botonAtacar,   mx, my); }
bool HUD::clickEnFinTurno(int mx, int my) const { return dentroDeRect(botonFinTurno, mx, my); }