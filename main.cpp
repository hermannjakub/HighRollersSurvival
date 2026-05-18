#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include "Player.h"
#include "projectile.h"

// Separating events - setting a gamestate for every of them.
enum class GameState {
    Hub,            // Free roaming around the casino (no enemies)
    ShopUI,         // Shop
    RouletteGame,   // Roullete table
    CardGame,      // Card game
    Survival        // Fight mode (when enemies are spawned)
};


int main() {
    // Initialising the window size and framerate.
    sf::RenderWindow window(sf::VideoMode({1600, 893}), "High Roller's Survival");
    window.setFramerateLimit(60);

    // Initialising the background texture.
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("casino_map.png")) {
        std::cerr << "Blad: Nie znaleziono pliku casino_map.png!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);


    // Initialising player's standing texture.
    sf::Texture playerStandingTexture;
    if (!playerStandingTexture.loadFromFile("playerStanding.png")) {
        std::cerr << "Blad: Nie znaleziono pliku player.png!" << std::endl;
        return -1; // Closing the window, if graphics not found.
    }

    // Initialising player's moving texture 1.
    sf::Texture playerMovingTexture1;
    if (!playerMovingTexture1.loadFromFile("playerMove1.png")) {
        std::cerr << "Blad: Nie znaleziono pliku player.png!" << std::endl;
        return -1; // Closing the window, if graphics not found.
    }
    // Initialising player's moving texture 2.
    sf::Texture playerMovingTexture2;
    if (!playerMovingTexture2.loadFromFile("playerMove2.png")) {
        std::cerr << "Blad: Nie znaleziono pliku player.png!" << std::endl;
        return -1; // Closing the window, if graphics not found.
    }
    // Initialising chip texture ( projectile)

    sf::Texture chipTexture;
    if (!chipTexture.loadFromFile("poker_chip.png")) {
        std::cerr << "Blad: Nie znaleziono pliku chip.png!" << std::endl;
        return -1;
    }

    // Variable which stores the time between projectiles sent.
    float shootTimer = 0.0f;

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // Initialising the player.
    gameObjects.push_back(std::make_unique<Player>(playerStandingTexture, playerMovingTexture1, playerMovingTexture2, 400.0f, 300.0f));

    GameState currentState = GameState::Hub; // Setting deafult gamestate.
    int consecutiveWins = 0; // Counter of wins in roullete/card game.

    // Clock for the movement.
    sf::Clock clock;

    // Main game loop
    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        // Event manager - closing the window.
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        shootTimer += dt;

        if (currentState == GameState::Hub) {

            // If we pressed left mouse button and there was at least 0.3 sec from the pervious shot
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTimer >= 0.3f) {
                shootTimer = 0.0f; // Restarting the timer

                // Getting player position
                sf::Vector2f playerPos = gameObjects[0]->getPosition();

                // Creating the projectile and putting it in the container.
                gameObjects.push_back(std::make_unique<Projectile>(chipTexture, playerPos.x, playerPos.y, sf::Vector2f(1.0f, 0.0f)));
            }

            // Updating the elements of the game (walking, projectiles).
            for (auto& obj : gameObjects) {
                obj->update(dt);
            }

            // Definining invisible interactive zones, which are responsible for interactions with the shop and games.
            sf::FloatRect rouletteZone(sf::Vector2f(20.0f, 250.0f), sf::Vector2f(300.0f, 400.0f)); //Left side, where the roullette is located.
            sf::FloatRect shopZone(sf::Vector2f(600.0f, 100.0f), sf::Vector2f(250.0f, 250.0f));  // Upper area, where the shop is located
            sf::FloatRect slotsZone(sf::Vector2f(1200.0f, 400.0f), sf::Vector2f(200.0f, 200.0f));  // Right area where the card games are located.

            // Checking the interaction, if player pressed E.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {

                // Getting players position
                sf::Vector2f pos = gameObjects[0]->getPosition();

                // Checking in which zone the player is at the moment.
                if (rouletteZone.contains(pos)) {
                    currentState = GameState::RouletteGame;
                    std::cout << "Otwieram Ruletke!" << std::endl;
                }
                else if (shopZone.contains(pos)) {
                    currentState = GameState::ShopUI;
                    std::cout << "Otwieram Sklep!" << std::endl;
                }
                else if (slotsZone.contains(pos)) {
                    currentState = GameState::CardGame;
                    std::cout << "Otwieram Maszyny!" << std::endl;
                }
            }
        }
        else if (currentState == GameState::RouletteGame) {
            // Roulette logic
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                currentState = GameState::Hub; // Exiting the roulette menu
                std::cout << "Wracam do kasyna." << std::endl;
            }
        }
        else if (currentState == GameState::ShopUI) {
            // Shop logic
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                currentState = GameState::Hub; // Exiting the shop
                std::cout << "Wracam do kasyna." << std::endl;
            }
        }
        else if (currentState == GameState::CardGame) {
            // Card game logic
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                currentState = GameState::Hub; // Exiting the card game
                std::cout << "Wracam do kasyna." << std::endl;
            }
        }

        window.clear(sf::Color::Black);

        // Drawing the casino background
        window.draw(backgroundSprite);


        // Drawing the objects.
        for (auto& obj : gameObjects) {
            obj->draw(window);
        }

        // Displaying.
        window.display();
    }

    return 0;
}