#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

// Abstract class, basis of all elements/objects that are in the game, not used directly.

class GameObject {
protected:

    sf::Vector2f position;
    sf::Sprite sprite;

public:
    GameObject(const sf::Texture& texture, float x, float y)
        : position(x, y), sprite(texture) {}


    virtual ~GameObject() = default;


    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;


    sf::Vector2f getPosition() const { return position; }
};

#endif