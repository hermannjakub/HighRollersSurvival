#include "shop.h"
#include <iostream>

// --- POPRAWKA: Lista inicjalizacyjna ustawia domyślne tekstury i czcionki (SFML 3.0) ---
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

    // ==========================================
    // HITBOXY: Dopasuj do plusów, minusów i przycisku BUY ze swojego obrazka!
    // ==========================================
    hpPlusHitbox   = sf::FloatRect(sf::Vector2f(222.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    hpMinusHitbox  = sf::FloatRect(sf::Vector2f(454.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    healPlusHitbox = sf::FloatRect(sf::Vector2f(675.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    healMinusHitbox= sf::FloatRect(sf::Vector2f(905.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    dmgPlusHitbox  = sf::FloatRect(sf::Vector2f(1121.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));
    dmgMinusHitbox = sf::FloatRect(sf::Vector2f(1355.0f, 555.0f), sf::Vector2f(78.0f, 78.0f));

    buyHitbox      = sf::FloatRect(sf::Vector2f(654.0f, 766.0f), sf::Vector2f(352.0f, 104.0f));

    // Inicjalizacja koszyka i posiadanych ulepszeń
    selectedHp = 0;
    selectedHeal = 0;
    selectedDmg = 0;
    ownedHpUpgrades = 0;
    ownedDmgUpgrades = 0;

    // Inicjalizacja formatowania tekstu (czcionki są już przypisane wyżej!)
    initText(hpPriceText, 30, sf::Color::Yellow);
    initText(hpQtyText, 40, sf::Color::White);

    initText(healPriceText, 30, sf::Color::Yellow);
    initText(healQtyText, 40, sf::Color::White);

    initText(dmgPriceText, 30, sf::Color::Yellow);
    initText(dmgQtyText, 40, sf::Color::White);

    initText(totalCostText, 40, sf::Color::Red);
    initText(playerMoneyText, 30, sf::Color::Green);

    // ==========================================
    // POZYCJE TEKSTU: Dopasuj do okienek na grafice!
    // ==========================================
    hpPriceText.setPosition(sf::Vector2f(338.0f, 498.0f));
    hpQtyText.setPosition(sf::Vector2f(370.0f, 570.0f)); // Środkowe puste okienko

    healPriceText.setPosition(sf::Vector2f(788.0f, 498.0f));
    healQtyText.setPosition(sf::Vector2f(820.0f, 570.0f)); // Środkowe puste okienko

    dmgPriceText.setPosition(sf::Vector2f(1236.0f, 498.0f));
    dmgQtyText.setPosition(sf::Vector2f(1270.0f, 570.0f)); // Środkowe puste okienko

    totalCostText.setPosition(sf::Vector2f(750.0f, 695.0f)); // Długie okienko nad BUY
}

// Skrócona wersja funkcji pomocniczej (bo font ustawiamy na liście inicjalizacyjnej)
void Shop::initText(sf::Text& text, int size, sf::Color color) {
    text.setCharacterSize(size);
    text.setFillColor(color);
}

// Skalowanie cen
int Shop::getHpCost()   { return 50 + (ownedHpUpgrades * 25); }
int Shop::getHealCost() { return 30; }
int Shop::getDmgCost()  { return 100 + (ownedDmgUpgrades * 50); }

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
    // Powrót do Huba kasyna
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        selectedHp = 0; selectedHeal = 0; selectedDmg = 0; // Opróżniamy koszyk
        currentState = GameState::Hub;
    }

    if (shootTimer >= 0.15f && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        shootTimer = 0.0f; // Reset kliknięcia
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // --- Obsługa HP ---
        if (hpPlusHitbox.contains(mousePos)) selectedHp++;
        else if (hpMinusHitbox.contains(mousePos) && selectedHp > 0) selectedHp--;

        // --- Obsługa Leczenia ---
        if (healPlusHitbox.contains(mousePos)) selectedHeal++;
        else if (healMinusHitbox.contains(mousePos) && selectedHeal > 0) selectedHeal--;

        // --- Obsługa Obrażeń ---
        if (dmgPlusHitbox.contains(mousePos)) selectedDmg++;
        else if (dmgMinusHitbox.contains(mousePos) && selectedDmg > 0) selectedDmg--;

        // --- Obsługa Zakupu (BUY) ---
        if (buyHitbox.contains(mousePos)) {
            int total = getTotalCost();

            if (total > 0 && playerMoney >= total) {
                playerMoney -= total;
                assets.winSound.play(); // Dźwięk udanego zakupu

                // Aplikujemy HP Upgrade
                if (selectedHp > 0) {
                    ownedHpUpgrades += selectedHp;
                    playerMaxHp += (50 * selectedHp);
                    playerHp += (50 * selectedHp);
                }

                // Aplikujemy Pełne Leczenie
                if (selectedHeal > 0) {
                    playerHp = playerMaxHp;
                }

                // Aplikujemy DMG Upgrade
                if (selectedDmg > 0) {
                    ownedDmgUpgrades += selectedDmg;
                    playerDamage += (5 * selectedDmg);
                }

                // Reset koszyka po zakupie
                selectedHp = 0;
                selectedHeal = 0;
                selectedDmg = 0;

                std::cout << "Purchase successful! Max HP: " << playerMaxHp << " | DMG: " << playerDamage << std::endl;
            } else if (total > 0) {
                assets.loseSound.play(); // Dźwięk błędu - brak kasy
                std::cout << "Not enough money!" << std::endl;
            }
        }
    }

    // Aktualizacja wyświetlanego tekstu
    hpPriceText.setString("Cost: $" + std::to_string(getHpCost()));
    hpQtyText.setString(std::to_string(selectedHp));

    healPriceText.setString("Cost: $" + std::to_string(getHealCost()));
    healQtyText.setString(std::to_string(selectedHeal));

    dmgPriceText.setString("Cost: $" + std::to_string(getDmgCost()));
    dmgQtyText.setString(std::to_string(selectedDmg));

    totalCostText.setString("TOTAL: $" + std::to_string(getTotalCost()));

}

void Shop::render(sf::RenderWindow& window) {
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