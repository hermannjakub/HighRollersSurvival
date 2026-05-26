#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "assetManager.h"
#include "gameState.h"

class Shop {
public:
    Shop(AssetManager& assets);

    void update(float dt, sf::RenderWindow& window, float& shootTimer,
                int& playerMoney, GameState& currentState,
                int& playerHp, int& playerMaxHp, int& playerDamage,
                AssetManager& assets);

    void render(sf::RenderWindow& window);

private:
    sf::Sprite shopUISprite;

    // Button hitboxes (+,-,BUY)
    sf::FloatRect hpPlusHitbox, hpMinusHitbox;
    sf::FloatRect healPlusHitbox, healMinusHitbox;
    sf::FloatRect dmgPlusHitbox, dmgMinusHitbox;
    sf::FloatRect buyHitbox;

    // "Shopping cart" - how many items we want to buy.
    int selectedHp;
    int selectedHeal;
    int selectedDmg;

    // Owned upgrades ( to increase prices )
    int ownedHpUpgrades;
    int ownedDmgUpgrades;

    // Text fields (costs, total etc.)
    sf::Text hpPriceText, hpQtyText;
    sf::Text healPriceText, healQtyText;
    sf::Text dmgPriceText, dmgQtyText;
    sf::Text totalCostText;
    sf::Text playerMoneyText;

    void initText(sf::Text& text, int size, sf::Color color);
    
    // Getters (to calculate prices)
    int getHpCost();
    int getHealCost();
    int getDmgCost();
    int getTotalCost();
};