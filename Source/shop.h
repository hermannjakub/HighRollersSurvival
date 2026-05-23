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

    // Hitboxy przycisków (+, -, BUY)
    sf::FloatRect hpPlusHitbox, hpMinusHitbox;
    sf::FloatRect healPlusHitbox, healMinusHitbox;
    sf::FloatRect dmgPlusHitbox, dmgMinusHitbox;
    sf::FloatRect buyHitbox;

    // Koszyk (ile kliknęliśmy plusów)
    int selectedHp;
    int selectedHeal;
    int selectedDmg;

    // Posiadane ulepszenia (do skalowania ceny)
    int ownedHpUpgrades;
    int ownedDmgUpgrades;

    // Elementy tekstowe
    sf::Text hpPriceText, hpQtyText;
    sf::Text healPriceText, healQtyText;
    sf::Text dmgPriceText, dmgQtyText;
    sf::Text totalCostText;
    sf::Text playerMoneyText;

    void initText(sf::Text& text, int size, sf::Color color);
    
    // Funkcje pomocnicze do obliczania cen
    int getHpCost();
    int getHealCost();
    int getDmgCost();
    int getTotalCost();
};