#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

// Abstract class, basis of all elements/objects that are in the game, not used directly.

class GameObject {
protected:

    sf::Vector2f position; // Position of the object.
    sf::Sprite sprite; // Visuals of the object.

public:
    bool isDestroyed = false; // Flag for disappearing objects.
    GameObject(const sf::Texture& texture, float x, float y)
        : position(x, y), sprite(texture) {}


    virtual ~GameObject() = default; // Virtual destructor.
    virtual void update(float dt) = 0; // Updating the objects (hp,position)
    virtual void draw(sf::RenderWindow& window) = 0; // Drawing the objects.
    virtual void takeDamage(int dmg) {} // Modifying the HP.


    sf::Vector2f getPosition() const { return position; }

    virtual sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
};

#endif