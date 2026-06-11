#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class AssetManager {
public:
    AssetManager();

    // --- TEXTURES ---
    // Basic elements
    sf::Texture background, gameOver , menuBackground, settingsBackground;
    // Player
    sf::Texture playerStanding, playerMove1, playerMove2;
    // Projectiles
    sf::Texture chip,card;
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

    // --- FONTS, SOUNDS & MUSIC ---
    sf::Font hudFont;
    sf::SoundBuffer winBuffer, loseBuffer, enemyDamagedBuffer, enemyDamagedBuffer2, enemyDamagedBuffer3, playerDamagedBuffer, shopSoundBuffer, clickSoundBuffer;
    sf::SoundBuffer shootBuffer1;
    sf::SoundBuffer shootBuffer2;
    sf::Sound winSound, loseSound, enemyDamaged, enemyDamaged2,enemyDamaged3, playerDamaged, shopSound, clickSound;
    sf::Sound shootSound1;
    sf::Sound shootSound2;

    sf::Music menuMusic;
    sf::Music hubMusic;
    sf::Music minigameMusic;
    sf::Music survivalMusic;
};