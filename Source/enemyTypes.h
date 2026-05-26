#pragma once
#include "enemy.h"
#include "assetManager.h"
// Classes inheriting from the enemy class (base)
class FastEnemy : public Enemy {
public:
    FastEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
              const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived);
};

class TankEnemy : public Enemy {
public:
    TankEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
              const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived);
};

class BossEnemy : public Enemy {
public:
    BossEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
              const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived);
};