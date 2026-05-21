#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "assetManager.h"
#include "gameObject.h"
#include "enemy.h"
#include "gameState.h"

class Roulette {
public:
    // Constructor takes the asset manager to initialize the textures
    Roulette(AssetManager& assets);

    // Update gets references to the player from game.cpp to be able to modify them.
    void update(float dt, sf::RenderWindow& window, float& shootTimer,
                int& playerMoney, int& consecutiveWins, GameState& currentState,
                std::vector<std::unique_ptr<GameObject>>& gameObjects,
                int& playerHp, AssetManager& assets);

    void render(sf::RenderWindow& window);

    // Getters for HUB.
    int getBetRed() const { return betRed; }
    int getBetBlack() const { return betBlack; }
    int getBetGreen() const { return betGreen; }

private:
    // Visuals of ui
    sf::Sprite wheelSprite;
    sf::Sprite pointerSprite;
    sf::Sprite rouletteUISprite;
    sf::Sprite winLoseSprite;
    // Sounds for results.
    sf::Sound winSound;
    sf::Sound loseSound;
    // Variables storing the amount of money player have bet on every colour.
    int betRed;
    int betBlack;
    int betGreen;
    // Containers storing the visuals of bets.
    std::vector<sf::Sprite> redChipsVisuals;
    std::vector<sf::Sprite> blackChipsVisuals;
    std::vector<sf::Sprite> greenChipsVisuals;
    // Logic variables.
    bool isSpinning;
    float spinSpeed;
    float spinTimer;
    float resultDisplayTimer;
    bool showResultSprite;
};