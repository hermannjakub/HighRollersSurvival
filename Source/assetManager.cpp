#include "assetManager.h"
#include <iostream>

AssetManager::AssetManager()
    // Initializing sound objects with their respective buffers in the initializer list
    : winSound(winBuffer),
      loseSound(loseBuffer),
      enemyDamaged(enemyDamagedBuffer),
      playerDamaged(playerDamagedBuffer),
      enemyDamaged2(enemyDamagedBuffer2),
      enemyDamaged3(enemyDamagedBuffer3),
      shopSound(shopSoundBuffer),
      clickSound(clickSoundBuffer),
      shootSound1(shootBuffer1),
      shootSound2(shootBuffer2),
      wrongButton(wrongButtonBuffer)
{
    // ==========================================
    // 1. LOADING TEXTURES (ENVIRONMENT AND MENUS)
    // ==========================================

    // Backgrounds and game over screens
    if (!background.loadFromFile("Assets/casino_map.png") ||
        !gameOver.loadFromFile("Assets/game_over.png")) {
        std::cerr << "Error - Background textures (casino_map.png / game_over.png) not found!" << std::endl;
    }

    // Main menu and settings backgrounds
    if (!menuBackground.loadFromFile("Assets/main_menu_bg.png") ||
        !settingsBackground.loadFromFile("Assets/settings_menu_bg.png")) {
        std::cerr << "Error - Menu backgrounds (main_menu_bg.png / settings_menu_bg.png) not found!" << std::endl;
    }

    // Shop UI
    if (!shopUI.loadFromFile("Assets/shop_UI.png")) {
        std::cerr << "Error - Shop UI (shop_UI.png) not found!" << std::endl;
    }

    // ==========================================
    // 2. LOADING TEXTURES (CHARACTERS AND OBJECTS)
    // ==========================================

    // Player textures
    if (!playerStanding.loadFromFile("Assets/playerStanding.png") ||
        !playerMove1.loadFromFile("Assets/playerMove1.png") ||
        !playerMove2.loadFromFile("Assets/playerMove2.png")) {
        std::cerr << "Error - Player textures not found!" << std::endl;
    }

    // Projectile textures
    if (!chip.loadFromFile("Assets/poker_chip.png") ||
        !card.loadFromFile("Assets/poker_card.png")) {
        std::cerr << "Error - Projectile textures (poker_chip.png / poker_card.png) not found!" << std::endl;
    }

    // ==========================================
    // 3. LOADING TEXTURES (ENEMIES)
    // ==========================================

    // Fast enemy
    if (!enemyFastRun1.loadFromFile("Assets/enemy_fast1.png") ||
        !enemyFastRun2.loadFromFile("Assets/enemy_fast2.png") ||
        !enemyFastAttack1.loadFromFile("Assets/enemy_fast_attack_1.png") ||
        !enemyFastAttack2.loadFromFile("Assets/enemy_fast_attack_2.png")) {
        std::cerr << "Error - Fast enemy textures not found!" << std::endl;
    }

    // Tank enemy
    if (!enemyTankRun1.loadFromFile("Assets/enemy_tank1.png") ||
        !enemyTankRun2.loadFromFile("Assets/enemy_tank2.png") ||
        !enemyTankAttack1.loadFromFile("Assets/enemy_tank_attack_1.png") ||
        !enemyTankAttack2.loadFromFile("Assets/enemy_tank_attack_2.png")) {
        std::cerr << "Error - Tank enemy textures not found!" << std::endl;
    }

    // Boss enemy
    if (!enemyBossRun1.loadFromFile("Assets/enemy_boss1.png") ||
        !enemyBossRun2.loadFromFile("Assets/enemy_boss2.png") ||
        !enemyBossAttack1.loadFromFile("Assets/enemy_boss_attack_1.png") ||
        !enemyBossAttack2.loadFromFile("Assets/enemy_boss_attack_2.png")) {
        std::cerr << "Error - Boss enemy textures not found!" << std::endl;
    }

    // ==========================================
    // 4. LOADING TEXTURES (MINIGAMES)
    // ==========================================

    // Roulette
    if (!wheel.loadFromFile("Assets/roulette_wheel.png") ||
        !pointer.loadFromFile("Assets/win_pointer.png") ||
        !rouletteUI.loadFromFile("Assets/roulette_UI.png") ||
        !rouletteChip.loadFromFile("Assets/roulette_chip.png")) {
        std::cerr << "Error - Roulette textures not found!" << std::endl;
    }

    // Cards
    if (!card10.loadFromFile("Assets/card_10.png") ||
        !cardJ.loadFromFile("Assets/card_J.png") ||
        !cardQ.loadFromFile("Assets/card_Q.png") ||
        !cardK.loadFromFile("Assets/card_K.png") ||
        !cardA.loadFromFile("Assets/card_A.png") ||
        !cardHidden.loadFromFile("Assets/card_hidden.png") ||
        !cardGameUI.loadFromFile("Assets/cardgame_UI.png")) {
        std::cerr << "Error - Cardgame textures not found!" << std::endl;
    }

    // Gambling result screens
    if (!winTexture.loadFromFile("Assets/you_win.png") ||
        !loseTexture.loadFromFile("Assets/you_lose.png")) {
        std::cerr << "Error - Gambling result textures (you_win.png / you_lose.png) not found!" << std::endl;
    }

    // ==========================================
    // 5. FONTS
    // ==========================================

    // Check if your SFML version uses openFromFile or loadFromFile for fonts
    if (!hudFont.openFromFile("Assets/retro_font.ttf")) {
        std::cerr << "Error - HUD Font (retro_font.ttf) not found!" << std::endl;
    }

    // ==========================================
    // 6. LOADING SOUNDS (SFX)
    // ==========================================

    // Main system sounds
    if (!winBuffer.loadFromFile("Assets/win_sound.mp3") ||
        !loseBuffer.loadFromFile("Assets/lose_sound.mp3") ||
        !playerDamagedBuffer.loadFromFile("Assets/security_punch.wav") ||
        !shopSoundBuffer.loadFromFile("Assets/shop_effect.wav") ||
        !clickSoundBuffer.loadFromFile("Assets/click_sound.wav") ||
        !wrongButtonBuffer.loadFromFile("Assets/wrong_button.wav")) {
        std::cerr << "Error - Core sounds not found!" << std::endl;
    }

    // Enemy damaged sounds
    if (!enemyDamagedBuffer.loadFromFile("Assets/security_ouch_effect.wav")) {
        std::cerr << "Error - Sound (security_ouch_effect.wav) not found!" << std::endl;
    }

    if (!enemyDamagedBuffer2.loadFromFile("Assets/security_ouch_effect2.wav")) {
        std::cerr << "Error - Sound (security_ouch_effect2.wav) not found!" << std::endl;
    }

    if (!enemyDamagedBuffer3.loadFromFile("Assets/security_ouch_effect3.wav")) {
        std::cerr << "Error - Sound (security_ouch_effect3.wav) not found!" << std::endl;
    }

    // Throw/shoot sounds
    if (!shootBuffer1.loadFromFile("Assets/throw1.mp3")) {
        std::cerr << "Error - Sound (throw1.mp3) not found!" << std::endl;
    }

    if (!shootBuffer2.loadFromFile("Assets/throw2.mp3")) {
        std::cerr << "Error - Sound (throw2.mp3) not found!" << std::endl;
    }

    // ==========================================
    // 7. LOADING MUSIC
    // ==========================================

    if (!menuMusic.openFromFile("Assets/menu_music.mp3")) {
        std::cerr << "Error! menu_music.mp3 not found!" << std::endl;
    } else {
        menuMusic.setLooping(true);
        menuMusic.setVolume(50.0f);
    }

    if (!hubMusic.openFromFile("Assets/hub_music.mp3")) {
        std::cerr << "Error! hub_music.mp3 not found!" << std::endl;
    } else {
        hubMusic.setLooping(true);
        hubMusic.setVolume(50.0f);
    }

    if (!minigameMusic.openFromFile("Assets/minigame_music.mp3")) {
        std::cerr << "Error! minigame_music.mp3 not found!" << std::endl;
    } else {
        minigameMusic.setLooping(true);
        minigameMusic.setVolume(50.0f);
    }

    if (!survivalMusic.openFromFile("Assets/survival_music.mp3")) {
        std::cerr << "Error! survival_music.mp3 not found!" << std::endl;
    } else {
        survivalMusic.setLooping(true);
        survivalMusic.setVolume(50.0f);
    }
}