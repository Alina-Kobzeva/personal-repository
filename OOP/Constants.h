#pragma once
#include <string>

namespace Constants {
    // Игровое поле
    inline constexpr int DEFAULT_FIELD_WIDTH = 16;
    inline constexpr int DEFAULT_FIELD_HEIGHT = 9;
    inline constexpr int FACTORY_X = 7;
    inline constexpr int FACTORY_Y = 4;
    inline constexpr int FACTORY_SIZE = 2;
    inline constexpr int FACTORY_SPAWN_INTERVAL = 3;

    // Границы инварианта поля
    inline constexpr int MIN_FIELD_SIZE = 5;
    inline constexpr int MAX_FIELD_SIZE = 30;

    // Стоимость перемещения по рельефу
    inline constexpr int COST_NORMAL_TERRAIN = 1;
    inline constexpr int COST_HEAVY_TERRAIN = 2;

    // Стоимость и регенерация энергии
    inline constexpr int ENERGY_COMBAT_COST = 2;
    inline constexpr int ENERGY_REGEN_PLAYER = 2;
    inline constexpr int ENERGY_REGEN_ENEMY = 2;

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

    // Телепортация
    inline constexpr int ENERGY_FOR_TELEPORTATION = 15;
    inline constexpr int TELEPORT_START_LEVEL = 1;
    inline constexpr int TELEPORT_START_RANGE = 5;
    inline constexpr int TELEPORT_ENERGY_UPDATE = 3;

    // Лечение
    inline constexpr int HEAL_START_LEVEL = 1;
    inline constexpr int HEAL_START_VALUE = 20;
    inline constexpr int HEAL_UPGRADE_STEP = 10;
    inline constexpr int ENERGY_FOR_TREATMENT = 10;
    
    // Дальняя атака
    inline constexpr int RANGED_START_LEVEL = 1;
    inline constexpr int RANGED_START_DAMAGE = 15;
    inline constexpr int RANGED_START_RANGE = 3;
    inline constexpr int RANGED_DAMAGE_UPGRADE = 5;
    inline constexpr int ENERGY_FOR_RANGED_ATTACK = 10;

    // Атака по площади
    inline constexpr int AOS_START_LEVEL = 1;
    inline constexpr int AOS_START_DAMAGE = 10;
    inline constexpr int AOS_RADIUS = 1;
    inline constexpr int ENERGY_FOR_AOS_ATTACK = 15;

    // Горение
    inline constexpr int BURN_TURNS = 3;
    inline constexpr int BURN_DAMAGE = 8;

    // Щит и замедление
    inline constexpr int SHIELD_POWER = 20;
    inline constexpr int SLOWDOWN_TURNS = 1;

    // Подсветка
    enum class RenderMode {
        Standard,
        Enemies,
        Walls,
        Statuses
    };

    inline const std::string COLOR_RESET = "\033[0m";
    inline const std::string COLOR_PLAYER = "\033[32m";
    inline const std::string COLOR_ENEMY = "\033[31m";
    inline const std::string COLOR_WALL = "\033[33m";
    inline const std::string COLOR_STATUS = "\033[35m";


}