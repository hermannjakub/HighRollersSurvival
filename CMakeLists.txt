cmake_minimum_required(VERSION 3.20)
project(HighRollersSurvival)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")


find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(HighRollersSurvival Source/main.cpp Source/gameObject.h
        Source/player.h
        Source/player.cpp
        Source/projectile.h
        Source/projectile.cpp
        Source/game.h
        Source/game.cpp
        Source/enemy.h
        Source/enemy.cpp
        Source/assetManager.h
        Source/assetManager.cpp
        Source/gameState.h
        Source/roulette.h
        Source/roulette.cpp
        Source/cardGame.h
        Source/cardgame.cpp)


target_link_libraries(HighRollersSurvival SFML::Graphics SFML::Window SFML::System SFML::Audio)
