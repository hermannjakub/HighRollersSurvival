#include "cardGame.h"
#include <iostream>

CardGame::CardGame(AssetManager& assets)
    // Loading the assets (textures, sounds)
    : cardGameUISprite(assets.cardGameUI),
      playerCardSprite(assets.cardHidden),
      dealerCardSprite(assets.cardHidden),
      winLoseSprite(assets.winTexture)
{
    // Setting up the background UI for the card game
    cardGameUISprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Creating invisible hitboxes for buttons in UI, to be clickable by the player.
    higherHitbox = sf::FloatRect(sf::Vector2f(450.0f, 600.0f), sf::Vector2f(200.0f, 100.0f));
    lowerHitbox  = sf::FloatRect(sf::Vector2f(900.0f, 600.0f), sf::Vector2f(200.0f, 100.0f));
    playHitbox   = sf::FloatRect(sf::Vector2f(800.0f, 750.0f), sf::Vector2f(150.0f, 80.0f));

    // Setting up the result texture
    winLoseSprite.setOrigin(sf::Vector2f(assets.winTexture.getSize().x / 2.0f, assets.winTexture.getSize().y / 2.0f));
    winLoseSprite.setPosition(sf::Vector2f(800.0f, 446.0f));

    // Setting up card positions
    playerCardSprite.setPosition(sf::Vector2f(450.0f, 220.0f));
    dealerCardSprite.setPosition(sf::Vector2f(930.0f, 220.0f));

    // Setting initial values
    showResultSprite = false;
    revealTimer = 0.0f;
    isRevealed = false;

    startNewRound();
}

void CardGame::startNewRound() {
    // Drawing cards
    // Player: Jack (11), Queen (12), King (13)
    playerCardValue = 11 + (rand() % 3);

    // Dealer: 10, Jack (11), Queen (12), King (13), Ace (14)
    dealerCardValue = 10 + (rand() % 5);

    // Setting values for a new round - clearing bets and visuals, hiding the dealers card.
    betHigher = 0;
    betLower = 0;
    higherChipsVisuals.clear();
    lowerChipsVisuals.clear();
    isRevealed = false;
    showResultSprite = false;
}

void CardGame::setCardTexture(sf::Sprite& sprite, int value, AssetManager& assets) {
    // Setting the texture based on the card that has been drawed for both player and dealer.
    switch(value) {
        case 10: sprite.setTexture(assets.card10, true); break;
        case 11: sprite.setTexture(assets.cardJ, true); break;
        case 12: sprite.setTexture(assets.cardQ, true); break;
        case 13: sprite.setTexture(assets.cardK, true); break;
        case 14: sprite.setTexture(assets.cardA, true); break;
    }
}


void CardGame::update(float dt, sf::RenderWindow& window, float& shootTimer,
                      int& playerMoney, int& casinoHeat, int& heatThreshold, GameState& currentState,
                      std::vector<std::unique_ptr<GameObject>>& gameObjects,
                      int& playerHp, AssetManager& assets,
                      int& daysSurvived, const std::vector<sf::FloatRect>& casinoObstacles)
{
    // Going back to the casino and returning unplaced bets.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !isRevealed) {
        playerMoney += (betHigher + betLower);
        startNewRound();
        currentState = GameState::Hub;
        std::cout << "Leaving Card Game." << std::endl;
    }

    // Timer for showing the win/lose result
    if (showResultSprite) {
        revealTimer -= dt;
        if (revealTimer <= 0.0f) {
            startNewRound(); // Reseting the game after 2 seconds
        }
    }

    // Betting and clicking system
    if (!isRevealed && shootTimer >= 0.15f) {
        bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

        if (leftClicked || rightClicked) {
            shootTimer = 0.0f; // Resetting the cooldown

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // Getting coordinates of mouse position

            // Betting by leftclicking on the button (HIGHER button)
            if (higherHitbox.contains(mousePos)) {
                if (leftClicked && playerMoney >= 5) {
                    betHigher += 5; playerMoney -= 5;
                    sf::Sprite newChip(assets.chip); // Adding the visual chip when placed a bet.

                    newChip.setPosition(sf::Vector2f(
                        higherHitbox.position.x + (rand() % static_cast<int>(higherHitbox.size.x)),
                        higherHitbox.position.y + (rand() % 50)
                    ));
                    higherChipsVisuals.push_back(newChip);
                } else if (rightClicked && betHigher >= 5) {
                    betHigher -= 5; playerMoney += 5;
                    if (!higherChipsVisuals.empty()) higherChipsVisuals.pop_back(); // Deleting the visual
                }
            }
            // Betting when leftclicked (LOWER BUTTON)
            else if (lowerHitbox.contains(mousePos)) {
                if (leftClicked && playerMoney >= 5) {
                    betLower += 5; playerMoney -= 5;
                    sf::Sprite newChip(assets.chip); // Creating the visual sprite

                    newChip.setPosition(sf::Vector2f(
                        lowerHitbox.position.x + (rand() % static_cast<int>(lowerHitbox.size.x)),
                        lowerHitbox.position.y + (rand() % 50)
                    ));
                    lowerChipsVisuals.push_back(newChip);
                } else if (rightClicked && betLower >= 5) {
                    betLower -= 5; playerMoney += 5;
                    if (!lowerChipsVisuals.empty()) lowerChipsVisuals.pop_back(); // Deleting the visual
                }
            }
            // Revealing the cards (PLAY button)
            else if (playHitbox.contains(mousePos) && leftClicked) {
                if (betHigher > 0 || betLower > 0) {
                    isRevealed = true;
                    showResultSprite = true;
                    revealTimer = 2.0f; // Result will be visible for 2 seconds

                    bool isTie = (dealerCardValue == playerCardValue);
                    int totalBet = betHigher + betLower;

                    // Tie system (returning the money)
                    if (isTie) {
                        winLoseSprite.setColor(sf::Color::Transparent);
                        std::cout << "It's a tie! Money returned." << std::endl;
                        playerMoney += totalBet;

                    }
                    else {
                        int winnings = 0;
                        bool playerWon = false;

                        if (dealerCardValue > playerCardValue && betHigher > 0) {
                            winnings = betHigher * 2;
                            playerWon = true;
                        }
                        else if (dealerCardValue < playerCardValue && betLower > 0) {
                            winnings = betLower * 2;
                            playerWon = true;
                        }

                        if (playerWon) {
                            playerMoney += winnings;
                            std::cout << "You won: $" << winnings << " (Total bet was: $" << totalBet << ")" << std::endl;
                            winLoseSprite.setTexture(assets.winTexture, true);
                            assets.winSound.play(); // Sound trigger

                            casinoHeat += winnings;

                            // Spawning the enemies if the casino heat level reaches the threshold
                            if (casinoHeat >= heatThreshold) {
                                currentState = GameState::Survival;
                                std::cout << "The casino security is after you! Defend yourself!" << std::endl;

                                casinoHeat = 0; // Reseting the heat after the wave
                                heatThreshold += 25; // Increasing the threshold
                                // Three spawnpoints for the enemies, to make the game look realistic.
                                std::vector<sf::Vector2f> spawnPoints = {
                                    sf::Vector2f(20.0f, 779.0f),
                                    sf::Vector2f(1585.0f, 745.0f),
                                    sf::Vector2f(1585.0f, 236.0f)
                                };

                                // Amount of enemies starts at 2 and increases every 9 days
                                int enemiesToSpawn = 2 + (daysSurvived / 9);

                                // For-loop spawning the enemies
                                for (int i = 0; i < enemiesToSpawn; i++) {
                                    // We draw the position for every enemy separately
                                    sf::Vector2f spawnPos = spawnPoints[rand() % 3];
                                    int enemyType = rand() % 10;

                                    if (enemyType <= 5) {
                                        // Fast enemy (60% chance)
                                        gameObjects.push_back(std::make_unique<FastEnemy>(
                                            spawnPos.x, spawnPos.y, gameObjects[0].get(), &playerHp,
                                            casinoObstacles, assets, daysSurvived
                                        ));
                                    }
                                    else if (enemyType <= 8) {
                                        // Tank (30% chance)
                                        gameObjects.push_back(std::make_unique<TankEnemy>(
                                            spawnPos.x, spawnPos.y, gameObjects[0].get(), &playerHp,
                                            casinoObstacles, assets, daysSurvived
                                        ));
                                    }
                                    else {
                                        // Boss (10% chance)
                                        gameObjects.push_back(std::make_unique<BossEnemy>(
                                            spawnPos.x, spawnPos.y, gameObjects[0].get(), &playerHp,
                                            casinoObstacles, assets, daysSurvived
                                        ));
                                    }
                                }
                            }
                        } else {
                            std::cout << "You lost $" << totalBet << "!" << std::endl;
                            winLoseSprite.setTexture(assets.loseTexture, true);
                            assets.loseSound.play();
                            if (casinoHeat > 0) {
                                casinoHeat -= totalBet/2.5;
                            }
                        }
                    }
                }
            }
        }
    }
    setCardTexture(playerCardSprite, playerCardValue, assets);

    if (isRevealed) {
        setCardTexture(dealerCardSprite, dealerCardValue, assets); // Reveals the dealer's card
    } else {
        dealerCardSprite.setTexture(assets.cardHidden, true); // Hides under the question mark
    }
}

void CardGame::render(sf::RenderWindow& window) {
    // Drawing the background UI first
    window.draw(cardGameUISprite);

    // Drawing the cards
    window.draw(playerCardSprite);
    window.draw(dealerCardSprite);

    // Drawing the chips on the hitboxes
    for (const auto& chip : higherChipsVisuals) window.draw(chip);
    for (const auto& chip : lowerChipsVisuals) window.draw(chip);

    // Drawing the win/lose result screen
    if (showResultSprite) {
        window.draw(winLoseSprite);
    }
}