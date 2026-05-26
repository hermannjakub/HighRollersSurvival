#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include <random>

#include "player.h"
#include "projectile.h"
#include "enemy.h"
#include "assetManager.h"
#include "gameState.h"
#include "roulette.h"
#include "cardGame.h"
#include "shop.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::Clock clock;

    // --- RESOURCE MANAGER ---
    AssetManager assets;

    // --- GAME MODULES ---
    Roulette roulette;
    CardGame cardGame;
    Shop shop;

    // --- SPRITES & TEXTS ---
    sf::Sprite backgroundSprite;
    sf::Sprite gameOverSprite;
    sf::Text hudText;

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- GAME VARIABLES ---
    GameState currentState;
    int casinoHeat;      // Aktualny poziom "złości" kasyna
    int heatThreshold;   // Próg, przy którym kasyno wysyła ochronę
    float shootTimer;
    std::string gameOverReason;

    int playerMoney;
    int daysSurvived;
    int playerHp;
    int playerMaxHp;
    int playerDamage;

    std::vector<sf::FloatRect> casinoObstacles;
};