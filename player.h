#ifndef HIGHROLLERSSURVIVAL_PLAYER_H
#define HIGHROLLERSSURVIVAL_PLAYER_H


#include "GameObject.h"

class Player : public GameObject {
private:
    float speed;
    int health;
    int money;

    // Animation variables

    const sf::Texture* texture1; // First frame indicator
    const sf::Texture* texture2; // Second frame indicator
    float animationTimer;
    bool isFrameOne;

public:
    Player(const sf::Texture& t1, const sf::Texture& t2, float x, float y);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void addMoney(int amount);
};

#endif //HIGHROLLERSSURVIVAL_PLAYER_H