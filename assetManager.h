#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class AssetManager {
public:
    AssetManager();

    // --- TEXTURES ---
    sf::Texture background;
    sf::Texture playerStanding, playerMove1, playerMove2;
    sf::Texture chip, rouletteChip;
    sf::Texture enemyFastRun1, enemyFastRun2, enemyFastAttack;
    sf::Texture wheel, pointer, rouletteUI;
    sf::Texture winTexture, loseTexture;

    // --- FONTS & SOUNDS ---
    sf::Font hudFont;
    sf::SoundBuffer winBuffer, loseBuffer;
};