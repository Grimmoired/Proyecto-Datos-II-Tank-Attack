    #include "GameManager.h"
    #include "../assets/AssetManager.h"
    #include <sstream>

    GameManager::GameManager(sf::RenderWindow& ventana): ventana(ventana), mapa(nullptr), esDesierto(false), gestor(), turnoEnRonda(0), bala(nullptr), balaActiva(false), spawnPU(), renderer(nullptr), hud(nullptr), tiempoRestante(300.0f), salirAlMenu(false) {
        inicializarColoresHUD();
    }

    GameManager::~GameManager() {
        delete mapa;
        delete renderer;
        delete hud;
        if (bala != nullptr) {
            delete bala;
            bala = nullptr;
        }
        AssetManager::getInstance().getMusic("juego").stop();
    }

    void GameManager::inicializarColoresHUD() {
        etiquetasJ1[0] = "AZL1"; etiquetasJ1[1] = "AZL2";
        etiquetasJ1[2] = "ROJ1"; etiquetasJ1[3] = "ROJ2";
        etiquetasJ2[0] = "VRD1"; etiquetasJ2[1] = "VRD2";
        etiquetasJ2[2] = "AMR1"; etiquetasJ2[3] = "AMR2";

        barrasJ1[0] = barrasJ1[1] = sf::Color(48, 112, 192);
        barrasJ1[2] = barrasJ1[3] = sf::Color(192, 48,  48);
        textosJ1[0] = textosJ1[1] = sf::Color(160, 192, 255);
        textosJ1[2] = textosJ1[3] = sf::Color(255, 144, 144);

        barrasJ2[0] = barrasJ2[1] = sf::Color(48,  160, 48);
        barrasJ2[2] = barrasJ2[3] = sf::Color(176, 144, 32);
        textosJ2[0] = textosJ2[1] = sf::Color(144, 255, 144);
        textosJ2[2] = textosJ2[3] = sf::Color(255, 224, 128);

        datosJ1.nombre      = "JUGADOR 1";
        datosJ1.colorNombre = sf::Color(96, 144, 208);
        datosJ2.nombre      = "JUGADOR 2";
        datosJ2.colorNombre = sf::Color(96, 192, 80);
    }

    void GameManager::inicializarMapa() {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> moneda(0, 1);
        esDesierto = moneda(rng) == 1;
        mapa = new Mapa(filasMapa, columnasMapa, (float)tamanioCasilla, esDesierto);
        mapa->generar();
    }

    bool GameManager::spawnValido(int f, int c) const {
        if (f < 0 || f >= filasMapa || c < 0 || c >= columnasMapa)
            return false;
        if (mapa->getCasilla(f, c).getTipo() != TipoCasilla::Suelo)
            return false;
        if (mapa->getCasilla(f, c).estaOcupada())
            return false;
        const int df[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
            int nf = f + df[d];
            int nc = c + dc[d];
            if (nf < 0 || nf >= filasMapa ||
                nc < 0 || nc >= columnasMapa) continue;
            if (mapa->getCasilla(nf, nc).getTipo() != TipoCasilla::Suelo)
                return false;
            if (mapa->getCasilla(nf, nc).estaOcupada())
                return false;
        }
        return true;
    }

    std::pair<int,int> GameManager::buscarSpawn(int fBase, int cBase, int dirF, int dirC) const {
        for (int dist = 0; dist < 10; dist++) {
            int f = fBase + dirF * dist;
            int c = cBase + dirC * dist;
            if (f < 0) f = 0;
            if (c < 0) c = 0;
            if (f >= filasMapa)    f = filasMapa - 1;
            if (c >= columnasMapa) c = columnasMapa - 1;
            if (spawnValido(f, c)) return {f, c};
            for (int lat = 1; lat <= 4; lat++) {
                if (spawnValido(f, c + lat)) return {f, c + lat};
                if (spawnValido(f, c - lat)) return {f, c - lat};
                if (spawnValido(f + lat, c)) return {f + lat, c};
                if (spawnValido(f - lat, c)) return {f - lat, c};
            }
        }
        for (int fi = fBase; fi >= 0 && fi < filasMapa; fi += dirF)
            for (int ci = cBase; ci >= 0 && ci < columnasMapa; ci += dirC)
                if (spawnValido(fi, ci)) return {fi, ci};
        return {fBase, cBase};
    }

    void GameManager::inicializarTanques() {
        auto [f0, c0] = buscarSpawn(0, 0, 1, 1);
        mapa->getCasilla(f0, c0).setOcupada(true);
        auto [f1, c1] = buscarSpawn(0, c0 + 6, 1, 1);
        mapa->getCasilla(f1, c1).setOcupada(true);
        auto [f2, c2] = buscarSpawn(f0 + 6, 0, 1, 1);
        mapa->getCasilla(f2, c2).setOcupada(true);
        auto [f3, c3] = buscarSpawn(f0 + 6, c0 + 6, 1, 1);
        mapa->getCasilla(f3, c3).setOcupada(true);

        auto [f4, c4] = buscarSpawn(filasMapa - 1, columnasMapa - 1, -1, -1);
        mapa->getCasilla(f4, c4).setOcupada(true);
        auto [f5, c5] = buscarSpawn(filasMapa - 1, c4 - 6, -1, -1);
        mapa->getCasilla(f5, c5).setOcupada(true);
        auto [f6, c6] = buscarSpawn(f4 - 6, columnasMapa - 1, -1, -1);
        mapa->getCasilla(f6, c6).setOcupada(true);
        auto [f7, c7] = buscarSpawn(f4 - 6, c4 - 6, -1, -1);
        mapa->getCasilla(f7, c7).setOcupada(true);

        // Asigna directamente al miembro de clase, no a un arreglo local
        tanques[0] = Tanque(f0, c0, ColorTanque::Azul,     1, 0);
        tanques[1] = Tanque(f1, c1, ColorTanque::Azul,     1, 1);
        tanques[2] = Tanque(f2, c2, ColorTanque::Rojo,     1, 2);
        tanques[3] = Tanque(f3, c3, ColorTanque::Rojo,     1, 3);
        tanques[4] = Tanque(f4, c4, ColorTanque::Verde,    2, 0);
        tanques[5] = Tanque(f5, c5, ColorTanque::Verde,    2, 1);
        tanques[6] = Tanque(f6, c6, ColorTanque::Amarillo, 2, 2);
        tanques[7] = Tanque(f7, c7, ColorTanque::Amarillo, 2, 3);

        AssetManager& assets     = AssetManager::getInstance();
        const sf::Texture& atlas = assets.getAtlasTexture();

        for (int i = 0; i < cantTanques; i++) {
            sf::IntRect rect = assets.getSprite(
                nombreSpritePorColor(tanques[i].getColor()));
            tanques[i].inicializarSprite(atlas, rect, (float)tamanioCasilla);
            mapa->getCasilla(tanques[i].getFila(),
                              tanques[i].getColumna()).setTanque(&tanques[i]);
        }
    }

    void GameManager::inicializarHUD() {
        renderer = new Renderer(ventana);
        hud = new HUD(ventana);
    }

    const char* GameManager::nombreSpritePorColor(ColorTanque color) {
        switch (color) {
            case ColorTanque::Azul:     return "tank_blue";
            case ColorTanque::Rojo:     return "tank_red";
            case ColorTanque::Verde:    return "tank_green";
            case ColorTanque::Amarillo: return "tank_sand";
        }
        return "tank_blue"; // Color por defecto
    }

    bool GameManager::ejecutar() {
        inicializarMapa();
        inicializarTanques();
        inicializarHUD();
        spawnPU.inicializar();
        gestor.inicializar(tanques, cantTanques);
        AssetManager::getInstance().getMusic("juego").setLoop(true);
        AssetManager::getInstance().getMusic("juego").play();
        tiempoRestante = 300.0f;
        turnoEnRonda   = 0;
        salirAlMenu    = true;

        while (ventana.isOpen()) {
            float tiempoTotal  = relojTotal.getElapsedTime().asSeconds();
            dtActual           = reloj.restart().asSeconds(); // guarda el dt
            tiempoRestante    -= dtActual;
            procesarEventos();
            if (!ventana.isOpen()) break;
            if (tiempoRestante <= 0.f) {
                // Aun falta implementar la pantalla de victoria
                break;
            }
            actualizarDatosHUD();
            renderizar(tiempoTotal);
        }
        return salirAlMenu;
    }

    void GameManager::procesarEventos() {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                salirAlMenu = false;
                ventana.close();
                return;
            }
            if (evento.type == sf::Event::MouseButtonPressed)
                procesarClickHUD(evento.mouseButton.x, evento.mouseButton.y);
            if (evento.type == sf::Event::KeyPressed)
                procesarTeclado(evento.key.code);
        }
    }

    void GameManager::procesarClickHUD(int mx, int my) {
        if (hud->clickEnMover(mx, my) && gestor.puedeMover()) {
            gestor.setEstado(EstadoJuego::modoMover);
            return;
        }
        if (hud->clickEnAtacar(mx, my) && gestor.puedeAtacar()) {
            gestor.setEstado(EstadoJuego::modoAtacar);
            return;
        }
        if (hud->clickEnFinTurno(mx, my)) {
            avanzarTurno();
            return;
        }
        procesarClickMapa(mx, my);
    }

    void GameManager::procesarClickMapa(int mx, int my) {
        if (gestor.getEstado() != EstadoJuego::modoAtacar) return;
        if (balaActiva) return;
        if (my >= altoVentana - altoHUD) return;
        Tanque* activo = gestor.getTanqueActual();
        float pos[2] = {
            activo->getColumna() * (float)tamanioCasilla + tamanioCasilla / 2.f,
            activo->getFila()    * (float)tamanioCasilla + tamanioCasilla / 2.f
        };

        bala = new bullet(pos, 5.0f, sf::Color::Blue, 10, activo);
        bala->aim(ventana);
        balaActiva = true;
        gestor.usarAtaque();
    }

    void GameManager::procesarTeclado(sf::Keyboard::Key tecla) {
        switch (tecla) {
            case sf::Keyboard::Up:
            case sf::Keyboard::Down:
            case sf::Keyboard::Left:
            case sf::Keyboard::Right:
                // Aun falta el movimiento del tanque
                break;
            case sf::Keyboard::LShift:
            case sf::Keyboard::RShift:
                gestor.powerUpUsado();
                break;
            case sf::Keyboard::M:
                if (gestor.puedeMover())
                    gestor.setEstado(EstadoJuego::modoMover);
                break;
            case sf::Keyboard::A:
                if (gestor.puedeAtacar())
                    gestor.setEstado(EstadoJuego::modoAtacar);
                break;
            case sf::Keyboard::Return:
                avanzarTurno();
                break;
            case sf::Keyboard::Escape:
                salirAlMenu = true;
                ventana.close();
                break;
            default: break;
        }
    }

    void GameManager::avanzarTurno() {
        gestor.siguienteTurno();
        PowerUp* puPendiente = gestor.aplicarPowerUpPendiente();
        if (puPendiente != nullptr) {
            switch (puPendiente->tipo) {  // Pendiente implementar Power Ups
                case TipoPowerUp::DobleTurno:
                    break;
                case TipoPowerUp::PrecisionMovimiento:
                    break;
                case TipoPowerUp::PrecisionAtaque:
                    break;
                case TipoPowerUp::PoderAtaque:
                    break;
            }
        }

        turnoEnRonda++;
        if (turnoEnRonda >= turnosPorRonda) {
            turnoEnRonda = 0;
            spawnPU.intentarSpawn(*mapa, tanques, cantTanques);
        }
        Tanque* activo = gestor.getTanqueActual();
        PowerUpEnMapa* puRecogido = spawnPU.recoger(
            activo->getFila(), activo->getColumna());
        if (puRecogido != nullptr)
            activo->agregarPowerUp(puRecogido->tipo);
    }


    void GameManager::actualizarDatosHUD() {
        for (int i = 0; i < 4; i++) {
            TipoPowerUp tiposJ1[3], tiposJ2[3];
            tanques[i].getTiposPowerUps(tiposJ1, 3);
            tanques[i + 4].getTiposPowerUps(tiposJ2, 3);
            datosJ1.tanques[i] = {
                etiquetasJ1[i], barrasJ1[i], textosJ1[i],
                tanques[i].getVidaPorcentaje(),
                tanques[i].getCantidadPowerUps(),
                tanques[i].estaVivo()
            };

            datosJ1.tanques[i].tiposPU[0] = tiposJ1[0];
            datosJ1.tanques[i].tiposPU[1] = tiposJ1[1];
            datosJ1.tanques[i].tiposPU[2] = tiposJ1[2];
            datosJ2.tanques[i] = {
                etiquetasJ2[i], barrasJ2[i], textosJ2[i],
                tanques[i + 4].getVidaPorcentaje(),
                tanques[i + 4].getCantidadPowerUps(),
                tanques[i + 4].estaVivo()
            };

            datosJ2.tanques[i].tiposPU[0] = tiposJ2[0];
            datosJ2.tanques[i].tiposPU[1] = tiposJ2[1];
            datosJ2.tanques[i].tiposPU[2] = tiposJ2[2];
        }
        hud->actualizar(datosJ1, datosJ2, tiempoRestante, gestor.getJugadorActual(), gestor.getEstado());
    }

    void GameManager::renderizar(float tiempoTotal) {
        Tanque* activo = gestor.getTanqueActual();
        renderer->limpiar();
        renderer->dibujarMapa(*mapa);
        spawnPU.dibujar(ventana, (float)tamanioCasilla);
        for (int i = 0; i < cantTanques; i++) {
            if (&tanques[i] == activo) tanques[i].dibujarResaltado(ventana, (float)tamanioCasilla, tiempoTotal);
            else
                tanques[i].dibujar(ventana);
        }

        if (balaActiva) {
            bala->update_coords(dtActual * 400.f);
            bala->collition_check(*mapa);
            bala->shape.setPosition({bala->pos[0], bala->pos[1]});
            renderer->dibujarBala(bala->shape);

            if (!bala->isalive) {
                delete bala;
                bala = nullptr;
                balaActiva = false;
            }
        }

        hud->dibujar();
        renderer->mostrar();
    }