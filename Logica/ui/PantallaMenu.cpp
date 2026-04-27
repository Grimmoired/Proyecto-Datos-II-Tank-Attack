#include "PantallaMenu.h"
#include "../assets/AssetManager.h"

PantallaMenu::PantallaMenu(sf::RenderWindow& ventana)
    : ventana(ventana), opcionSeleccionada(0), visible(true) {
    inicializar();
    inicializarDecoracion();
}

sf::Sprite PantallaMenu::crearSprite(const std::string& nombre, float escala, float x, float y, float rotacion, float origenX, float origenY) {
    AssetManager& assets = AssetManager::getInstance();
    sf::Sprite sprite(assets.getAtlasTexture(), assets.getSprite(nombre));
    sf::IntRect rect = assets.getSprite(nombre);
    sprite.setOrigin(rect.width * origenX, rect.height * origenY);
    sprite.setScale(escala, escala);
    sprite.setRotation(rotacion);
    sprite.setPosition(x, y);
    return sprite;
}

void PantallaMenu::inicializarDecoracion() {
    const float escTanque = 3.5f;
    const float escBala   = 5.0f;
    {
        SpriteTanque st;
        st.cuerpo = crearSprite("tank_red",    escTanque, 850,  130, -90, 0.5f, 0.5f);
        st.bala   = crearSprite("bulletRed2",  escBala,   1070,  130,  90, 0.5f, 0.5f);
        decoracion.push_back(st);
    }
    {
        SpriteTanque st;
        st.cuerpo = crearSprite("tank_blue",   escTanque, 1600, 480,   0, 0.5f, 0.5f);
        st.bala   = crearSprite("bulletBlue2", escBala,   1600, 670,   180, 0.5f, 0.5f);
        decoracion.push_back(st);
    }
    {
        SpriteTanque st;
        st.cuerpo = crearSprite("tank_green",   escTanque, 1000, 920,  90, 0.5f, 0.5f);
        st.bala   = crearSprite("bulletGreen2", escBala,    800, 920, -90, 0.5f, 0.5f);
        decoracion.push_back(st);
    }
    {
        SpriteTanque st;
        st.cuerpo = crearSprite("tank_sand",   escTanque, 320, 630, 180, 0.5f, 0.5f);
        st.bala   = crearSprite("bulletSand2", escBala,   320, 440, 0, 0.5f, 0.5f);
        decoracion.push_back(st);
    }
}

void PantallaMenu::inicializar() {
    const sf::Font& fuente = AssetManager::getInstance().getFuente();
    const sf::Texture& atlas = AssetManager::getInstance().getAtlasTexture();
    sf::RenderTexture rt;
    rt.create(1920, 1080);
    rt.clear(sf::Color(210, 180, 140));
    sf::IntRect rectSuelo = AssetManager::getInstance().getSprite("tileSand1");
    for (int f = 0; f < 18; f++) {
        for (int c = 0; c < 32; c++) {
            sf::Sprite tile(atlas, rectSuelo);
            tile.setScale(60.0f / rectSuelo.width, 60.0f / rectSuelo.height);
            tile.setPosition(c * 60.0f, f * 60.0f);
            rt.draw(tile);
        }
    }

    rt.display();
    fondoTextura = rt.getTexture();
    fondoSprite.setTexture(fondoTextura);
    panelTitulo.setSize({900, 540});
    panelTitulo.setPosition(510, 260);
    panelTitulo.setFillColor(sf::Color(20, 15, 10, 225));
    panelTitulo.setOutlineColor(sf::Color(180, 140, 60));
    panelTitulo.setOutlineThickness(4);
    titulo.setFont(fuente);
    titulo.setString("TANK ATTACK!");
    titulo.setCharacterSize(96);
    titulo.setFillColor(sf::Color(255, 210, 60));
    titulo.setOutlineColor(sf::Color(120, 70, 10));
    titulo.setOutlineThickness(4);
    centrarTexto(titulo, 900, 510, 260, 130);
    instruccion.setFont(fuente);
    instruccion.setString("Flechas para navegar  |  Enter para seleccionar");
    instruccion.setCharacterSize(26);
    instruccion.setFillColor(sf::Color(180, 160, 100));
    centrarTexto(instruccion, 900, 510, 750, 40);

    panelBoton1.setSize({500, 75});
    panelBoton1.setPosition(710, 410);
    panelBoton1.setFillColor(sf::Color(60, 100, 40, 200));
    panelBoton1.setOutlineColor(sf::Color(120, 200, 80));
    panelBoton1.setOutlineThickness(3);
    opcionJugar.setFont(fuente);
    opcionJugar.setString("JUGAR");
    opcionJugar.setCharacterSize(52);
    opcionJugar.setFillColor(sf::Color(180, 255, 120));
    centrarTexto(opcionJugar, 500, 710, 410, 75);
    panelBoton3.setSize({500, 75});
    panelBoton3.setPosition(710, 510);
    panelBoton3.setFillColor(sf::Color(140, 110, 20, 200));
    panelBoton3.setOutlineColor(sf::Color(240, 200, 60));
    panelBoton3.setOutlineThickness(3);

    opcionInstrucciones.setFont(fuente);
    opcionInstrucciones.setString("INSTRUCCIONES");
    opcionInstrucciones.setCharacterSize(52);
    opcionInstrucciones.setFillColor(sf::Color(255, 220, 80));
    centrarTexto(opcionInstrucciones, 500, 710, 510, 75);
    panelBoton2.setSize({500, 75});
    panelBoton2.setPosition(710, 610);
    panelBoton2.setFillColor(sf::Color(100, 30, 30, 200));
    panelBoton2.setOutlineColor(sf::Color(200, 80, 80));
    panelBoton2.setOutlineThickness(3);
    opcionSalir.setFont(fuente);
    opcionSalir.setString("SALIR");
    opcionSalir.setCharacterSize(52);
    opcionSalir.setFillColor(sf::Color(255, 130, 130));
    centrarTexto(opcionSalir, 500, 710, 610, 75);
    opcionSeleccionada = 0;
}

void PantallaMenu::centrarTexto(sf::Text& texto, float anchoContenedor,
                                 float xContenedor, float yContenedor,
                                 float altoContenedor) {
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f,
                    bounds.top  + bounds.height / 2.f);
    texto.setPosition(xContenedor + anchoContenedor / 2.f,
                      yContenedor + altoContenedor / 2.f);
}
OpcionMenu PantallaMenu::ejecutar() {
    AssetManager::getInstance().getMusic("menu").setLoop(true);
    AssetManager::getInstance().getMusic("menu").play();
    bool corriendo = true;
    OpcionMenu resultado = OpcionMenu::Ninguna;
    while (ventana.isOpen() && corriendo) {
        procesarEventos(corriendo, resultado);
        actualizar();
        dibujar();
    }
    AssetManager::getInstance().getMusic("menu").stop();
    return resultado;
}

void PantallaMenu::procesarEventos(bool& corriendo, OpcionMenu& resultado) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
            corriendo = false;
        }
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::Up) {
                opcionSeleccionada = (opcionSeleccionada + 2) % 3;
                relojParpadeo.restart();
                visible = true;
            } else if (evento.key.code == sf::Keyboard::Down) {
                opcionSeleccionada = (opcionSeleccionada + 1) % 3;
                relojParpadeo.restart();
                visible = true;
            } else if (evento.key.code == sf::Keyboard::Return) {
                if      (opcionSeleccionada == 0) resultado = OpcionMenu::Jugar;
                else if (opcionSeleccionada == 1) resultado = OpcionMenu::Instrucciones;
                else                              resultado = OpcionMenu::Salir;
                corriendo = false;
            }
        }
    }
}

void PantallaMenu::actualizar() {
    if (relojParpadeo.getElapsedTime().asSeconds() > 0.5f) {
        visible = !visible;
        relojParpadeo.restart();
    }
    panelBoton1.setFillColor(sf::Color(60, 100, 40, 180));
    panelBoton1.setOutlineColor(sf::Color(100, 170, 60));
    opcionJugar.setFillColor(sf::Color(150, 210, 100));
    panelBoton3.setFillColor(sf::Color(140, 110, 20, 180));
    panelBoton3.setOutlineColor(sf::Color(180, 150, 40));
    opcionInstrucciones.setFillColor(sf::Color(200, 170, 60));
    panelBoton2.setFillColor(sf::Color(100, 30, 30, 180));
    panelBoton2.setOutlineColor(sf::Color(150, 60, 60));
    opcionSalir.setFillColor(sf::Color(200, 100, 100));
    sf::Color colorActivo = visible ? sf::Color(220, 255, 150) : sf::Color(140, 190, 90);

    if (opcionSeleccionada == 0) {
        panelBoton1.setFillColor(sf::Color(80, 140, 50, 230));
        panelBoton1.setOutlineColor(sf::Color(180, 255, 100));
        opcionJugar.setFillColor(colorActivo);
    } else if (opcionSeleccionada == 1) {
        panelBoton3.setFillColor(sf::Color(180, 140, 30, 230));
        panelBoton3.setOutlineColor(sf::Color(255, 220, 80));
        opcionInstrucciones.setFillColor(colorActivo);
    } else {
        panelBoton2.setFillColor(sf::Color(140, 40, 40, 230));
        panelBoton2.setOutlineColor(sf::Color(255, 100, 100));
        opcionSalir.setFillColor(colorActivo);
    }
}

void PantallaMenu::dibujar() {
    ventana.clear();
    ventana.draw(fondoSprite);
    for (auto& st : decoracion) {
        ventana.draw(st.cuerpo);
        ventana.draw(st.bala);
    }
    ventana.draw(panelTitulo);
    ventana.draw(titulo);
    ventana.draw(panelBoton1);
    ventana.draw(opcionJugar);
    ventana.draw(panelBoton3);
    ventana.draw(opcionInstrucciones);
    ventana.draw(panelBoton2);
    ventana.draw(opcionSalir);
    ventana.draw(instruccion);
    ventana.display();
}