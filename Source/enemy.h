#pragma once
#include "gameObject.h"
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
public:
    // Constructor styled exactly like the Player's constructor
    Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2, const sf::Texture& tAttack,
          float x, float y, GameObject* targetPlayer, int* playerHpRef,
          float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;


private:
    // Texture pointers (just like in Player)
    const sf::Texture* textureRun1;
    const sf::Texture* textureRun2;
    const sf::Texture* textureAttack;

    // Tracking the player
    GameObject* target;
    int* playerHp;

    // Animation variables (matching Player logic)
    float animationTimer;
    bool isFrameOne;
    bool isAttacking;

    // Enemy combat stats
    float speed;
    int damage;
    int hp;
    float attackCooldown;
    float attackTimer;
    float attackRange;
};