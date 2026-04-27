#include "PantallaInstrucciones.h"
#include "../assets/AssetManager.h"

PantallaInstrucciones::PantallaInstrucciones(sf::RenderWindow& ventana)
    : ventana(ventana) {
    inicializar();
    AssetManager::getInstance().getMusic("instrucciones").setLoop(true);
    AssetManager::getInstance().getMusic("instrucciones").play();
}

sf::Text PantallaInstrucciones::crearTexto(const std::string& str, unsigned int tam, sf::Color color, float x, float y) {
    sf::Text t;
    t.setFont(AssetManager::getInstance().getFuente());
    t.setString(str);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition(x, y);
    return t;
}

sf::RectangleShape PantallaInstrucciones::crearCajaTecla(float x, float y, float ancho, float alto) {
    sf::RectangleShape caja;
    caja.setSize({ancho, alto});
    caja.setPosition(x, y);
    caja.setFillColor(sf::Color(50, 45, 35, 230));
    caja.setOutlineColor(sf::Color(200, 170, 80));
    caja.setOutlineThickness(2);
    return caja;
}

sf::Text PantallaInstrucciones::crearLabelTecla(const std::string& label, sf::RectangleShape& caja) {
    sf::Text t = crearTexto(label, 22, sf::Color(240, 220, 140), 0, 0);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(
        caja.getPosition().x + caja.getSize().x / 2.f,
        caja.getPosition().y + caja.getSize().y / 2.f
    );
    return t;
}

sf::Sprite PantallaInstrucciones::crearSpritePU(const std::string& nombre, float x, float y, float altoDeseado) {
    const sf::Texture& tex = AssetManager::getInstance().getPowerUpTexture(nombre);
    float escala = altoDeseado / (float)tex.getSize().y;
    sf::Sprite sp(tex);
    sp.setScale(escala, escala);
    sp.setPosition(x, y);
    return sp;
}

void PantallaInstrucciones::inicializar() {
    AssetManager& assets = AssetManager::getInstance();
    const sf::Texture& atlas = assets.getAtlasTexture();
    sf::RenderTexture rt;
    rt.create(1920, 1080);
    rt.clear(sf::Color(210, 180, 140));
    sf::IntRect rectSuelo = assets.getSprite("tileSand1");
    for (int f = 0; f < 18; f++)
        for (int c = 0; c < 32; c++) {
            sf::Sprite tile(atlas, rectSuelo);
            tile.setScale(60.0f / rectSuelo.width, 60.0f / rectSuelo.height);
            tile.setPosition(c * 60.0f, f * 60.0f);
            rt.draw(tile);
        }

    rt.display();
    fondoTextura = rt.getTexture();
    fondoSprite.setTexture(fondoTextura);
    panel.setSize({1700, 940});
    panel.setPosition(110, 60);
    panel.setFillColor(sf::Color(18, 14, 8, 225));
    panel.setOutlineColor(sf::Color(180, 140, 60));
    panel.setOutlineThickness(4);
    textTitulo = crearTexto("INSTRUCCIONES", 72, sf::Color(255, 210, 60), 0, 0);
    textTitulo.setOutlineColor(sf::Color(120, 70, 10));
    textTitulo.setOutlineThickness(3);
    {
        sf::FloatRect b = textTitulo.getLocalBounds();
        textTitulo.setOrigin(b.left + b.width / 2.f, 0);
        textTitulo.setPosition(960, 80);
    }

    textSecControles = crearTexto("CONTROLES", 38, sf::Color(180, 220, 255), 330, 175);
    cajaFlechaArriba  = crearCajaTecla(198, 228, 42, 42);
    labelFlechaArriba = crearLabelTecla("^", cajaFlechaArriba);
    cajaFlechaIzq  = crearCajaTecla(155, 276, 42, 42);
    labelFlechaIzq = crearLabelTecla("<", cajaFlechaIzq);
    cajaFlechaAbajo  = crearCajaTecla(198, 276, 42, 42);
    labelFlechaAbajo = crearLabelTecla("v", cajaFlechaAbajo);
    cajaFlechaDer  = crearCajaTecla(246, 276, 42, 42);
    labelFlechaDer = crearLabelTecla(">", cajaFlechaDer);
    textMover = crearTexto("Navegar por el mapa y seleccionar\ncasilla de movimiento", 26, sf::Color(210, 200, 170), 335, 242);
    cajaLMB  = crearCajaTecla(150, 350, 72, 42);
    labelLMB = crearLabelTecla("LMB", cajaLMB);
    cajaRMB  = crearCajaTecla(230, 350, 72, 42);
    labelRMB = crearLabelTecla("RMB", cajaRMB);
    textDisparar = crearTexto( "Confirmar movimiento (LMB)\nconfirmar disparo (RMB)", 26, sf::Color(210, 200, 170), 335, 338);
    cajaShift  = crearCajaTecla(160, 430, 120, 42);
    labelShift = crearLabelTecla("SHIFT", cajaShift);
    textPowerUp = crearTexto("Usar primer power up en la cola\n en el tanque activo", 26, sf::Color(210, 200, 170), 335, 420);
    textSecReglas = crearTexto("REGLAS", 38, sf::Color(180, 255, 180), 330, 490);

    textReglas = crearTexto(
        "Cada jugador controla 4 tanques de 4 colores posibles:\n"
        "- Los tanques Azules y Verdes tienen 50% de probabilidad\n"
        "de moverse mediante Djikstra o movimiento aleatorio\n"
        "- Los tanques rojos y amarillos tienen 80% de probabilidad\n"
        "de moverse mediante djikstra y 20% con movimiento aleatorio\n\n"
        "En cada turno el jugador puede realizar las siguientes\n"
        "acciones una unica vez, despues tendra que ceder turno:\n"
        "- Mover un tanque\n"
        "- Atacar con un tanque\n"
        "- Usar un power-up\n\n"
        "Gana quien destruya todos los tanques rivales, o quien\n"
        "haya destruido mas al pasar 5 minutos del contador",
        26, sf::Color(200, 190, 160), 150, 542);
    textSecPowerUps = crearTexto("POWER-UPS", 38, sf::Color(255, 200, 80), 1280, 155);
    spritePU1 = crearSpritePU("powerupDobleTurno", 1020, 250, 60);
    descPU1   = crearTexto("Doble turno:\nEl tanque recibe un segundo turno inmediatamente despues\nde ceder turno, recargando movimiento y ataque", 26, sf::Color(220, 205, 160), 1110, 235);
    spritePU2 = crearSpritePU("powerupPrecisionMovimiento", 1020, 400, 60);
    descPU2   = crearTexto("Precision de movimiento:\nAumenta la probabilidad de moverse usando BFS o Dijkstra\nen vez de movimiento aleatorio al 90%.", 26, sf::Color(220, 205, 160), 1110, 385);
    spritePU3 = crearSpritePU("powerupPrecisionAtaque",1020, 550, 60);
    descPU3   = crearTexto("Precision de ataque:\nLa siguiente bala disparada no requiere apuntar, en su lugar ira\nteledirigida hacia el objetivo seleccionado usando pathfinding A*.", 26, sf::Color(220, 205, 160), 1120, 535);
    spritePU4 = crearSpritePU("powerupAtaque", 1020, 700, 60);
    descPU4   = crearTexto("Poder de ataque: \nLa siguiente bala disparada inflige el doble de dano al impactar\nal objetivo, tambien afecta a los obstaculos rompibles.", 26, sf::Color(220, 205, 160), 1110, 685);
    textCreditos = crearTexto( " Juego creado por Bryan David Abarca Quiros y Jorge Pablo Porras Alvarado.\n"
        "Segundo proyecto de Algoritmos y Estructuras de Datos II, I Semestre 2026.", 30, sf::Color(160, 150, 120), 0, 0);
    {
        sf::FloatRect b = textCreditos.getLocalBounds();
        textCreditos.setOrigin(b.left + b.width / 2.f, 0);
        textCreditos.setPosition(1350, 840);
    }
    panelVolver.setSize({180, 40});
    panelVolver.setPosition(1590, 940);
    panelVolver.setFillColor(sf::Color(60, 50, 30, 220));
    panelVolver.setOutlineColor(sf::Color(180, 140, 60));
    panelVolver.setOutlineThickness(2);
    textVolver = crearTexto("[ ESC ] Volver", 28, sf::Color(200, 170, 80), 1613, 940);
}

void PantallaInstrucciones::ejecutar() {
    bool corriendo = true;
    while (ventana.isOpen() && corriendo) {
        procesarEventos(corriendo);
        dibujar();
    }
    AssetManager::getInstance().getMusic("instrucciones").stop();
}

void PantallaInstrucciones::procesarEventos(bool& corriendo) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
            corriendo = false;

        }
        if (evento.type == sf::Event::KeyPressed &&
            evento.key.code == sf::Keyboard::Escape)
            corriendo = false;
    }
}

void PantallaInstrucciones::dibujar() {
    ventana.clear();
    ventana.draw(fondoSprite);
    ventana.draw(panel);
    ventana.draw(textTitulo);
    ventana.draw(textSecControles);
    ventana.draw(cajaFlechaArriba);
    ventana.draw(labelFlechaArriba);
    ventana.draw(cajaFlechaIzq);
    ventana.draw(labelFlechaIzq);
    ventana.draw(cajaFlechaAbajo);
    ventana.draw(labelFlechaAbajo);
    ventana.draw(cajaFlechaDer);
    ventana.draw(labelFlechaDer);
    ventana.draw(textMover);
    ventana.draw(cajaLMB);
    ventana.draw(labelLMB);
    ventana.draw(cajaRMB);
    ventana.draw(labelRMB);
    ventana.draw(textDisparar);
    ventana.draw(cajaShift);
    ventana.draw(labelShift);
    ventana.draw(textPowerUp);
    ventana.draw(textSecReglas);
    ventana.draw(textReglas);
    ventana.draw(textSecPowerUps);
    ventana.draw(spritePU1);
    ventana.draw(descPU1);
    ventana.draw(spritePU2);
    ventana.draw(descPU2);
    ventana.draw(spritePU3);
    ventana.draw(descPU3);
    ventana.draw(spritePU4);
    ventana.draw(descPU4);
    ventana.draw(textCreditos);
    ventana.draw(panelVolver);
    ventana.draw(textVolver);
    ventana.display();
}