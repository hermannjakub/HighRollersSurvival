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

// Separating events - setting a gamestate for every of them.
enum class GameState {
    Hub,            // Free roaming around the casino (no enemies)
    ShopUI,         // Shop
    RouletteGame,   // Roullete table
    CardGame,       // Card game
    Survival        // Fight mode (when enemies are spawned)
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow window;

    // Clock for the movement.
    sf::Clock clock;

    // Textures & Sprites
    sf::Texture backgroundTexture, playerStandingTexture, playerMovingTexture1, playerMovingTexture2;
    sf::Texture chipTexture, rouletteChip;
    sf::Texture wheelTexture, pointerTexture, rouletteUITexture;
    sf::Sprite backgroundSprite, wheelSprite, pointerSprite, rouletteUISprite;

    sf::Font hudFont;
    sf::Text hudText;

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- GAME VARIABLES ---
    GameState currentState; // Setting deafult gamestate.
    int consecutiveWins;    // Counter of wins in roullete/card game.
    float shootTimer;       // Variable which stores the time between projectiles sent.

    // --- NOWE STATYSTYKI GRACZA ---
    int playerMoney;        // Entry money for the player
    int daysSurvived;       // Counter of days survived
    int playerHp;           // Entry player hp.

    // --- KOSZYKI NA MULTI-BET ---
    int betRed;
    int betBlack;
    int betGreen;

    // Containers for textures of chips.
    std::vector<sf::Sprite> redChipsVisuals;
    std::vector<sf::Sprite> blackChipsVisuals;
    std::vector<sf::Sprite> greenChipsVisuals;

    // Variables for the roulette.
    bool isSpinning;
    float spinSpeed;
    float spinTimer;
    int playerBet;          // -1:No bet, 0:Green, 1:Red, 2:Black.
};