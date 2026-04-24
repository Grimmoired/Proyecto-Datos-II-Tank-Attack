#include "AssetManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "estructuras/ArregloDinamico.h"

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

void AssetManager::loadAll() {
    loadAtlas();
    loadPowerUps();
    loadAudio();
}

void AssetManager::loadAtlas() {
    if (!atlasTexture.loadFromFile("assets/sprites/SpritesFull.png"))
        throw std::runtime_error("No se pudo cargar SpritesFull.png");
    if (!atlasTexture.loadFromFile("assets/sprites/SpritesFull.png"))
        throw std::runtime_error("No se pudo cargar SpritesFull.png");
    atlasTexture.setSmooth(false);
    std::ifstream file("assets/sprites/SpritesFull.xml");
    if (!file.is_open())
        throw std::runtime_error("No se pudo cargar SpritesFull.xml");

    std::string line;
    while (std::getline(file, line)) {
        std::string name;
        int x = 0, y = 0, w = 0, h = 0;
        auto extract = [&](const std::string& attr) -> std::string {
            size_t pos = line.find(attr + "=\"");
            if (pos == std::string::npos) return "";
            pos += attr.size() + 2;
            size_t end = line.find("\"", pos);
            return line.substr(pos, end - pos);
        };
        name = extract("name");
        if (name.empty()) continue;
        if (name.size() > 4 && name.substr(name.size() - 4) == ".png")
            name = name.substr(0, name.size() - 4);
        x = std::stoi(extract("x"));
        y = std::stoi(extract("y"));
        w = std::stoi(extract("width"));
        h = std::stoi(extract("height"));
        spriteRects[name] = sf::IntRect(x, y, w, h);
    }
}

void AssetManager::loadPowerUps() {
    ArregloDinamico<std::string> nombres(4);

    nombres.agregar("powerupDobleTurno");
    nombres.agregar("powerupPrecisionMovimiento");
    nombres.agregar("powerupPrecisionAtaque");
    nombres.agregar("powerupAtaque");

    for (int i = 0; i < nombres.size(); i++) {
        const std::string& nombre = nombres[i];

        sf::Texture tex;
        if (!tex.loadFromFile("assets/sprites/" + nombre + ".png"))
            throw std::runtime_error("No se pudo cargar " + nombre + ".png");

        powerUpTextures[nombre] = std::move(tex);
    }
}

void AssetManager::loadAudio() {
    // SFX
    ArregloDinamico<std::string> sfxNames(6);

    sfxNames.agregar("disparo");
    sfxNames.agregar("explosion");
    sfxNames.agregar("movimiento");
    sfxNames.agregar("rebote");
    sfxNames.agregar("powerup");
    sfxNames.agregar("victoria");

    for (int i = 0; i < sfxNames.size(); i++) {
        const std::string& nombre = sfxNames[i];

        sf::SoundBuffer buf;
        if (!buf.loadFromFile("assets/audio/sfx/" + nombre + ".ogg"))
            throw std::runtime_error("No se pudo cargar " + nombre + ".ogg");

        soundBuffers[nombre] = std::move(buf);
    }

    // Música
    ArregloDinamico<std::string> musicNames(2);

    musicNames.agregar("menu");
    musicNames.agregar("juego");

    for (int i = 0; i < musicNames.size(); i++) {
        const std::string& nombre = musicNames[i];

        sf::Music* track = new sf::Music();
        if (!track->openFromFile("assets/audio/music/" + nombre + ".ogg"))
            throw std::runtime_error("No se pudo cargar " + nombre + ".ogg");

        musicTracks[nombre] = track;
    }
}

sf::IntRect AssetManager::getSprite(const std::string& name) const {
    auto it = spriteRects.find(name);
    if (it == spriteRects.end())
        throw std::runtime_error("Sprite no encontrado: " + name);
    return it->second;
}

const sf::Texture& AssetManager::getAtlasTexture() const {
    return atlasTexture;
}

const sf::Texture& AssetManager::getPowerUpTexture(const std::string& name) const {
    auto it = powerUpTextures.find(name);
    if (it == powerUpTextures.end())
        throw std::runtime_error("Textura de power-up no encontrada: " + name);
    return it->second;
}

sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& name) {
    auto it = soundBuffers.find(name);
    if (it == soundBuffers.end())
        throw std::runtime_error("Sonido no encontrado: " + name);
    return it->second;
}

sf::Music& AssetManager::getMusic(const std::string& name) {
    auto it = musicTracks.find(name);
    if (it == musicTracks.end())
        throw std::runtime_error("Música no encontrada: " + name);
    return *it->second;
}