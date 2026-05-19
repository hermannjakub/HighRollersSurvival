#include "game.h"

Game::Game() :
    window(sf::VideoMode({1600, 893}), "High Roller's Survival"),
    backgroundSprite(backgroundTexture),
    wheelSprite(wheelTexture),
    pointerSprite(pointerTexture),
    rouletteUISprite(rouletteUITexture),
    hudText(hudFont)
{
    // Initialising the window size and framerate.
    window.setFramerateLimit(60);

    // Initialising the background texture.
    if (!backgroundTexture.loadFromFile("casino_map.png")) {
        std::cerr << "Error - Texture (casino_map.png) not found!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture, true);

    // Initialising player's standing texture.
    if (!playerStandingTexture.loadFromFile("playerStanding.png")) {
        std::cerr << "Error - Texture (playerStanding.png) not found!" << std::endl;
    }

    // Initialising player's moving texture 1.
    if (!playerMovingTexture1.loadFromFile("playerMove1.png")) {
        std::cerr << "Error - Texture (playerMove1.png) not found!" << std::endl;
    }

    // Initialising player's moving texture 2.
    if (!playerMovingTexture2.loadFromFile("playerMove2.png")) {
        std::cerr << "Error - Texture (playerMove2.png) not found!" << std::endl;
    }

    // Initialising chip texture ( projectile)
    if (!chipTexture.loadFromFile("poker_chip.png")) {
        std::cerr << "Error - Texture (poker_chip.png) not found!" << std::endl;
    }

    if (!rouletteChip.loadFromFile("roulette_chip.png")) {
        std::cerr << "Error - Texture (roulette_chip.png) not found!" << std::endl;
    }

    // Initialising roulette textures.
    if (!wheelTexture.loadFromFile("roulette_wheel.png") ||
        !pointerTexture.loadFromFile("win_pointer.png") ||
        !rouletteUITexture.loadFromFile("roulette_UI.png")) {
        std::cerr << "Error - Roulette textures (roulette_wheel.png, or win_pointer.png, or roulette_UI.png) NOT FOUND!" << std::endl;
    }

    // Creating sprites and point of origin for the roulette to make it easier for rotating and positioning.
    wheelSprite.setTexture(wheelTexture, true);
    wheelSprite.setOrigin(sf::Vector2f(static_cast<float>(wheelTexture.getSize().x) / 2.0f, static_cast<float>(wheelTexture.getSize().y) / 2.0f));
    wheelSprite.setPosition(sf::Vector2f(400.0f, 446.0f)); // Left side of the screen

    pointerSprite.setTexture(pointerTexture, true);
    pointerSprite.setOrigin(sf::Vector2f(static_cast<float>(pointerTexture.getSize().x) / 2.0f, static_cast<float>(pointerTexture.getSize().y) / 2.0f));
    pointerSprite.setPosition(sf::Vector2f(400.0f, 510.0f - static_cast<float>(wheelTexture.getSize().y) / 2.0f - 20.0f)); // Over the wheel, to indicate the winning number.

    rouletteUISprite.setTexture(rouletteUITexture, true); // Right side of the screen.
    // Setting the roulette UI position.
    rouletteUISprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Initialising the player.
    gameObjects.push_back(std::make_unique<Player>(playerStandingTexture, playerMovingTexture1, playerMovingTexture2, 400.0f, 300.0f));

    // Loading the font for the HUD
    if (!hudFont.openFromFile("retro_font.ttf")) {
        std::cerr << "Blad: Nie znaleziono pliku retro_font.ttf!" << std::endl;
    }

    hudText.setFont(hudFont);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(sf::Vector2f(20.0f, 20.0f)); // Fixing the hud position in top-left corner.

    // Setting starting variables
    shootTimer = 0.0f;
    currentState = GameState::Hub;
    consecutiveWins = 0;
    playerMoney = 200;
    daysSurvived = 1;
    playerHp = 100;
    betRed = 0;
    betBlack = 0;
    betGreen = 0;
    isSpinning = false;
    spinSpeed = 0.0f;
    spinTimer = 0.0f;
    playerBet = -1;
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

    if (currentState == GameState::Hub) {

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
            gameObjects.push_back(std::make_unique<Projectile>(chipTexture, playerPos.x, playerPos.y, aimDirection));
        }

        // Updating the elements of the game (walking, projectiles).
        for (auto& obj : gameObjects) {
            obj->update(dt);
        }

        // Definining invisible interactive zones, which are responsible for interactions with the shop and games.
        sf::FloatRect rouletteZone(sf::Vector2f(30.0f, 200.0f), sf::Vector2f(300.0f, 480.0f)); //Left side, where the roullette is located.
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
        // Roulette logic and withdrawing bets that have not been placed.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !isSpinning) {
            playerMoney += (betRed + betBlack + betGreen); // Giving back the money player didnt bet.
            betRed = 0; betBlack = 0; betGreen = 0; // Reseting the bets
            redChipsVisuals.clear(); blackChipsVisuals.clear(); greenChipsVisuals.clear(); // Clearing visual chips
            currentState = GameState::Hub;
            std::cout << "Withdrawing the bets, going back to the casino." << std::endl;
        }

        // Using the shoot timer as the clock for betting.
        if (!isSpinning  && shootTimer >= 0.15f) {
            bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            if (leftClicked || rightClicked) {
                shootTimer = 0.0f; // Reseting the clock to get the click cooldwn.

                // Getting the coursor position
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

                // Grouped coordinates of every betting ui button.
                sf::FloatRect redButton(sf::Vector2f(1150.0f,  370.0f), sf::Vector2f(270.0f, 180.0f));
                sf::FloatRect blackButton(sf::Vector2f(1150.0f,  540.0f), sf::Vector2f(270.0f, 180.0f));
                sf::FloatRect greenButton(sf::Vector2f(1150.0f,  200.0f), sf::Vector2f(270.0f, 180.0f));

                // Button for starting the wheel with the bets set by the player,.
                sf::FloatRect spinButton(sf::Vector2f(800.0f, 350.0f), sf::Vector2f(360.0f, 270.0f));

                // Setting bets and chips visuals on every roulette colour.
                // For every button: Leftclick - placing a bet, right click - withdrawing a bet.

                // Green button:
                if (greenButton.contains(mousePos)) {
                    if (leftClicked && playerMoney >= 5) {
                        betGreen += 5;
                        playerMoney -= 5;
                        sf::Sprite newChip(rouletteChip); // Creating a new sprite visual.
                        // Random placement of the chip on the button.
                        newChip.setPosition(sf::Vector2f(1150.0f + static_cast<float>(rand() % 220), 200.0f + static_cast<float>(rand() % 130)));
                        // Adding the chip to the container.
                        greenChipsVisuals.push_back(newChip);
                    } else if (rightClicked && betGreen >= 5) { // Withdrawing the bets
                        betGreen -= 5;
                        playerMoney += 5;
                        if (!greenChipsVisuals.empty()) greenChipsVisuals.pop_back();
                    }
                }
                // Red button:
                else if (redButton.contains(mousePos)) {
                    if (leftClicked && playerMoney >= 5) {
                        betRed += 5;
                        playerMoney -= 5;
                        sf::Sprite newChip(rouletteChip);

                        newChip.setPosition(sf::Vector2f(1150.0f + static_cast<float>(rand() % 220), 370.0f + static_cast<float>(rand() % 130)));
                        redChipsVisuals.push_back(newChip);
                    } else if (rightClicked && betRed >= 5) {
                        betRed -= 5;
                        playerMoney += 5;
                        if (!redChipsVisuals.empty()) redChipsVisuals.pop_back();
                    }
                }
                // Black button
                else if (blackButton.contains(mousePos)) {
                    if (leftClicked && playerMoney >= 5) {
                        betBlack += 5;
                        playerMoney -= 5;
                        sf::Sprite newChip(rouletteChip);

                        newChip.setPosition(sf::Vector2f(1150.0f + static_cast<float>(rand() % 220), 540.0f + static_cast<float>(rand() % 130)));
                        blackChipsVisuals.push_back(newChip);
                    } else if (rightClicked && betBlack >= 5) {
                        betBlack -= 5;
                        playerMoney += 5;
                        if (!blackChipsVisuals.empty()) blackChipsVisuals.pop_back();
                    }
                }
                else if (spinButton.contains(mousePos) && leftClicked) {
                    if (betRed > 0 || betBlack > 0 || betGreen > 0) {
                        isSpinning = true;
                        spinSpeed = 800.0f + static_cast<float>(rand() % 400);
                        spinTimer = 0.0f;
                        wheelSprite.setRotation(sf::degrees(0.0f));
                    }
                }
            }
        }

        if (isSpinning) {
            wheelSprite.rotate(sf::degrees(spinSpeed * dt)); // Animation of spinning
            spinSpeed -= 250.0f * dt;
            spinTimer += dt;

            if (spinSpeed <= 0.0f || spinTimer >= 3.5f) { // Ends the spinning if it spins more than 3.5 secs to make it dynamic.
                isSpinning = false;
                spinSpeed = 0.0f; // Stopping the wheel.

                float finalAngle = wheelSprite.getRotation().asDegrees();
                while (finalAngle >= 360.0f) finalAngle -= 360.0f; // Normalising the angles to make it calculable for every field.

                // Distinguishing the winning colour.
                int winningColor = -1;
                if ((finalAngle >= 355.0f || finalAngle < 5.0f) || (finalAngle >= 175.0f && finalAngle < 185.0f)) {
                    winningColor = 0;
                    std::cout << "Winner: GREEN!" << std::endl;
                } else {
                    int segmentIndex = static_cast<int>(finalAngle / 9.47f);
                    winningColor = (segmentIndex % 2 == 0) ? 1 : 2;
                    std::cout << "Winner: " << (winningColor == 1 ? "RED" : "BLACK") << "!" << std::endl;
                }

                // Updating the players money after the spin.
                int totalBetThisRound = betRed + betBlack + betGreen;
                int moneyWon = 0;

                if (winningColor == 0)      moneyWon = betGreen * 14; // Green pays x14
                else if (winningColor == 1) moneyWon = betRed * 2;     // Red pays x2
                else if (winningColor == 2) moneyWon = betBlack * 2;   // Black pays  x2

                playerMoney += moneyWon;

                // Defining if the round is won by the player (if he won more than he bet)
                if (moneyWon > totalBetThisRound) {
                    consecutiveWins++;
                    std::cout << "You have won: $" << moneyWon << " (You earned: +$" << (moneyWon - totalBetThisRound) << ")" << std::endl;
                    std::cout << "Hou have got: " << consecutiveWins << " consecutive wins!"<< std::endl;

                    if (consecutiveWins >= 2) {
                        currentState = GameState::Survival;
                        std::cout << "The casino security is after you! Defend yourself!" << std::endl;
                    }
                } else {
                    consecutiveWins = 0;
                    std::cout << "You have lost! The money you got back: $" << moneyWon << " (You have bet: $" << totalBetThisRound << ")." << std::endl;
                }

                // Clearing the bets for the next round.
                betRed = 0;
                betBlack = 0;
                betGreen = 0;
                redChipsVisuals.clear(); blackChipsVisuals.clear(); greenChipsVisuals.clear();
            }
        }
    }
    else if (currentState == GameState::ShopUI) {
        // Shop logic
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            currentState = GameState::Hub; // Exiting the shop
            std::cout << "Going back to the casino." << std::endl;
        }
    }
    else if (currentState == GameState::CardGame) {
        // Card game logic
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            currentState = GameState::Hub; // Exiting the card game
            std::cout << "Going back to the casino." << std::endl;
        }
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
        // When we are in the roullette mode, we draw only the ui, game is frozen.
        window.draw(rouletteUISprite);
        window.draw(wheelSprite);
        window.draw(pointerSprite);

        // Drawing the virtual chips on the buttons
        for (const auto& chip : greenChipsVisuals) window.draw(chip);
        for (const auto& chip : redChipsVisuals) window.draw(chip);
        for (const auto& chip : blackChipsVisuals) window.draw(chip);
    }

    // Drawing the HUD.
    std::string hudString = "HP: " + std::to_string(playerHp) +
                            " | MONEY: $" + std::to_string(playerMoney) +
                            " | DAY: " + std::to_string(daysSurvived);

    // Additional informations in the hud when we are in the roulette event.
    if (currentState == GameState::RouletteGame) {
        hudString += "\nCurrent bets -> Red: $" + std::to_string(betRed) +
                     " | Black: $" + std::to_string(betBlack) +
                     " | Green: $" + std::to_string(betGreen);
    }

    hudText.setString(hudString);
    window.draw(hudText); // Drawing the hud itself.

    // Displaying.
    window.display();
}