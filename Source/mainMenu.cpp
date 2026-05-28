#include "mainMenu.h"
#include <iostream>

MainMenu::MainMenu(AssetManager& assets)
    : menuBgSprite(assets.menuBackground), settingsBgSprite(assets.settingsBackground),
      newGameText(assets.hudFont), loadGameText(assets.hudFont),
      settingsText(assets.hudFont), exitText(assets.hudFont), volumeTitleText(assets.hudFont),
      volumeValueText(assets.hudFont), volumeInfoText(assets.hudFont), backText(assets.hudFont)
{
    // Setting the position of background sprites.
    menuBgSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
    settingsBgSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Text configuration for the main menu.

    // Text rendered on the main menu background.
    setupText(newGameText, "NEW GAME", 40, sf::Vector2f(800.0f, 350.0f));
    newGameText.setOutlineColor(sf::Color(100, 0, 0)); // Outline of the text.
    newGameText.setOutlineThickness(2.0f);

    setupText(loadGameText, "LOAD GAME", 40, sf::Vector2f(800.0f, 450.0f));
    loadGameText.setOutlineColor(sf::Color(100, 0, 0));
    loadGameText.setOutlineThickness(2.0f);

    setupText(settingsText, "SETTINGS", 40, sf::Vector2f(800.0f, 550.0f));
    settingsText.setOutlineColor(sf::Color(100, 0, 0));
    settingsText.setOutlineThickness(2.0f);

    setupText(exitText, "EXIT", 40, sf::Vector2f(800.0f, 650.0f));
    exitText.setOutlineColor(sf::Color(100, 0, 0));
    exitText.setOutlineThickness(2.0f);

    // Hitboxes of buttons on main menu
    newGameHitbox  = sf::FloatRect(sf::Vector2f(650.0f, 320.0f), sf::Vector2f(300.0f, 60.0f));
    loadGameHitbox = sf::FloatRect(sf::Vector2f(650.0f, 420.0f), sf::Vector2f(300.0f, 60.0f));
    settingsHitbox = sf::FloatRect(sf::Vector2f(650.0f, 520.0f), sf::Vector2f(300.0f, 60.0f));
    exitHitbox     = sf::FloatRect(sf::Vector2f(650.0f, 620.0f), sf::Vector2f(300.0f, 60.0f));

    // TEXT CONFIGURATION FOR SETTINGS GAMESTATE
    setupText(volumeTitleText, "VOLUME SETTINGS", 50, sf::Vector2f(800.0f, 200.0f));

    setupText(volumeValueText, "100%", 50, sf::Vector2f(800.0f, 400.0f)); // Volume level in the middle

    setupText(volumeInfoText, "[ Click on the sound lever: LEFTCLICK = -10% | RIGHTCLICK = +10% ]", 22, sf::Vector2f(800.0f, 480.0f));
    volumeInfoText.setFillColor(sf::Color::Yellow);

    setupText(backText, "RETURN", 35, sf::Vector2f(800.0f, 650.0f)); // Return button

    // Hitboxes for settings
    volUpHitbox   = sf::FloatRect(sf::Vector2f(700.0f, 375.0f), sf::Vector2f(200.0f, 70.0f)); // Clicking on the volume level itself.
    backHitbox    = sf::FloatRect(sf::Vector2f(650.0f, 630.0f), sf::Vector2f(300.0f, 50.0f));
}

void MainMenu::setupText(sf::Text& text, const std::string& str, int size, sf::Vector2f pos) {
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);

    // Setting the text on the centre.
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    text.setPosition(pos);
}

void MainMenu::update(float dt, sf::RenderWindow& window, float& shootTimer, GameState& currentState,
                      bool& requestLoad, float& currentVolume, AssetManager& assets)
{
    if (shootTimer < 0.2f) return; // Small clicking cooldown

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

    if (currentState == GameState::MainMenu && leftClicked) {
        shootTimer = 0.0f;
        if (newGameHitbox.contains(mousePos)) {
            assets.clickSound.play();
            currentState = GameState::Hub; // Starting a new game
        }
        else if (loadGameHitbox.contains(mousePos)) {
            assets.clickSound.play();
            requestLoad = true; // Flag for the class to load the save.txt file
            currentState = GameState::Hub;
        }
        else if (settingsHitbox.contains(mousePos)) {
            assets.clickSound.play();
            currentState = GameState::Settings; // Going into settings
        }
        else if (exitHitbox.contains(mousePos)) {
            window.close();
        }
    }
    else if (currentState == GameState::Settings) {
        // Changing the volume.
        if (volUpHitbox.contains(mousePos) && (leftClicked || rightClicked)) {
            shootTimer = 0.0f;
            if (leftClicked && currentVolume > 0.0f) currentVolume -= 10.0f;
            if (rightClicked && currentVolume < 100.0f) currentVolume += 10.0f;


            sf::Listener::setGlobalVolume(currentVolume);
            assets.clickSound.play(); // playing the sound for the player to hear.
        }

        if (backHitbox.contains(mousePos) && leftClicked) {
            shootTimer = 0.0f;
            assets.clickSound.play();
            currentState = GameState::MainMenu; // Going back to the main menu
        }
    }

    // updating the volume level
    volumeValueText.setString(std::to_string(static_cast<int>(currentVolume)) + "%");
}

void MainMenu::render(sf::RenderWindow& window, GameState currentState) {

    if (currentState == GameState::MainMenu) {
        window.draw(menuBgSprite); // Drawing the menu background

        // Drawing the text on the panels
        window.draw(newGameText);
        window.draw(loadGameText);
        window.draw(settingsText);
        window.draw(exitText);
    }
    else if (currentState == GameState::Settings) {
        window.draw(settingsBgSprite); // Drawing the settings background

        // Drawing the text in the central settings panel
        window.draw(volumeTitleText);
        window.draw(volumeValueText);
        window.draw(volumeInfoText);
        window.draw(backText);
    }
}