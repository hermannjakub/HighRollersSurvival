#pragma once
#include "gameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include "assetManager.h"

class Enemy : public GameObject {
public:
    Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2,
          const sf::Texture& tAttack1, const sf::Texture& tAttack2,
          float x, float y, GameObject* targetPlayer, int* playerHpRef,
          float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown,
          const std::vector<sf::FloatRect>& obstaclesList,
          AssetManager& assetsManager);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;

protected:
    const sf::Texture* textureRun1;
    const sf::Texture* textureRun2;
    const sf::Texture* textureAttack1;
    const sf::Texture* textureAttack2;

    AssetManager& assets;

    GameObject* target;
    int* playerHp;

    // Reference to the list of obstacles for the enemy to avoid it.
    const std::vector<sf::FloatRect>& obstacles;

    float animationTimer;
    bool isFrameOne;
    bool isAttacking;

    float speed;
    int damage;
    int hp;
    float attackCooldown;
    float attackTimer;
    float attackRange;

    int maxHp;
    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarForeground;
};