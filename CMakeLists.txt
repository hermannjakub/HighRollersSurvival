cmake_minimum_required(VERSION 3.20)
project(HighRollersSurvival)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")


find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(HighRollersSurvival main.cpp gameObject.h
        player.h
        player.cpp
        projectile.h
        projectile.cpp
        game.h
        game.cpp)


target_link_libraries(HighRollersSurvival SFML::Graphics SFML::Window SFML::System SFML::Audio)
