#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "assetManager.h"
#include "gameState.h"

class MainMenu {
public:
    MainMenu(AssetManager& assets);

    void update(float dt, sf::RenderWindow& window, float& shootTimer, GameState& currentState, 
                bool& requestLoad, float& currentVolume, AssetManager& assets);
    void render(sf::RenderWindow& window, GameState currentState);

private:

    sf::Sprite menuBgSprite;
    sf::Sprite settingsBgSprite;

    // Main menu text
    sf::Text newGameText;
    sf::Text loadGameText;
    sf::Text settingsText;
    sf::Text exitText;

    // Settings text
    sf::Text volumeTitleText;
    sf::Text volumeValueText;
    sf::Text volumeInfoText;
    sf::Text backText;

    // Button hitboxes
    sf::FloatRect newGameHitbox;
    sf::FloatRect loadGameHitbox;
    sf::FloatRect settingsHitbox;
    sf::FloatRect exitHitbox;
    sf::FloatRect volUpHitbox;
    sf::FloatRect volDownHitbox;
    sf::FloatRect backHitbox;

    void setupText(sf::Text& text, const std::string& str, int size, sf::Vector2f pos);
};