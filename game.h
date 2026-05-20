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

    // --- SPRITES & TEXTS ---
    sf::Sprite backgroundSprite;
    sf::Text hudText;

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- GAME VARIABLES ---
    GameState currentState;
    int consecutiveWins;
    float shootTimer;

    int playerMoney;
    int daysSurvived;
    int playerHp;
};