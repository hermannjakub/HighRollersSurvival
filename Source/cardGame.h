#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "assetManager.h"
#include "gameState.h"
#include "gameObject.h"
#include "enemy.h"
#include "enemyTypes.h"

class CardGame {
public:
    CardGame(AssetManager& assets);

    void update(float dt, sf::RenderWindow& window, float& shootTimer,
                int& playerMoney, int& casinoHeat, int& heatThreshold, GameState& currentState,
                std::vector<std::unique_ptr<GameObject>>& gameObjects,
                int& playerHp, AssetManager& assets,
                int& daysSurvived, const std::vector<sf::FloatRect>& casinoObstacles);

    void render(sf::RenderWindow& window);
    void startNewRound(); // Draws new cards

private:
    // UI and cards
    sf::Sprite cardGameUISprite; // Background UI for the card game
    sf::Sprite playerCardSprite;
    sf::Sprite dealerCardSprite;
    sf::Sprite winLoseSprite;

    // Invisible hitboxes for buttons (X, Y, Width, Height)
    sf::FloatRect higherHitbox;
    sf::FloatRect lowerHitbox;
    sf::FloatRect playHitbox;

    int playerCardValue; // 11 (J), 12 (Q), 13 (K)
    int dealerCardValue; // 10, 11, 12, 13, 14 (A)

    int betHigher;
    int betLower;

    std::vector<sf::Sprite> higherChipsVisuals;
    std::vector<sf::Sprite> lowerChipsVisuals;

    bool isRevealed;
    float revealTimer;
    bool showResultSprite;

    void setCardTexture(sf::Sprite& sprite, int value, AssetManager& assets);
};