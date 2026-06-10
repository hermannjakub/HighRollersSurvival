#include "enemy.h"
#include <cmath>
#include <iostream>
// Enemy inheriting from gameobject.
Enemy::Enemy(const sf::Texture& tRun1, const sf::Texture& tRun2,
             const sf::Texture& tAttack1, const sf::Texture& tAttack2,
             float x, float y, GameObject* targetPlayer, int* playerHpRef,
             float enemySpeed, int enemyHp, int enemyDamage, float enemyAttackCooldown,
             const std::vector<sf::FloatRect>& obstaclesList,
             AssetManager& assetsManager)
    : GameObject(tRun1, x, y),
      textureRun1(&tRun1), textureRun2(&tRun2),
      textureAttack1(&tAttack1), textureAttack2(&tAttack2),
      target(targetPlayer), playerHp(playerHpRef), obstacles(obstaclesList),
       assets(assetsManager),
      animationTimer(0.0f), isFrameOne(true), isAttacking(false), hp(enemyHp),
      damage(enemyDamage), attackCooldown(enemyAttackCooldown),
      attackTimer(enemyAttackCooldown), attackRange(60.0f)
{
    speed = enemySpeed;
    sprite.setOrigin(sf::Vector2f(static_cast<float>(tRun1.getSize().x) / 2.0f, static_cast<float>(tRun1.getSize().y) / 2.0f));

    // Setting the starting hp as max hp.
    maxHp = enemyHp;

    // HP BAR CONFIGURATION
    // Bar background (dark grey)
    hpBarBackground.setSize(sf::Vector2f(50.0f, 6.0f));
    hpBarBackground.setFillColor(sf::Color(30, 30, 30, 200));
    hpBarBackground.setOrigin(sf::Vector2f(25.0f, 3.0f)); // the middle.

    // Bar filling (red)
    hpBarForeground.setSize(sf::Vector2f(50.0f, 6.0f));
    hpBarForeground.setFillColor(sf::Color(200, 20, 20, 220));
    // Origin is set to left side (0.0f), so the bar will get smaller from right to the left
    hpBarForeground.setOrigin(sf::Vector2f(0.0f, 3.0f));

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
            assets.playerDamaged.play();
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

    // HP BAR LOGIC
    // Setting the bar 65px above the enemy.
    hpBarBackground.setPosition(sf::Vector2f(position.x, position.y - 65.0f));

    // Fill must be moved 25px to the left, its origin was set on the border.
    hpBarForeground.setPosition(sf::Vector2f(position.x - 25.0f, position.y - 65.0f));

    // Calculating how many % of hp is left (Protecting with max to not fall below 0)
    float hpPercentage = std::max(0.0f, static_cast<float>(hp) / static_cast<float>(maxHp));

    // Changing the width of the bar
    hpBarForeground.setSize(sf::Vector2f(50.0f * hpPercentage, 6.0f));
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    // Drawing the bar only if enemy is alive.
    if (hp > 0) {
        window.draw(hpBarBackground);
        window.draw(hpBarForeground);
    }
}

void Enemy::takeDamage(int dmg) {
    assets.enemyDamaged.play();
    hp -= dmg;
    if (hp <= 0) {
        isDestroyed = true;
        std::cout << "Enemy eliminated!" << std::endl;
    }
}