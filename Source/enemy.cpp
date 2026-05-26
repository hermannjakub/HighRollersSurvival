#include "enemy.h"
#include <cmath>
#include <iostream>
// Enemy inheriting from gameobject.
Enemy::Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2,
             const sf::Texture& tAttack1, const sf::Texture& tAttack2,
             float x, float y, GameObject* targetPlayer, int* playerHpRef,
             float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown,
             const std::vector<sf::FloatRect>& obstaclesList)
    : GameObject(tRun1, x, y),
      textureRun1(&tRun1), textureRun2(&tRun2),
      textureAttack1(&tAttack1), textureAttack2(&tAttack2),
      target(targetPlayer), playerHp(playerHpRef), obstacles(obstaclesList),
      animationTimer(0.0f), isFrameOne(true), isAttacking(false), hp(enemyHp),
      damage(enemyDamage), attackCooldown(enemyAttackCooldown),
      attackTimer(enemyAttackCooldown), attackRange(60.0f)
{
    speed = enemySpeed;
    sprite.setOrigin(sf::Vector2f(static_cast<float>(tRun1.getSize().x) / 2.0f, static_cast<float>(tRun1.getSize().y) / 2.0f));
}

void Enemy::update(float dt) {
    if (!target || !playerHp) return;

    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f direction = targetPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    attackTimer += dt;
    animationTimer += dt;

    // Changing the texture for every 0.2 sec.
    if (animationTimer >= 0.2f) {
        animationTimer = 0.0f;
        isFrameOne = !isFrameOne;
    }

    if (distance <= attackRange) {
        // --- ATTACK MODE ---
        if (!isAttacking) isAttacking = true;

        // Attack animation
        sprite.setTexture(isFrameOne ? *textureAttack1 : *textureAttack2, true);

        if (attackTimer >= attackCooldown) {
            *playerHp -= damage;
            std::cout << "Enemy hits you! -" << damage << " HP. Current HP: " << *playerHp << std::endl;
            attackTimer = 0.0f;
        }
    } else {
        // --- RUNNING MODE ---
        if (isAttacking) isAttacking = false;

        sprite.setTexture(isFrameOne ? *textureRun1 : *textureRun2, true);

        direction.x /= distance;
        direction.y /= distance;

        // Collisions and movement of the enemy.
        // Movin in Y direction.
        sf::Sprite tempSpriteX = sprite;
        tempSpriteX.move(sf::Vector2f(direction.x * speed * dt, 0.0f));
        bool collisionX = false;
        for (const auto& obs : obstacles) {
            if (obs.findIntersection(tempSpriteX.getGlobalBounds())) { collisionX = true; break; }
        }
        if (!collisionX) position.x += direction.x * speed * dt;

        // Moving in Y direction.
        sf::Sprite tempSpriteY = sprite;
        tempSpriteY.move(sf::Vector2f(0.0f, direction.y * speed * dt));
        bool collisionY = false;
        for (const auto& obs : obstacles) {
            if (obs.findIntersection(tempSpriteY.getGlobalBounds())) { collisionY = true; break; }
        }
        if (!collisionY) position.y += direction.y * speed * dt;

        // Rotating the sprite.
        if (direction.x < 0) sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
        else sprite.setScale(sf::Vector2f(1.0f, 1.0f));
    }

    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) { window.draw(sprite); }

void Enemy::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        isDestroyed = true;
        std::cout << "Enemy eliminated!" << std::endl;
    }
}