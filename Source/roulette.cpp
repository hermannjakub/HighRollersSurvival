#include "roulette.h"
#include <iostream>

Roulette::Roulette(AssetManager& assets)
    : wheelSprite(assets.wheel),
      pointerSprite(assets.pointer),
      rouletteUISprite(assets.rouletteUI),
      winLoseSprite(assets.winTexture),
      winSound(assets.winBuffer),
      loseSound(assets.loseBuffer)
{
    // Initialising the sprite of win texture
    winLoseSprite.setTexture(assets.winTexture, true);
    winLoseSprite.setOrigin(sf::Vector2f(static_cast<float>(assets.winTexture.getSize().x) / 2.0f, static_cast<float>(assets.winTexture.getSize().y) / 2.0f));
    winLoseSprite.setPosition(sf::Vector2f(800.0f, 446.0f)); // Middle of the screen (coordinates divided by 2)

    // Setting starting values of result sprites.
    resultDisplayTimer = 0.0f;
    showResultSprite = false;

    // Creating sprites and point of origin for the roulette to make it easier for rotating and positioning.
    wheelSprite.setTexture(assets.wheel, true);
    wheelSprite.setOrigin(sf::Vector2f(static_cast<float>(assets.wheel.getSize().x) / 2.0f, static_cast<float>(assets.wheel.getSize().y) / 2.0f));
    wheelSprite.setPosition(sf::Vector2f(400.0f, 446.0f)); // Left side of the screen

    pointerSprite.setTexture(assets.pointer, true);
    pointerSprite.setOrigin(sf::Vector2f(static_cast<float>(assets.pointer.getSize().x) / 2.0f, static_cast<float>(assets.pointer.getSize().y) / 2.0f));
    pointerSprite.setPosition(sf::Vector2f(400.0f, 510.0f - static_cast<float>(assets.wheel.getSize().y) / 2.0f - 20.0f)); // Over the wheel, to indicate the winning number.

    rouletteUISprite.setTexture(assets.rouletteUI, true); // Right side of the screen.
    // Setting the roulette UI position.
    rouletteUISprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Setting starting variables for roulette
    betRed = 0;
    betBlack = 0;
    betGreen = 0;
    isSpinning = false;
    spinSpeed = 0.0f;
    spinTimer = 0.0f;
}

void Roulette::update(float dt, sf::RenderWindow& window, float& shootTimer,
                      int& playerMoney, int& consecutiveWins, GameState& currentState,
                      std::vector<std::unique_ptr<GameObject>>& gameObjects,
                      int& playerHp, AssetManager& assets)
{
    // Roulette logic and withdrawing bets that have not been placed.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !isSpinning) {
        playerMoney += (betRed + betBlack + betGreen); // Giving back the money player didn't bet.
        betRed = 0; betBlack = 0; betGreen = 0; // Reseting the bets
        redChipsVisuals.clear(); blackChipsVisuals.clear(); greenChipsVisuals.clear(); // Clearing visual chips
        currentState = GameState::Hub;
        std::cout << "Withdrawing the bets, going back to the casino." << std::endl;
    }
    // Limiting the time for showing the result texture.
    if (showResultSprite) {
        resultDisplayTimer -= dt;
        if (resultDisplayTimer <= 0.0f) {
            showResultSprite = false;
        }
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

            // Button for starting the wheel with the bets set by the player.
            sf::FloatRect spinButton(sf::Vector2f(800.0f, 350.0f), sf::Vector2f(360.0f, 270.0f));

            // Setting bets and chips visuals on every roulette colour.
            // For every button: Leftclick - placing a bet, right click - withdrawing a bet.

            // Green button:
            if (greenButton.contains(mousePos)) {
                if (leftClicked && playerMoney >= 5) {
                    betGreen += 5;
                    playerMoney -= 5;
                    sf::Sprite newChip(assets.rouletteChip); // Creating a new sprite visual.
                    // Random placement of the chip on the button.
                    newChip.setPosition(sf::Vector2f(1150.0f + static_cast<float>(rand() % 220), 180.0f + static_cast<float>(rand() % 130)));
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
                    sf::Sprite newChip(assets.rouletteChip);

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
                    sf::Sprite newChip(assets.rouletteChip);

                    newChip.setPosition(sf::Vector2f(1150.0f + static_cast<float>(rand() % 220), 560.0f + static_cast<float>(rand() % 130)));
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

                // Showing the win effects (sound, texture)
                winSound.play();
                winLoseSprite.setTexture(assets.winTexture, true);
                showResultSprite = true;
                resultDisplayTimer = 2.0f; // The texture will be visible for 2 seconds

                std::cout << "You have got: " << consecutiveWins << " consecutive wins!"<< std::endl;

                if (consecutiveWins >= 2) { // If 2 consecutive wins, setting gamemode to survival and spawning the enemy.
                    currentState = GameState::Survival;
                    std::cout << "The casino security is after you! Defend yourself!" << std::endl;

                    gameObjects.push_back(std::make_unique<Enemy>(
                        assets.enemyFastRun1, assets.enemyFastRun2, assets.enemyFastAttack,
                        -50.0f, 400.0f, gameObjects[0].get(), &playerHp,
                        160.0f, 30, 10, 1.0f
                    ));
                }
            } else {

                // Showing the lose effects (sound, texture)
                loseSound.play();
                winLoseSprite.setTexture(assets.loseTexture, true);
                showResultSprite = true;
                resultDisplayTimer = 2.0f; // The texture will be visible for 2 seconds

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


void Roulette::render(sf::RenderWindow& window) {
    // When we are in the roullette mode, we draw only the ui, game is frozen.
    window.draw(rouletteUISprite);
    window.draw(wheelSprite);
    window.draw(pointerSprite);

    // Drawing the virtual chips on the buttons
    for (const auto& chip : greenChipsVisuals) window.draw(chip);
    for (const auto& chip : redChipsVisuals) window.draw(chip);
    for (const auto& chip : blackChipsVisuals) window.draw(chip);

    if (showResultSprite) {
        window.draw(winLoseSprite);
    }
}