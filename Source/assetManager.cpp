#include "assetManager.h"

#include <stdatomic.h>

AssetManager::AssetManager():
winSound(winBuffer),loseSound(loseBuffer),enemyDamaged(enemyDamagedBuffer),playerDamaged(playerDamagedBuffer),shopSound(shopSoundBuffer), clickSound(clickSoundBuffer){


    // Initialising the background and gameover texture.
    if (!background.loadFromFile("Assets/casino_map.png") ||
        !gameOver.loadFromFile("Assets/game_over.png")) {
        std::cerr << "Error - Textures (casino_map.png/game_over.png) not found!" << std::endl;
    }
    // Initialising main menu textures.
    if (!menuBackground.loadFromFile("Assets/main_menu_bg.png") ||
        !settingsBackground.loadFromFile("Assets/settings_menu_bg.png")) {
        std::cerr << "Error - Menu backgrounds (Assets/main_menu_bg.png / Assets/settings_menu_bg.png) NOT FOUND!" << std::endl;
        }
    //Initialising player's textures.
    if (!playerStanding.loadFromFile("Assets/playerStanding.png") ||
        !playerMove1.loadFromFile("Assets/playerMove1.png") ||
        !playerMove2.loadFromFile("Assets/playerMove2.png")) {
        std::cerr << "Error - Player textures (playerStanding.png, or playerMove1.png, or playerMove2.png) NOT FOUND!" << std::endl;
        }

    // Initialising chip texture ( projectile)
    if (!chip.loadFromFile("Assets/poker_chip.png")) {
        std::cerr << "Error - Texture (poker_chip.png) not found!" << std::endl;
    }

    // Initialising enemy textures
    if (!enemyFastRun1.loadFromFile("Assets/enemy_fast1.png") ||
        !enemyFastRun2.loadFromFile("Assets/enemy_fast2.png") ||
        !enemyFastAttack1.loadFromFile("Assets/enemy_fast_attack_1.png") ||
        !enemyFastAttack2.loadFromFile("Assets/enemy_fast_attack_2.png")) {
        std::cerr << "Error - Texturs (enemy_fast1.png, or enemy_fast2.png or enemy_fast_attack.png) not found!" << std::endl;
    }
    if (!enemyTankRun1.loadFromFile("Assets/enemy_tank1.png") ||
        !enemyTankRun2.loadFromFile("Assets/enemy_tank2.png") ||
        !enemyTankAttack1.loadFromFile("Assets/enemy_tank_attack_1.png") ||
        !enemyTankAttack2.loadFromFile("Assets/enemy_tank_attack_2.png")) {
        std::cerr << "Error - Texturs (enemy_fast1.png, or enemy_fast2.png or enemy_fast_attack.png) not found!" << std::endl;
        }
    if (!enemyBossRun1.loadFromFile("Assets/enemy_boss1.png") ||
        !enemyBossRun2.loadFromFile("Assets/enemy_boss2.png") ||
        !enemyBossAttack1.loadFromFile("Assets/enemy_boss_attack_1.png") ||
        !enemyBossAttack2.loadFromFile("Assets/enemy_boss_attack_2.png")) {
        std::cerr << "Error - Texturs (enemy_fast1.png, or enemy_fast2.png or enemy_fast_attack.png) not found!" << std::endl;
        }

    // Initialising roulette textures.
    if (!wheel.loadFromFile("Assets/roulette_wheel.png") ||
        !pointer.loadFromFile("Assets/win_pointer.png") ||
        !rouletteUI.loadFromFile("Assets/roulette_UI.png") ||
        !rouletteChip.loadFromFile("Assets/roulette_chip.png")) {
        std::cerr << "Error - Roulette textures (roulette_wheel.png, or win_pointer.png, or roulette_UI.png or roulette_chip.png) NOT FOUND!" << std::endl;
    }

    // Initialising gambling result textures
    if (!winTexture.loadFromFile("Assets/you_win.png") ||
        !loseTexture.loadFromFile("Assets/you_lose.png")) {
        std::cerr << "Error - Textures (you_win.png or you_lose.png) not found!" << std::endl;
    }

    // Initialising sounds
    if (!winBuffer.loadFromFile("Assets/win_sound.mp3") ||
        !loseBuffer.loadFromFile("Assets/lose_sound.mp3") ||
        !enemyDamagedBuffer.loadFromFile("Assets/security_ouch.wav") ||
        !playerDamagedBuffer.loadFromFile("Assets/security_punch.wav") ||
        !shopSoundBuffer.loadFromFile("Assets/shop_effect.wav") ||
        !clickSoundBuffer.loadFromFile("Assets/click_sound.wav")) {
        std::cerr << "Error - sounds (win_sound.mp3/lose_sound.mp3/security_ouch.wav/security_punch.wav/shop_effect.wav) not found!" << std::endl;
        }else {
            // Connecting buffers to sound objects
            winSound.setBuffer(winBuffer);
            loseSound.setBuffer(loseBuffer);
            enemyDamaged.setBuffer(enemyDamagedBuffer);
            playerDamaged.setBuffer(playerDamagedBuffer);
            shopSound.setBuffer(shopSoundBuffer);
        }

    // Loading the font for the HUD
    if (!hudFont.openFromFile("Assets/retro_font.ttf")) {
        std::cerr << "Error - HUD Font (retro_font.ttf) not found!" << std::endl;
    }

    // Initialising the cardgame textures.
    if (!card10.loadFromFile("Assets/card_10.png") ||
        !cardJ.loadFromFile("Assets/card_J.png") ||
        !cardQ.loadFromFile("Assets/card_Q.png") ||
        !cardK.loadFromFile("Assets/card_K.png") ||
        !cardA.loadFromFile("Assets/card_A.png") ||
        !cardHidden.loadFromFile("Assets/card_hidden.png") ||
        !cardGameUI.loadFromFile("Assets/cardgame_UI.png")) {
        std::cerr << "Error - Cardgame textures (card_10.png/card_J.png/card_Q.png/card_K.png/card_A.png/card_hidden.png/cardgame_UI.png) not found!" << std::endl;
        }

    if (!shopUI.loadFromFile("Assets/shop_UI.png")) {
        std::cerr << "Error - Shop UI (Shop_UI.png) not found in Assets!" << std::endl;
    }

}
