#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class AssetManager {
public:
    AssetManager();

    // ==========================================
    // 1. TEXTURES
    // ==========================================

    // Environment, UI & Menus
    sf::Texture background;
    sf::Texture gameOver;
    sf::Texture menuBackground;
    sf::Texture settingsBackground;
    sf::Texture shopUI;

    // Player
    sf::Texture playerStanding;
    sf::Texture playerMove1;
    sf::Texture playerMove2;

    // Projectiles
    sf::Texture chip;
    sf::Texture card;

    // Fast Enemy
    sf::Texture enemyFastRun1;
    sf::Texture enemyFastRun2;
    sf::Texture enemyFastAttack1;
    sf::Texture enemyFastAttack2;

    // Tank Enemy
    sf::Texture enemyTankRun1;
    sf::Texture enemyTankRun2;
    sf::Texture enemyTankAttack1;
    sf::Texture enemyTankAttack2;

    // Boss Enemy
    sf::Texture enemyBossRun1;
    sf::Texture enemyBossRun2;
    sf::Texture enemyBossAttack1;
    sf::Texture enemyBossAttack2;

    // Minigame - Roulette
    sf::Texture wheel;
    sf::Texture pointer;
    sf::Texture rouletteUI;
    sf::Texture rouletteChip;

    // Minigame - Card Game
    sf::Texture card10;
    sf::Texture cardJ;
    sf::Texture cardQ;
    sf::Texture cardK;
    sf::Texture cardA;
    sf::Texture cardHidden;
    sf::Texture cardGameUI;

    // Minigame - Results
    sf::Texture winTexture;
    sf::Texture loseTexture;

    // ==========================================
    // 2. FONTS
    // ==========================================
    sf::Font hudFont;

    // ==========================================
    // 3. SOUND BUFFERS (SFX DATA)
    // ==========================================
    sf::SoundBuffer winBuffer;
    sf::SoundBuffer loseBuffer;
    sf::SoundBuffer playerDamagedBuffer;
    sf::SoundBuffer shopSoundBuffer;
    sf::SoundBuffer clickSoundBuffer;
    sf::SoundBuffer wrongButtonBuffer;

    // Enemy hit variants
    sf::SoundBuffer enemyDamagedBuffer;
    sf::SoundBuffer enemyDamagedBuffer2;
    sf::SoundBuffer enemyDamagedBuffer3;

    // Weapon shoots
    sf::SoundBuffer shootBuffer1;
    sf::SoundBuffer shootBuffer2;

    // ==========================================
    // 4. SOUND OBJECTS (SFX PLAYERS)
    // ==========================================
    sf::Sound winSound;
    sf::Sound loseSound;
    sf::Sound playerDamaged;
    sf::Sound shopSound;
    sf::Sound clickSound;
    sf::Sound wrongButton;

    // Enemy hit variants
    sf::Sound enemyDamaged;
    sf::Sound enemyDamaged2;
    sf::Sound enemyDamaged3;

    // Weapon shoots
    sf::Sound shootSound1;
    sf::Sound shootSound2;

    // ==========================================
    // 5. MUSIC (STREAMED BACKGROUND AUDIO)
    // ==========================================
    sf::Music menuMusic;
    sf::Music hubMusic;
    sf::Music minigameMusic;
    sf::Music survivalMusic;
};