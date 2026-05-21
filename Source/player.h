#ifndef HIGHROLLERSSURVIVAL_PLAYER_H
#define HIGHROLLERSSURVIVAL_PLAYER_H


#include "gameObject.h"

class Player : public GameObject {
private:
    float speed;
    int health;
    int money;

    // Animation variables

    const sf::Texture* textureStanding1; // Standing texture
    const sf::Texture* textureRun1; // Move texture 1
    const sf::Texture* textureRun2; // Move texture 2
    float animationTimer; // Timer for changing the textures.
    bool isFrameOne; // Flag for distinguishing the movement.

public:
    Player(const sf::Texture& t1, const sf::Texture& t2,const sf::Texture& t3, float x, float y);

    void update(float dt) override; // Updating the player, inherited from gameobject
    void draw(sf::RenderWindow& window) override; // Updating the player, inherited from gameobject

    void addMoney(int amount); // Money interaction.
};

#endif //HIGHROLLERSSURVIVAL_PLAYER_H