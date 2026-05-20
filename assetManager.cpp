#include "AssetManager.h"

AssetManager::AssetManager() {
    // Initialising the background texture.
    if (!background.loadFromFile("casino_map.png")) {
        std::cerr << "Error - Texture (casino_map.png) not found!" << std::endl;
    }

    // Initialising player's standing texture.
    if (!playerStanding.loadFromFile("playerStanding.png")) {
        std::cerr << "Error - Texture (playerStanding.png) not found!" << std::endl;
    }

    // Initialising player's moving texture 1.
    if (!playerMove1.loadFromFile("playerMove1.png")) {
        std::cerr << "Error - Texture (playerMove1.png) not found!" << std::endl;
    }

    // Initialising player's moving texture 2.
    if (!playerMove2.loadFromFile("playerMove2.png")) {
        std::cerr << "Error - Texture (playerMove2.png) not found!" << std::endl;
    }

    // Initialising chip texture ( projectile)
    if (!chip.loadFromFile("poker_chip.png")) {
        std::cerr << "Error - Texture (poker_chip.png) not found!" << std::endl;
    }

    if (!rouletteChip.loadFromFile("roulette_chip.png")) {
        std::cerr << "Error - Texture (roulette_chip.png) not found!" << std::endl;
    }

    // Initialising fast enemy textures
    if (!enemyFastRun1.loadFromFile("enemy_fast1.png") ||
        !enemyFastRun2.loadFromFile("enemy_fast2.png") ||
        !enemyFastAttack.loadFromFile("enemy_fast_attack.png")) {
        std::cerr << "Error - Texturs (enemy_fast1.png, or enemy_fast2.png or enemy_fast_attack.png) not found!" << std::endl;
    }

    // Initialising roulette textures.
    if (!wheel.loadFromFile("roulette_wheel.png") ||
        !pointer.loadFromFile("win_pointer.png") ||
        !rouletteUI.loadFromFile("roulette_UI.png")) {
        std::cerr << "Error - Roulette textures (roulette_wheel.png, or win_pointer.png, or roulette_UI.png) NOT FOUND!" << std::endl;
    }

    // Initialising gambling result textures
    if (!winTexture.loadFromFile("you_win.png") ||
        !loseTexture.loadFromFile("you_lose.png")) {
        std::cerr << "Error - Textures (you_win.png or you_lose.png) not found!" << std::endl;
    }

    // Initialising gambling result sounds
    if (!winBuffer.loadFromFile("win_sound.mp3") ||
        !loseBuffer.loadFromFile("lose_sound.mp3")) {
        std::cerr << "Error - sounds (win_sound.mp3/lose_sound.mp3) not found!" << std::endl;
    }

    // Loading the font for the HUD
    if (!hudFont.openFromFile("retro_font.ttf")) {
        std::cerr << "Blad: Nie znaleziono pliku retro_font.ttf!" << std::endl;
    }
}