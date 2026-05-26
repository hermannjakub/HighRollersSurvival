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
    sf::Texture enemyFastRun1, enemyFastRun2, enemyFastAttack1, enemyFastAttack2;
    sf::Texture enemyBossRun1, enemyBossRun2, enemyBossAttack1, enemyBossAttack2;
    sf::Texture enemyTankRun1, enemyTankRun2, enemyTankAttack1, enemyTankAttack2;
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