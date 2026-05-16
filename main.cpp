#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include "Player.h" // Upewnij się, że masz tu nagłówek ze swoim graczem

int main() {
    // Initialising the window size and framerate.
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "High Roller's Survival");
    window.setFramerateLimit(60);

    // Initialising player's standing texture.
    sf::Texture playerStandingTexture;
    if (!playerStandingTexture.loadFromFile("playerStanding.png")) {
        std::cerr << "Blad: Nie znaleziono pliku player.png!" << std::endl;
        return -1; // Closing the window, if graphics not found.
    }

    // Initialising player's moving texture 1.
    sf::Texture playerMovingTexture;
    if (!playerMovingTexture.loadFromFile("playerMoving1.png")) {
        std::cerr << "Blad: Nie znaleziono pliku player.png!" << std::endl;
        return -1; // Closing the window, if graphics not found.
    }

    // Container for game objects.
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // Initialising the player.
    gameObjects.push_back(std::make_unique<Player>(playerStandingTexture, playerMovingTexture, 400.0f, 300.0f));

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

        // Updating every game object - basing on its hp, position etc.
        for (auto& obj : gameObjects) {
            obj->update(dt);
        }

        // Drawing the window itself
        window.clear(sf::Color(0, 100, 0));

        // Drawing the objects.
        for (auto& obj : gameObjects) {
            obj->draw(window);
        }

        // Displaying.
        window.display();
    }

    return 0;
}