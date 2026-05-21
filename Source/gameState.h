#pragma once

// Separating events - setting a gamestate for every of them.
enum class GameState {
    Hub,            // Free roaming around the casino (no enemies)
    ShopUI,         // Shop
    RouletteGame,   // Roullete table
    CardGame,       // Card game
    Survival,       // Fight mode (when enemies are spawned)
    GameOver        // Game over (when player dies or goes bankrupt)
};