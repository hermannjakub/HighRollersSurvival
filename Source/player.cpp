#include "player.h"



Player::Player(const sf::Texture& tStand, const sf::Texture& tRun1, const sf::Texture& tRun2, float x, float y)
    : GameObject(tStand, x, y), textureStanding1(&tStand), textureRun1(&tRun1), textureRun2(&tRun2), animationTimer(0.0f), isFrameOne(true)
{
    speed = 200.0f;
    health = 100;
    money = 0;
    // Setting the origin point to make the mirror reversing more smooth.
    sprite.setOrigin(sf::Vector2f(static_cast<float>(tStand.getSize().x) / 2.0f, static_cast<float>(tStand.getSize().y) / 2.0f));
}

void Player::update(float dt) {
    bool isMoving = false;
    sf::Vector2f oldPos = position;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { position.y -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { position.y += speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { position.x -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { position.x += speed * dt; isMoving = true; }


    // While pressing "A", the scale is set to -1 to make the character run the way we want.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        position.x -= speed * dt;
        isMoving = true;
        sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
    }

    // The opposite way for D.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        position.x += speed * dt;
        isMoving = true;
        sprite.setScale(sf::Vector2f(1.0f, 1.0f));
    }


    // Setting a margin to prevent legs/arms of the player to go through the border.
    float margin = 50.0f;

    // Collision with left and right ends of the map.
    if (position.x < margin) {
        position.x = margin;
    }
    else if (position.x > 1600.0f - margin) {
        position.x = 1600.0f - margin;
    }

    // Collision with upper and lower bounds of the map.
    if (position.y < margin + 100.0f) { // +100 to prevent the player from running on the wall at the top.
        position.y = margin + 100.0f;
    }
    else if (position.y > 893.0f - margin) {
        position.y = 893.0f - margin;
    }

    // Solid area that player can not go into for every playable object.
    sf::FloatRect solidRoulette(sf::Vector2f(30.0f, 230.0f), sf::Vector2f(220.0f, 400.0f));
    sf::FloatRect solidShop(sf::Vector2f(630.0f, 110.0f), sf::Vector2f(340.0f, 180.0f));
    sf::FloatRect solidSlots(sf::Vector2f(1350.0f, 335.0f), sf::Vector2f(250.0f, 225.0f));

    // If position of the player wants to go into any of objects (roulette, cardgames, shop)
    if (solidRoulette.contains(position) || solidShop.contains(position) || solidSlots.contains(position)) {
        position = oldPos; // Returning back to the old position - before interfering with any of the areas.
    }



    if (isMoving) {
        animationTimer += dt;

        if (animationTimer >= 0.2f) { // Changing the frame every 0.2 sec.
            animationTimer = 0.0f;
            isFrameOne = !isFrameOne;

            // Setting appropriate texture
            if (isFrameOne) {
                sprite.setTexture(*textureRun1);
            } else {
                sprite.setTexture(*textureRun2);
            }
        }
    } else {
        // If player stops moving, changing the texture
        sprite.setTexture(*textureStanding1);
        isFrameOne = true;
        animationTimer = 0.0f;
    }

    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}