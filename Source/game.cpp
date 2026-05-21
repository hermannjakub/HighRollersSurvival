#include "game.h"

Game::Game() :
    window(sf::VideoMode({1600, 893}), "High Roller's Survival"),
    roulette(assets),// Initialising the roulette module.
    cardGame(assets), // Initialising the cardgame module.
    backgroundSprite(assets.background), // Initialising the background sprite.
    hudText(assets.hudFont) // Initialising the hud elements.
{
    // Initialising the window size and framerate.
    window.setFramerateLimit(60);

    // Setting up the background.
    backgroundSprite.setTexture(assets.background, true);

    // Initialising the player.
    gameObjects.push_back(std::make_unique<Player>(assets.playerStanding, assets.playerMove1, assets.playerMove2, 400.0f, 300.0f));

    // Fixing the hud position in top-left corner.
    hudText.setFont(assets.hudFont);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(sf::Vector2f(20.0f, 20.0f));

    // Setting starting variables for player.
    shootTimer = 0.0f;
    currentState = GameState::Hub;
    consecutiveWins = 0;
    playerMoney = 200;
    daysSurvived = 1;
    playerHp = 100;
}

// Main game loop
void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

// Event manager - closing the window.
void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::update(float dt) {
    shootTimer += dt;

    if (currentState == GameState::Hub || currentState == GameState::Survival) {

        // If we pressed left mouse button and there was at least 0.3 sec from the pervious shot
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTimer >= 0.3f) {
            shootTimer = 0.0f;

            // Setting starting position (player)
            sf::Vector2f playerPos = gameObjects[0]->getPosition();

            // Aim position - mouse
            // Getting the mouse position and saving it as game coordinates
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

            // Calculating direction vector for the projectile
            sf::Vector2f aimDirection = mousePos - playerPos;

            // Creating a projectile with direction vector set to where our mouse is.
            gameObjects.push_back(std::make_unique<Projectile>(assets.chip, playerPos.x, playerPos.y, aimDirection));
        }

        // Updating the elements of the game (walking, projectiles).
        for (auto& obj : gameObjects) {
            obj->update(dt);
        }

        // --- Collision system (projectiles vs enemies) ----
        for (auto& obj1 : gameObjects) {
            if (Projectile* proj = dynamic_cast<Projectile*>(obj1.get())) {
                if (proj->isDestroyed) continue;

                for (auto& obj2 : gameObjects) {
                    if (Enemy* enemy = dynamic_cast<Enemy*>(obj2.get())) {
                        if (enemy->isDestroyed) continue;

                        // If the projectile touches enemy
                        if (proj->getGlobalBounds().findIntersection(enemy->getGlobalBounds())) {
                            enemy->takeDamage(10); // Projectile deals 10 damage
                            proj->isDestroyed = true; // Projectile disappears after touching the enemy.
                        }
                    }
                }
            }
        }

        // Clearing destroyed objects.
        std::erase_if(gameObjects, [](const std::unique_ptr<GameObject>& obj) {
            return obj->isDestroyed;
        });

        // Definining invisible interactive zones, which are responsible for interactions with the shop and games.
        sf::FloatRect rouletteZone(sf::Vector2f(30.0f, 200.0f), sf::Vector2f(300.0f, 480.0f)); // Left side, where the roullette is located.
        sf::FloatRect shopZone(sf::Vector2f(590.0f, 110.0f), sf::Vector2f(370.0f, 220.0f));  // Upper area, where the shop is located
        sf::FloatRect cardZone(sf::Vector2f(1300.0f, 335.0f), sf::Vector2f(270.0f, 260.0f));  // Right area where the card games are located.

        // Checking the interaction, if player pressed E.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {

            // Getting players position
            sf::Vector2f pos = gameObjects[0]->getPosition();

            // Checking in which zone the player is at the moment of pressing E.
            if (rouletteZone.contains(pos)) {
                currentState = GameState::RouletteGame;
                std::cout << "Into the roulette!" << std::endl;
            }
            else if (shopZone.contains(pos)) {
                currentState = GameState::ShopUI;
                std::cout << "Into the shop!" << std::endl;
            }
            else if (cardZone.contains(pos)) {
                currentState = GameState::CardGame;
                std::cout << "Into the card games!" << std::endl;
            }
        }
    }
    else if (currentState == GameState::RouletteGame) {
        // Delegating all roulette logic to the Roulette class.
        roulette.update(dt, window, shootTimer, playerMoney, consecutiveWins, currentState, gameObjects, playerHp, assets);
    }
    else if (currentState == GameState::ShopUI) {
        // Shop logic
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            currentState = GameState::Hub; // Exiting the shop
            std::cout << "Going back to the casino." << std::endl;
        }
    }
    else if (currentState == GameState::CardGame) {
        // Delegating all card game logic to the CardGame class.
        cardGame.update(dt, window, shootTimer, playerMoney, consecutiveWins, currentState, gameObjects, playerHp, assets);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Drawing the casino background
    window.draw(backgroundSprite);

    // 2. Drawing and updating the elements if we are in the hub or in survival.
    if (currentState == GameState::Hub || currentState == GameState::Survival) {
        for (auto& obj : gameObjects) {
            obj->draw(window);
        }
    }
    else if (currentState == GameState::RouletteGame) {
        // Delegating the rendering of the roulette to the Roulette class.
        roulette.render(window);
    }

    // Drawing the HUD.
    std::string hudString = "HP: " + std::to_string(playerHp) +
                            " | MONEY: $" + std::to_string(playerMoney) +
                            " | DAY: " + std::to_string(daysSurvived);

    // Additional information in the hud when we are in the roulette event.
    if (currentState == GameState::RouletteGame) {
        hudString += "\nCurrent bets -> Red: $" + std::to_string(roulette.getBetRed()) +
                     " | Black: $" + std::to_string(roulette.getBetBlack()) +
                     " | Green: $" + std::to_string(roulette.getBetGreen());
    }

    else if (currentState == GameState::CardGame) {
        // Delegating the rendering of the card game to the CardGame class.
        cardGame.render(window);
    }


    hudText.setString(hudString);
    window.draw(hudText); // Drawing the hud itself.

    // Displaying.
    window.display();
}