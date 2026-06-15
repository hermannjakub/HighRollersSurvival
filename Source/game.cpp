#include "game.h"

Game::Game() :
    window(sf::VideoMode({1600, 893}), "High Roller's Survival"),
    roulette(assets),// Initialising the roulette module.
    cardGame(assets), // Initialising the cardgame module.
    backgroundSprite(assets.background), // Initialising the background sprite.
    gameOverSprite(assets.gameOver), // Initialising the gameover sprite.
    hudText(assets.hudFont),// Initialising the hud elements.
    shop(assets), // Initialising the shop background.
    mainMenu(assets), // Initialising main menu background and sounds.
    pauseMenu(assets) // Initialising pause menu assets.
{
    // Initialising the window size and framerate.
    window.setFramerateLimit(60);

    // Flag indicating if escape was already pressed - preventing going into pause menu after leaving some activities ingame.
    wasEscPressed = false;

    // Setting up the background.
    backgroundSprite.setTexture(assets.background, true);

    // Initialising the player.
    gameObjects.push_back(std::make_unique<Player>(assets.playerStanding, assets.playerMove1, assets.playerMove2, 800.0f, 446.0f));

    // Fixing the hud position in top-left corner.
    hudText.setFont(assets.hudFont);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(sf::Vector2f(20.0f, 20.0f));

    // Setting starting variables for player.
    shootTimer = 0.0f;
    currentState = GameState::MainMenu;
    currentMusicTheme = MusicTheme::None; // For the start we set "none" for the function to detect the needed music itself.
    casinoHeat = 0;
    heatThreshold = 50;
    playerMoney = 65;
    daysSurvived = 1;
    playerHp = 50;
    playerMaxHp = 50;
    playerDamage = 10;

    // Initial settings variables
    globalVolume = 100.0f;
    sf::Listener::setGlobalVolume(globalVolume);

    // Setting the obstacles for the enemies to prevent them from running on roulette table, shop and cardgame.
    casinoObstacles.push_back(sf::FloatRect(sf::Vector2f(30.0f, 200.0f), sf::Vector2f(300.0f, 350.0f))); // Roulette
    casinoObstacles.push_back(sf::FloatRect(sf::Vector2f(590.0f, 110.0f), sf::Vector2f(370.0f, 220.0f))); // Shop
    casinoObstacles.push_back(sf::FloatRect(sf::Vector2f(1300.0f, 335.0f), sf::Vector2f(270.0f, 260.0f))); // CardGame
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
// Event manager - closing the window and handling one-time key presses.
void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        // Closing the window
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // Recording the singular click.
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {

            // If we clicked escape when on gameover screen.
            if (keyEvent->code == sf::Keyboard::Key::Escape && currentState == GameState::GameOver) {
                // Casino hard reset.
                playerHp = 50;
                playerMoney = 65;
                daysSurvived = 1;
                casinoHeat = 0;
                heatThreshold = 50; // The first wave of enemies spawns if the heat reaches 50.
                shootTimer = 0.0f;

                // Clearing the enemies and projectiles
                if (gameObjects.size() > 1) {
                    gameObjects.erase(gameObjects.begin() + 1, gameObjects.end());
                }
                // Setting players position back to the middle.
                gameObjects[0]->setPosition(sf::Vector2f(800.0f, 446.0f));
                // Changing the gamestate.
                currentState = GameState::MainMenu;
                std::cout << "Game restarted. Good luck!" << std::endl;
            }
        }
    }
}

void Game::update(float dt) {
    shootTimer += dt;
    updateMusic(); // Updating the music according to the gamestate.


    if (currentState == GameState::MainMenu || currentState == GameState::Settings) {
        bool requestLoad = false;
        mainMenu.update(dt, window, shootTimer, currentState, requestLoad, globalVolume, assets);

        if (requestLoad) {
            loadGame(); // Loading the game if player clicked the button
        }
        return; // Blocking the update if we are in the menu
    }
    // Checking if escape is pressed atm.
    bool isEscPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);

    if (currentState == GameState::Hub || currentState == GameState::Survival) {
        // Going into pause only if player is in hub/survival and if escape wasnt already pressed (to leave roulette for example)
        if (isEscPressed && !wasEscPressed) {
            currentState = GameState::PauseMenu;
            std::cout << "Game Paused." << std::endl;
        }

        if (playerHp <= 0) {
            currentState = GameState::GameOver;
            gameOverReason = "Security beat you to a pulp!";
            std::cout << "You died! GAME OVER." << std::endl;
        }
        else if (playerMoney == 0 && currentState == GameState::Hub) {
            // Checking bankrupcy in hub/survival to prevent ending the game during the roulette spin.
            currentState = GameState::GameOver;
            gameOverReason = "You went completely bankrupt!";
            std::cout << "Bankrupt! GAME OVER." << std::endl;
        }


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

            // Randomly drawing the texture of projectile - card/chip

            // Setting chip texture as the deafult one.
            const sf::Texture* chosenTexture = &assets.chip;

            // 50% chance for changing the chip texture into card texture.
            if (rand() % 2 == 0) {
                chosenTexture = &assets.card;
            }

            if (rand() % 2 == 0) {
                assets.shootSound1.play();
            } else {
                assets.shootSound2.play();
            }

            // Creating a projectile with direction vector set to where our mouse is.
            gameObjects.push_back(std::make_unique<Projectile>(*chosenTexture, playerPos.x, playerPos.y, aimDirection));
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
                            enemy->takeDamage(playerDamage); // Projectile deals 10 damage
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

        // Ending the day after defeating the enemies and increasing the days survived counter.
        if (currentState == GameState::Survival) {
            bool enemiesAlive = false;

            // Looking for the enemy in the vector, to check if there is any left
            for (const auto& obj : gameObjects) {
                if (dynamic_cast<Enemy*>(obj.get()) != nullptr) {
                    enemiesAlive = true;
                    break; // If at least one is found, we break the loop.
                }
            }

            // If the loop confirmed that there are no enemies left.
            if (!enemiesAlive) {
                daysSurvived++; // Increasing days survived counter
                currentState = GameState::Hub; // Changing the gamestate.

                std::cout << "Wave cleared! You survived to see Day " << daysSurvived << ". Casino Heat relaxed." << std::endl;
            }
        }

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
        roulette.update(dt, window, shootTimer, playerMoney, casinoHeat, heatThreshold, currentState, gameObjects, playerHp, assets, daysSurvived, casinoObstacles);
    }
    else if (currentState == GameState::ShopUI) {
        shop.update(dt, window, shootTimer, playerMoney, currentState, playerHp, playerMaxHp, playerDamage, assets);
    }
    else if (currentState == GameState::CardGame) {
        cardGame.update(dt, window, shootTimer, playerMoney, casinoHeat, heatThreshold, currentState, gameObjects, playerHp, assets, daysSurvived, casinoObstacles);
    }
    else if (currentState == GameState::PauseMenu || currentState == GameState::PauseSettings) {
        // update reurns "true" if player clicks SAVE.
        bool requestSave = pauseMenu.update(dt, window, shootTimer, currentState, globalVolume, assets);
        if (requestSave) {
            saveGame();
        }
    }
    // Setting the flag preventing user going into pause menu right after leaving an event.
    wasEscPressed = isEscPressed;
}

void Game::render() {

    if (currentState == GameState::MainMenu || currentState == GameState::Settings) {
        mainMenu.render(window, currentState);
        window.display();
        return; // Not drawing the HUD in main menu.
    }

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
    }else if (currentState == GameState::GameOver) {
        // Drawing "Game over" background.
        window.draw(gameOverSprite);

        // Setting the text (hub font)
        sf::Text gameOverText(assets.hudFont);
        gameOverText.setString(gameOverReason + "\n\nYou survived " + std::to_string(daysSurvived) + " days.\n\nPress ESC to Restart");
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::White);

        // Setting the text in the middle of the screen.
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
        gameOverText.setPosition(sf::Vector2f(800.0f, 600.0f));

        window.draw(gameOverText);
    }

    // Drawing the HUD.
    std::string hudString = "HP: " + std::to_string(playerHp) +
                            " | MONEY: $" + std::to_string(playerMoney) +
                            " | DAY: " + std::to_string(daysSurvived) +
                            " | HEAT: " + std::to_string(casinoHeat) + "/" + std::to_string(heatThreshold);

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
    else if (currentState == GameState::ShopUI) {
        shop.render(window);
    }


    hudText.setString(hudString);
    window.draw(hudText); // Drawing the hud itself.



    // Drawing the pause at the end, on top of every sprite.
    if (currentState == GameState::PauseMenu || currentState == GameState::PauseSettings) {
        pauseMenu.render(window, currentState);
    }

    // Displaying.
    window.display();
}

// WARNING: The order of reading variables must perfectly match the order of writing them!
// Game pattern for save files and saving/loading them.
void Game::saveGame() {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << playerMoney << "\n";
        file << daysSurvived << "\n";
        file << playerHp << "\n";
        file << playerMaxHp << "\n";
        file << playerDamage << "\n";
        file << casinoHeat << "\n";
        file << heatThreshold << "\n";
        file.close();
        std::cout << "PROGRESS SAVED SUCCESSFULLY TO save.txt!" << std::endl;
    } else {
        std::cerr << "Error - Could not create save file!" << std::endl;
    }
}
// Loading the game from the file.
void Game::loadGame() {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        file >> playerMoney;
        file >> daysSurvived;
        file >> playerHp;
        file >> playerMaxHp;
        file >> playerDamage;
        file >> casinoHeat;
        file >> heatThreshold;
        file.close();
        std::cout << "PROGRESS LOADED SUCCESSFULLY!" << std::endl;
    } else {
        std::cerr << "Save file save.txt not found! Starting new round stats." << std::endl;
    }
}

void Game::updateMusic() {
    MusicTheme neededTheme;

    // Checking what music actual gamestate needs.
    if (currentState == GameState::MainMenu || currentState == GameState::Settings ||
        currentState == GameState::PauseMenu || currentState == GameState::PauseSettings ||
        currentState == GameState::GameOver)
    {
        neededTheme = MusicTheme::Menu;
    }
    else if (currentState == GameState::Hub || currentState == GameState::ShopUI) {
        neededTheme = MusicTheme::Hub;
    }
    else if (currentState == GameState::RouletteGame || currentState == GameState::CardGame) {
        neededTheme = MusicTheme::Minigame;
    }
    else if (currentState == GameState::Survival) {
        neededTheme = MusicTheme::Survival;
    }

    // If we need other music we change it.
    if (neededTheme != currentMusicTheme) {

        // Stopping every music playing
        assets.menuMusic.stop();
        assets.hubMusic.stop();
        assets.minigameMusic.stop();
        assets.survivalMusic.stop();

        // Playing the music we need.
        if (neededTheme == MusicTheme::Menu) assets.menuMusic.play();
        else if (neededTheme == MusicTheme::Hub) assets.hubMusic.play();
        else if (neededTheme == MusicTheme::Minigame) assets.minigameMusic.play();
        else if (neededTheme == MusicTheme::Survival) assets.survivalMusic.play();

        // Saving the current state.
        currentMusicTheme = neededTheme;
    }
}