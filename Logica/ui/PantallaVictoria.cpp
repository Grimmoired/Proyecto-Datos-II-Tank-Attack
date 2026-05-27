#include "PantallaVictoria.h"
#include "../assets/AssetManager.h"
#include <sstream>

PantallaVictoria::PantallaVictoria(sf::RenderWindow& ventana)
    : ventana(ventana) {}

sf::Text PantallaVictoria::crearTexto(const std::string& str, unsigned int tam, sf::Color color, float x, float y) {
    sf::Text t;
    t.setFont(AssetManager::getInstance().getFuente());
    t.setString(str);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition(x, y);
    return t;
}

void PantallaVictoria::ejecutar(int ganador, Tanque* tanques, int cantTanques) {
    inicializar(ganador, tanques, cantTanques);
    bool corriendo = true;
    AssetManager::getInstance().getMusic("instrucciones").setLoop(true);
    AssetManager::getInstance().getMusic("instrucciones").play();
    while (ventana.isOpen() && corriendo) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
                corriendo = false;
            }
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape){
                corriendo = false;
                AssetManager::getInstance().getMusic("instrucciones").stop();
            }
        }
        dibujar();
    }
}

void PantallaVictoria::inicializar(int ganador, Tanque* tanques, int cantTanques) {
    AssetManager& assets = AssetManager::getInstance();
    const sf::Texture& atlas = assets.getAtlasTexture();
    sf::RenderTexture rt;
    rt.create(1920, 1080);
    rt.clear(sf::Color(210, 180, 140));
    sf::IntRect rectSuelo = assets.getSprite("tileSand1");
    for (int f = 0; f < 18; f++)
        for (int c = 0; c < 32; c++) {
            sf::Sprite tile(atlas, rectSuelo);
            tile.setScale(60.f / rectSuelo.width, 60.f / rectSuelo.height);
            tile.setPosition(c * 60.f, f * 60.f);
            rt.draw(tile);
        }

    rt.display();
    fondoTextura = rt.getTexture();
    fondoSprite.setTexture(fondoTextura);
    panel.setSize({1000, 700});
    panel.setPosition(460, 140);
    panel.setFillColor(sf::Color(18, 14, 8, 230));
    panel.setOutlineColor(sf::Color(180, 140, 60));
    panel.setOutlineThickness(4);

    textTitulo = crearTexto("FIN DE LA PARTIDA!", 72,
                             sf::Color(255, 210, 60), 0, 0);
    textTitulo.setOutlineColor(sf::Color(120, 70, 10));
    textTitulo.setOutlineThickness(3);
    {
        sf::FloatRect b = textTitulo.getLocalBounds();
        textTitulo.setOrigin(b.left + b.width / 2.f, 0);
        textTitulo.setPosition(960, 160);
    }

    std::string msgGanador;
    sf::Color colorGanador;
    if (ganador == 0) {
        msgGanador   = "EMPATE";
        colorGanador = sf::Color(255, 220, 80);
    } else {
        msgGanador   = "JUGADOR " + std::to_string(ganador) + " GANA!";
        colorGanador = (ganador == 1) ? sf::Color(96, 160, 255) : sf::Color(96, 208, 64);
    }
    textGanador = crearTexto(msgGanador, 64, colorGanador, 0, 0);
    textGanador.setOutlineColor(sf::Color(30, 20, 10));
    textGanador.setOutlineThickness(3);
    {
        sf::FloatRect b = textGanador.getLocalBounds();
        textGanador.setOrigin(b.left + b.width / 2.f, 0);
        textGanador.setPosition(960, 270);
    }

    int totalDestruidos = 0;
    int totalDanioTanques = 0;
    int totalDanioObstaculos = 0;
    int totalObsDestruidos = 0;
    int totalPUsados = 0;

    for (int i = 0; i < cantTanques; i++) {
        if (ganador != 0 && tanques[i].getJugador() != ganador) continue;
        totalDestruidos += tanques[i].getTanquesDestruidos();
        totalDanioTanques += tanques[i].getDanioTotalTanques();
        totalDanioObstaculos += tanques[i].getDanioTotalObstaculos();
        totalObsDestruidos += tanques[i].getObstaculosDestruidos();
        totalPUsados += tanques[i].getPowerUpsUsados();
    }

    std::ostringstream ss;
    if (ganador != 0)
        ss << "   Estadisticas del Jugador " << ganador << ":\n\n";
    else
        ss << "   Estadisticas combinadas:\n\n";
    ss << "Tanques enemigos destruidos:  " << totalDestruidos      << "\n\n"
       << "Dano total a tanques:            " << totalDanioTanques    << "\n\n"
       << "Dano total a obstaculos:         " << totalDanioObstaculos << "\n\n"
       << "Obstaculos destruidos:          " << totalObsDestruidos   << "\n\n"
       << "Power-ups utilizados:            " << totalPUsados         << "\n\n";

    textStats = crearTexto(ss.str(), 32, sf::Color(200, 190, 160), 795, 370);
    panelVolver.setSize({340, 52});
    panelVolver.setPosition(1110, 780);
    panelVolver.setFillColor(sf::Color(60, 50, 30, 220));
    panelVolver.setOutlineColor(sf::Color(180, 140, 60));
    panelVolver.setOutlineThickness(2);
    textVolver = crearTexto("[ ESC ]  Volver al menu", 30, sf::Color(200, 170, 80), 1130, 790);
}

void PantallaVictoria::dibujar() {
    ventana.clear();
    ventana.draw(fondoSprite);
    ventana.draw(panel);
    ventana.draw(textTitulo);
    ventana.draw(textGanador);
    ventana.draw(textStats);
    ventana.draw(panelVolver);
    ventana.draw(textVolver);
    ventana.display();
}