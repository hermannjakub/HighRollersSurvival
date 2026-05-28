#include "pauseMenu.h"

PauseMenu::PauseMenu(AssetManager& assets)
    : titleText(assets.hudFont), resumeText(assets.hudFont), saveText(assets.hudFont),
      settingsText(assets.hudFont), mainMenuText(assets.hudFont),
      volumeTitleText(assets.hudFont), volumeValueText(assets.hudFont),
      volumeInfoText(assets.hudFont), backText(assets.hudFont)
{
    // Setting up the see-through layer for the pause (Alpha = 200/255)
    overlay.setSize(sf::Vector2f(1600.0f, 893.0f));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));

    // Pause menu text
    setupText(titleText, "PAUSE", 70, sf::Vector2f(800.0f, 200.0f));
    titleText.setFillColor(sf::Color::Red);

    setupText(resumeText, "RESUME", 40, sf::Vector2f(800.0f, 350.0f));
    setupText(saveText, "SAVE PROGRESS", 40, sf::Vector2f(800.0f, 450.0f));
    setupText(settingsText, "SETTINGS", 40, sf::Vector2f(800.0f, 550.0f));
    setupText(mainMenuText, "MAIN MENU", 40, sf::Vector2f(800.0f, 650.0f));

    resumeHitbox   = sf::FloatRect(sf::Vector2f(600.0f, 320.0f), sf::Vector2f(400.0f, 60.0f));
    saveHitbox     = sf::FloatRect(sf::Vector2f(600.0f, 420.0f), sf::Vector2f(400.0f, 60.0f));
    settingsHitbox = sf::FloatRect(sf::Vector2f(600.0f, 520.0f), sf::Vector2f(400.0f, 60.0f));
    mainMenuHitbox = sf::FloatRect(sf::Vector2f(600.0f, 620.0f), sf::Vector2f(400.0f, 60.0f));

    // Mid pause settings menu text
    setupText(volumeTitleText, "VOLUME SETTINGS", 50, sf::Vector2f(800.0f, 200.0f));
    setupText(volumeValueText, "100%", 50, sf::Vector2f(800.0f, 400.0f));
    setupText(volumeInfoText, "[ Click on the sound lever: LEFTCLICK = -10% | RIGHTCLICK = +10% ]", 22, sf::Vector2f(800.0f, 480.0f));
    volumeInfoText.setFillColor(sf::Color::Yellow);
    setupText(backText, "RETURN", 40, sf::Vector2f(800.0f, 650.0f));

    volUpHitbox = sf::FloatRect(sf::Vector2f(700.0f, 370.0f), sf::Vector2f(200.0f, 70.0f));
    backHitbox  = sf::FloatRect(sf::Vector2f(650.0f, 620.0f), sf::Vector2f(300.0f, 60.0f));
}

void PauseMenu::setupText(sf::Text& text, const std::string& str, int size, sf::Vector2f pos) {
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color(100, 0, 0));
    text.setOutlineThickness(2.0f);
    
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    text.setPosition(pos);
}

bool PauseMenu::update(float dt, sf::RenderWindow& window, float& shootTimer, GameState& currentState,
                       float& currentVolume, AssetManager& assets) 
{
    bool requestSave = false;
    if (shootTimer < 0.2f) return requestSave; 

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

    if (currentState == GameState::PauseMenu && leftClicked) {
        shootTimer = 0.0f;
        if (resumeHitbox.contains(mousePos)) {
            assets.clickSound.play();
            currentState = GameState::Hub; 
        }
        else if (saveHitbox.contains(mousePos)) {
            assets.clickSound.play();
            requestSave = true; // Flag causing creating the save in game.cpp
        }
        else if (settingsHitbox.contains(mousePos)) {
            assets.clickSound.play();
            currentState = GameState::PauseSettings; 
        }
        else if (mainMenuHitbox.contains(mousePos)) {
            assets.clickSound.play();
            currentState = GameState::MainMenu; // Back to main menu
        }
    }
    else if (currentState == GameState::PauseSettings) {
        if (volUpHitbox.contains(mousePos) && (leftClicked || rightClicked)) {
            shootTimer = 0.0f;
            if (leftClicked && currentVolume > 0.0f) currentVolume -= 10.0f;
            if (rightClicked && currentVolume < 100.0f) currentVolume += 10.0f;
            
            sf::Listener::setGlobalVolume(currentVolume);
            assets.clickSound.play();
        }
        
        if (backHitbox.contains(mousePos) && leftClicked) {
            shootTimer = 0.0f;
            assets.clickSound.play();
            currentState = GameState::PauseMenu; 
        }
    }

    volumeValueText.setString(std::to_string(static_cast<int>(currentVolume)) + "%"); // Updating the volume level.
    return requestSave;
}

void PauseMenu::render(sf::RenderWindow& window, GameState currentState) {
    // Drawing the see-trough layer
    window.draw(overlay);

    if (currentState == GameState::PauseMenu) {
        window.draw(titleText);
        window.draw(resumeText);
        window.draw(saveText);
        window.draw(settingsText);
        window.draw(mainMenuText);
    } 
    else if (currentState == GameState::PauseSettings) {
        window.draw(volumeTitleText);
        window.draw(volumeValueText);
        window.draw(volumeInfoText);
        window.draw(backText);
    }
}