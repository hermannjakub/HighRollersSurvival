#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include <random>
#include <fstream>

#include "player.h"
#include "projectile.h"
#include "enemy.h"
#include "assetManager.h"
#include "gameState.h"
#include "roulette.h"
#include "cardGame.h"
#include "shop.h"
#include "mainMenu.h"
#include "pauseMenu.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void updateMusic();
    void saveGame();
    void loadGame();
    // Class responsible for distingushing which music is necessary.
    enum class MusicTheme { None, Menu, Hub, Minigame, Survival };
    // Variable responsible for current music.
    MusicTheme currentMusicTheme;
    // Rendering variables.
    sf::RenderWindow window;
    sf::Clock clock;

    // --- RESOURCE MANAGER ---
    AssetManager assets;

    // --- GAME MODULES ---
    Roulette roulette;
    CardGame cardGame;
    Shop shop;
    MainMenu mainMenu;
    PauseMenu pauseMenu;

    // --- SPRITES & TEXTS ---
    sf::Sprite backgroundSprite;
    sf::Sprite gameOverSprite;
    sf::Text hudText;

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- GAME VARIABLES ---
    GameState currentState;
    int casinoHeat;      // Level of "angryness" of the casino
    int heatThreshold;   // A point where the guards are spawning
    float shootTimer;    // Cooldown timer for the shots.
    std::string gameOverReason; // Reason of losing (appearing in the game over gamestate)

// Player variables
    float globalVolume;
    int playerMoney;
    int daysSurvived;
    int playerHp;
    int playerMaxHp;
    int playerDamage;

    // Vector with obstacles for enemies.
    std::vector<sf::FloatRect> casinoObstacles;
    // Flag for reducing double clicks.
    bool wasEscPressed;
};