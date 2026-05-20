#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

// Abstract class, basis of all elements/objects that are in the game, not used directly.

class GameObject {
protected:

    sf::Vector2f position;
    sf::Sprite sprite;

public:
    bool isDestroyed = false;
    GameObject(const sf::Texture& texture, float x, float y)
        : position(x, y), sprite(texture) {}


    virtual ~GameObject() = default;


    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void takeDamage(int dmg) {}


    sf::Vector2f getPosition() const { return position; }

    virtual sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
};

#endif