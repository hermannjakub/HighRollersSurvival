#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class AssetManager {
public:
    AssetManager();

    // --- TEXTURES ---
    // Basic elements
    sf::Texture background, gameOver;
    // Player
    sf::Texture playerStanding, playerMove1, playerMove2;
    // Projectiles
    sf::Texture chip;
    // Enemies
    sf::Texture enemyFastRun1, enemyFastRun2, enemyFastAttack;
    // Roulette
    sf::Texture wheel, pointer, rouletteUI, rouletteChip;
    sf::Texture winTexture, loseTexture;
    // Cardgame
    sf::Texture card10, cardJ, cardQ, cardK, cardA, cardHidden;
    sf::Texture cardGameUI;
    // Shop
    sf::Texture shopUI;

    // --- FONTS & SOUNDS ---
    sf::Font hudFont;
    sf::SoundBuffer winBuffer, loseBuffer;
    sf::Sound winSound, loseSound;
};