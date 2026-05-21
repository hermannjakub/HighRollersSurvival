#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameObject.h"

class Projectile : public GameObject {
private:
    sf::Vector2f direction;
    float speed;

public:
    Projectile(const sf::Texture& texture, float x, float y, sf::Vector2f dir);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif