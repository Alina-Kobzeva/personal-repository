#pragma once
#include <string>

namespace Constants {
    // Игровое поле
    inline constexpr int DEFAULT_FIELD_WIDTH = 16;
    inline constexpr int DEFAULT_FIELD_HEIGHT = 9;
    inline constexpr int FACTORY_X = 7;
    inline constexpr int FACTORY_Y = 4;
    inline constexpr int FACTORY_SIZE = 2;
    inline constexpr int FACTORY_SPAWN_INTERVAL = 5;

    // Границы инварианта поля
    inline constexpr int MIN_FIELD_SIZE = 5;
    inline constexpr int MAX_FIELD_SIZE = 30;

    // Стоимость перемещения по рельефу
    inline constexpr int COST_NORMAL_TERRAIN = 1;
    inline constexpr int COST_HEAVY_TERRAIN = 2;

    // Стоимость и регенерация энергии
    inline constexpr int ENERGY_COMBAT_COST = 2;
    inline constexpr int ENERGY_REGEN_PLAYER = 0;
    inline constexpr int ENERGY_REGEN_ENEMY = 0;

    // Стартовые характеристики робота
    const std::string PLAYER_NAME = "Маленький робот";
    inline constexpr int PLAYER_MAX_HP = 100;
    inline constexpr int PLAYER_MAX_ENERGY = 50;
    inline constexpr int PLAYER_ATTACK = 20;
    inline constexpr int PLAYER_SPEED = 3;
    inline constexpr int PLAYER_VISION = 1;

    // Стартовые характеристики врагов
    inline constexpr int ENEMY_MAX_HP = 40;
    inline constexpr int ENEMY_MAX_ENERGY = 20;
    inline constexpr int NEW_ENEMY_MAX_ENERGY = 25;
    inline constexpr int NEW_ENEMY_ATTACK = 15;
    inline constexpr int ENEMY_ATTACK = 15;
    inline constexpr int ENEMY_SPEED = 2;
    inline constexpr int ENEMY_VISION = 1;
}