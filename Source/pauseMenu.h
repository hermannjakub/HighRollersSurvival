#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "assetManager.h"
#include "gameState.h"

class PauseMenu {
public:
    PauseMenu(AssetManager& assets);

    // Zwraca true jeśli gracz kliknął "Zapisz", co da znać klasie Game, by wywołała saveGame()
    bool update(float dt, sf::RenderWindow& window, float& shootTimer, GameState& currentState,
                float& currentVolume, AssetManager& assets);
    
    void render(sf::RenderWindow& window, GameState currentState);

private:
    sf::RectangleShape overlay; // See-through filter put on tge gameplay.

    sf::Text titleText;
    sf::Text resumeText;
    sf::Text saveText;
    sf::Text settingsText;
    sf::Text mainMenuText;

    sf::Text volumeTitleText;
    sf::Text volumeValueText;
    sf::Text volumeInfoText;
    sf::Text backText;

    sf::FloatRect resumeHitbox;
    sf::FloatRect saveHitbox;
    sf::FloatRect settingsHitbox;
    sf::FloatRect mainMenuHitbox;
    sf::FloatRect volUpHitbox;
    sf::FloatRect backHitbox;

    void setupText(sf::Text& text, const std::string& str, int size, sf::Vector2f pos);
};