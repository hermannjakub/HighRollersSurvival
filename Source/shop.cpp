#include "shop.h"
#include <iostream>

// Initialising the assets.
Shop::Shop(AssetManager& assets)
    : shopUISprite(assets.shopUI),
      hpPriceText(assets.hudFont),
      hpQtyText(assets.hudFont),
      healPriceText(assets.hudFont),
      healQtyText(assets.hudFont),
      dmgPriceText(assets.hudFont),
      dmgQtyText(assets.hudFont),
      totalCostText(assets.hudFont),
      playerMoneyText(assets.hudFont)
{
    shopUISprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Button hitboxes
    hpPlusHitbox   = sf::FloatRect(sf::Vector2f(222.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    hpMinusHitbox  = sf::FloatRect(sf::Vector2f(454.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    healPlusHitbox = sf::FloatRect(sf::Vector2f(675.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    healMinusHitbox= sf::FloatRect(sf::Vector2f(905.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    dmgPlusHitbox  = sf::FloatRect(sf::Vector2f(1121.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    dmgMinusHitbox = sf::FloatRect(sf::Vector2f(1355.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    buyHitbox      = sf::FloatRect(sf::Vector2f(654.0f, 766.0f), sf::Vector2f(352.0f, 104.0f));

    // Initialising the shopping card and owned upgrades.
    selectedHp = 0;
    selectedHeal = 0;
    selectedDmg = 0;
    ownedHpUpgrades = 0;
    ownedDmgUpgrades = 0;

    // Initialising the formatting of the text (size, colour)
    initText(hpPriceText, 30, sf::Color::Yellow);
    initText(hpQtyText, 40, sf::Color::White);

    initText(healPriceText, 30, sf::Color::Yellow);
    initText(healQtyText, 40, sf::Color::White);

    initText(dmgPriceText, 30, sf::Color::Yellow);
    initText(dmgQtyText, 40, sf::Color::White);

    initText(totalCostText, 40, sf::Color::Red);
    initText(playerMoneyText, 30, sf::Color::Green);

    // Positions of the texts (amounts, costs, total)
    hpPriceText.setPosition(sf::Vector2f(338.0f, 498.0f));
    hpQtyText.setPosition(sf::Vector2f(370.0f, 570.0f));

    healPriceText.setPosition(sf::Vector2f(788.0f, 498.0f));
    healQtyText.setPosition(sf::Vector2f(820.0f, 570.0f));

    dmgPriceText.setPosition(sf::Vector2f(1236.0f, 498.0f));
    dmgQtyText.setPosition(sf::Vector2f(1270.0f, 570.0f));

    totalCostText.setPosition(sf::Vector2f(750.0f, 695.0f));
}

// Additional function for the text.
void Shop::initText(sf::Text& text, int size, sf::Color color) {
    text.setCharacterSize(size);
    text.setFillColor(color);
}

// Price scaling depending on how many upgrades we own.
int Shop::getHpCost()   { return 50 + (ownedHpUpgrades * 25); }
int Shop::getHealCost() { return 30 + (ownedHpUpgrades * 10); }
int Shop::getDmgCost()  { return 100 + (ownedDmgUpgrades * 50); }

// Function getting the total cost to display to the player.
int Shop::getTotalCost() {
    return (selectedHp * getHpCost()) +
           (selectedHeal * getHealCost()) +
           (selectedDmg * getDmgCost());
}

void Shop::update(float dt, sf::RenderWindow& window, float& shootTimer,
                  int& playerMoney, GameState& currentState,
                  int& playerHp, int& playerMaxHp, int& playerDamage,
                  AssetManager& assets)
{
    // Going back to the casino
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        selectedHp = 0; selectedHeal = 0; selectedDmg = 0; // Emptying the cart when we come back.
        currentState = GameState::Hub;
    }

    if (shootTimer >= 0.15f && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        shootTimer = 0.0f; // Button cooldown.
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // HP logic
        if (hpPlusHitbox.contains(mousePos)) selectedHp++;
        else if (hpMinusHitbox.contains(mousePos) && selectedHp > 0) selectedHp--;

        // Healing logic
        if (healPlusHitbox.contains(mousePos)) selectedHeal++;
        else if (healMinusHitbox.contains(mousePos) && selectedHeal > 0) selectedHeal--;

        // Damage logic
        if (dmgPlusHitbox.contains(mousePos)) selectedDmg++;
        else if (dmgMinusHitbox.contains(mousePos) && selectedDmg > 0) selectedDmg--;

        // BUY button logic
        if (buyHitbox.contains(mousePos)) {
            int total = getTotalCost();

            if (total > 0 && playerMoney >= total) {
                playerMoney -= total;
                assets.shopSound.play(); // Buying sound

                // Applying the HP upgrade
                if (selectedHp > 0) {
                    ownedHpUpgrades += selectedHp;
                    playerMaxHp += (50 * selectedHp);
                    playerHp += (50 * selectedHp);
                }

                // Applying full heal
                if (selectedHeal > 0) {
                    playerHp = playerMaxHp;
                }

                // Applying DMG upgrade
                if (selectedDmg > 0) {
                    ownedDmgUpgrades += selectedDmg;
                    playerDamage += (5 * selectedDmg);
                }

                // Reseting the cart after buying
                selectedHp = 0;
                selectedHeal = 0;
                selectedDmg = 0;

                std::cout << "Purchase successful! Max HP: " << playerMaxHp << " | DMG: " << playerDamage << std::endl;
            } else if (total > 0) {
                assets.wrongButton.play(); // Error sound - not enough money.
                std::cout << "Not enough money!" << std::endl;
            }
        }
    }

    // Updating the text shown to the player.
    hpPriceText.setString("Cost: $" + std::to_string(getHpCost()));
    hpQtyText.setString(std::to_string(selectedHp));

    healPriceText.setString("Cost: $" + std::to_string(getHealCost()));
    healQtyText.setString(std::to_string(selectedHeal));

    dmgPriceText.setString("Cost: $" + std::to_string(getDmgCost()));
    dmgQtyText.setString(std::to_string(selectedDmg));

    totalCostText.setString("TOTAL: $" + std::to_string(getTotalCost()));

}

void Shop::render(sf::RenderWindow& window) {
    // Drawing the elements.
    window.draw(shopUISprite);

    window.draw(hpPriceText);
    window.draw(hpQtyText);

    window.draw(healPriceText);
    window.draw(healQtyText);

    window.draw(dmgPriceText);
    window.draw(dmgQtyText);

    window.draw(totalCostText);
    window.draw(playerMoneyText);
}