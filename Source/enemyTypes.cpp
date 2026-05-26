#include "enemyTypes.h"

FastEnemy::FastEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
                     const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived)
    // Fast enemy, high velocity, low damage.
    : Enemy(assets.enemyFastRun1, assets.enemyFastRun2, assets.enemyFastAttack1, assets.enemyFastAttack2, 
            x, y, targetPlayer, playerHpRef, 
            160.0f + (daysSurvived * 5), 30 + (daysSurvived * 5), 10 + (daysSurvived * 2), 1.0f, obstaclesList) {}

TankEnemy::TankEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
                     const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived)
    // Tank: slow velocity, higher damage.
    : Enemy(assets.enemyTankRun1, assets.enemyTankRun2, assets.enemyTankAttack1, assets.enemyTankAttack2, 
            x, y, targetPlayer, playerHpRef, 
            130.0f + (daysSurvived * 2), 150 + (daysSurvived * 20), 20 + (daysSurvived * 4), 1.5f, obstaclesList) {}

BossEnemy::BossEnemy(float x, float y, GameObject* targetPlayer, int* playerHpRef, 
                     const std::vector<sf::FloatRect>& obstaclesList, AssetManager& assets, int daysSurvived)
    // Boss: Medium velocity, high damage.
    : Enemy(assets.enemyBossRun1, assets.enemyBossRun2, assets.enemyBossAttack1, assets.enemyBossAttack2, 
            x, y, targetPlayer, playerHpRef, 
            150.0f + (daysSurvived * 4), 300 + (daysSurvived * 40), 40 + (daysSurvived * 8), 2.0f, obstaclesList) {}