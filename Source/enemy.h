#pragma once
#include "gameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy : public GameObject {
public:
    // Dodano drugą klatkę ataku i wektor przeszkód
    Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2,
          const sf::Texture& tAttack1, const sf::Texture& tAttack2,
          float x, float y, GameObject* targetPlayer, int* playerHpRef,
          float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown,
          const std::vector<sf::FloatRect>& obstaclesList);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;

protected: // Zmieniono na protected dla klas pochodnych
    const sf::Texture* textureRun1;
    const sf::Texture* textureRun2;
    const sf::Texture* textureAttack1;
    const sf::Texture* textureAttack2;

    GameObject* target;
    int* playerHp;

    // Referencja do listy stołów w kasynie, żeby wróg wiedział, co omijać
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
};