#include "projectile.h"
#include <cmath>

// Texture, starting position and direction.
Projectile::Projectile(const sf::Texture& texture, float x, float y, sf::Vector2f dir)
    : GameObject(texture, x, y), direction(dir), speed(600.0f)
{
    // Origin is set to make it look like it comes from the "middle" of the player
    sprite.setOrigin(sf::Vector2f(static_cast<float>(texture.getSize().x) / 2.0f, static_cast<float>(texture.getSize().y) / 2.0f));

    // Preventing the differences in speed basing on its direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction.x /= length;
        direction.y /= length;
    }
}

void Projectile::update(float dt) {
    // Projectile just follows a trajectory.
    position.x += direction.x * speed * dt;
    position.y += direction.y * speed * dt;

    sprite.setPosition(position);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}