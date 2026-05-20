#include "enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2, const sf::Texture& tAttack,
             float x, float y, GameObject* targetPlayer, int* playerHpRef,
             float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown)
    : GameObject(tRun1, x, y), // Inheriting initialization directly from GameObject!
      textureRun1(&tRun1), textureRun2(&tRun2), textureAttack(&tAttack),
      target(targetPlayer), playerHp(playerHpRef),
      animationTimer(0.0f), isFrameOne(true), isAttacking(false), hp(enemyHp),
      damage(enemyDamage), attackCooldown(enemyAttackCooldown),
      attackTimer(enemyAttackCooldown), // Ready to attack immediately when in range
      attackRange(60.0f)
{
    // Speed is inherited from GameObject
    speed = enemySpeed;

    // Setting the origin point to make the mirror reversing smooth (exactly like Player)
    sprite.setOrigin(sf::Vector2f(static_cast<float>(tRun1.getSize().x) / 2.0f, static_cast<float>(tRun1.getSize().y) / 2.0f));
}

void Enemy::update(float dt) {
    if (!target || !playerHp) return;

    // 1. Calculating distance to the player
    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f direction = targetPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    attackTimer += dt;

    // 2. Combat and Movement Logic
    if (distance <= attackRange) {
        // --- ATTACK MODE ---
        if (!isAttacking) {
            isAttacking = true;
            // 'true' flag ensures the size updates if the attack texture is larger/smaller than run textures
            sprite.setTexture(*textureAttack, true);
        }

        // Dealing damage if the cooldown has passed
        if (attackTimer >= attackCooldown) {
            *playerHp -= damage;
            std::cout << "Enemy hits you! -" << damage << " HP. Current HP: " << *playerHp << std::endl;
            attackTimer = 0.0f; // Reset cooldown
        }
    } else {
        // --- RUNNING MODE ---
        if (isAttacking) {
            isAttacking = false;
            // Returning to the running texture when player runs away
            sprite.setTexture(isFrameOne ? *textureRun1 : *textureRun2, true);
        }

        // Normalizing the vector so the speed is constant
        direction.x /= distance;
        direction.y /= distance;

        // Updating position towards the player
        position.x += direction.x * speed * dt;
        position.y += direction.y * speed * dt;

        // Mirror reversing the sprite depending on the walk direction
        if (direction.x < 0) {
            sprite.setScale(sf::Vector2f(-1.0f, 1.0f)); // Facing left
        } else {
            sprite.setScale(sf::Vector2f(1.0f, 1.0f));  // Facing right
        }

        // Animation logic (exactly like in Player texture)
        animationTimer += dt;
        if (animationTimer >= 0.2f) { // Changing the frame every 0.2 sec.
            animationTimer = 0.0f;
            isFrameOne = !isFrameOne;

            // Setting appropriate texture
            if (isFrameOne) {
                sprite.setTexture(*textureRun1, true);
            } else {
                sprite.setTexture(*textureRun2, true);
            }
        }
    }

    // Applying calculated position to the sprite
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    // Only drawing the sprite, just like Player
    window.draw(sprite);
}

    void Enemy::takeDamage(int dmg) {
        hp -= dmg;
        std::cout << "You have hit the enemy! He has : " << hp << " HP left" << std::endl;

        if (hp <= 0) {
            isDestroyed = true; // Marking the enemy that was killed and is meant to be erased.
            std::cout << "You have killed the enemy!" << std::endl;
        }
    }

