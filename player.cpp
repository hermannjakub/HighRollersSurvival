#include "Player.h"


Player::Player(const sf::Texture& t1, const sf::Texture& t2, float x, float y)
    : GameObject(t1, x, y), texture1(&t1), texture2(&t2), animationTimer(0.0f), isFrameOne(true)
{
    speed = 200.0f;
    health = 100;
    money = 0;
}

void Player::update(float dt) {
    bool isMoving = false;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { position.y -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { position.y += speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { position.x -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { position.x += speed * dt; isMoving = true; }


    if (isMoving) {
        animationTimer += dt;

        if (animationTimer >= 0.2f) { // Changing the frame every 0.2 sec.
            animationTimer = 0.0f;
            isFrameOne = !isFrameOne;

            // Ustawiamy odpowiednią teksturę
            if (isFrameOne) {
                sprite.setTexture(*texture1);
            } else {
                sprite.setTexture(*texture2);
            }
        }
    } else {
        // If player stops moving, changing the texture
        sprite.setTexture(*texture1);
        isFrameOne = true;
        animationTimer = 0.0f;
    }

    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}