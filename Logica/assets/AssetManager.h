#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AssetManager {
public:
    static AssetManager& getInstance();

    void loadAll();

    sf::IntRect        getSprite(const std::string& name) const;
    const sf::Texture& getAtlasTexture() const;
    const sf::Texture& getPowerUpTexture(const std::string& name) const;
    sf::SoundBuffer&   getSoundBuffer(const std::string& name);
    sf::Music&         getMusic(const std::string& name);

private:
    AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    void loadAtlas();
    void loadPowerUps();
    void loadAudio();

    sf::Texture                          atlasTexture;
    std::map<std::string, sf::IntRect>   spriteRects;
    std::map<std::string, sf::Texture>   powerUpTextures;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Music*>    musicTracks;
};