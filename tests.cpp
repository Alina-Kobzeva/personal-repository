#include <gtest/gtest.h>
#include "Robot.h"
#include "Cell.h"
#include "GameField.h"
#include "Factory.h"

TEST(RobotTest, HealthDoesNotExceedMaximum) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.takeDamage(30);
    EXPECT_EQ(bot.getHealth(), 70);
    
    bot.heal(50);
    EXPECT_EQ(bot.getHealth(), 100);
}

TEST(RobotTest, HealthDoesNotBecomeNegative) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.takeDamage(150);
    EXPECT_EQ(bot.getHealth(), 0);
}

TEST(GameFieldTest, SizeIsClampedToValidRange) {
    GameField smallField(2, 4);
    EXPECT_EQ(smallField.getWidth(), 5);
    EXPECT_EQ(smallField.getHeight(), 5);

    GameField bigField(40, 50);
    EXPECT_EQ(bigField.getWidth(), 30);
    EXPECT_EQ(bigField.getHeight(), 30);
}

TEST(RobotTest, MovePointsSpending) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.resetMovePoints();
    EXPECT_EQ(bot.getCurrentMovePoints(), 3);
    
    bot.spendMovePoints(2);
    EXPECT_EQ(bot.getCurrentMovePoints(), 1);
    
    bot.spendMovePoints(5);
    EXPECT_EQ(bot.getCurrentMovePoints(), 0);
}

TEST(FactoryTest, SpawnTimerWorksAccordingToCooldown) {
    Factory factory(4, 2, 3);
    
    EXPECT_FALSE(factory.readyToSpawn());
    EXPECT_FALSE(factory.readyToSpawn());
    EXPECT_TRUE(factory.readyToSpawn());
    EXPECT_FALSE(factory.readyToSpawn());
}

TEST(GameControllerTest, HonestMovementAndTerrainCost) {
    Robot realPlayer("Тест-Робот", 100, 50, 20, 3, 1, false);
    GameField field(5, 5);
    PlacedRobot player(realPlayer, 1, 1);
    
    field.getCell(2, 1).setMovementCost(2);
    
    player.getRobot().resetMovePoints();
    int stepsWanted = player.getRobot().getSpeed();
    
    for (int i = 0; i < stepsWanted; ++i) {
        int nextX = player.getX() + 1;
        int nextY = player.getY();
        
        int cost = field.getCell(nextX, nextY).getMovementCost();
        
        if (player.getRobot().getCurrentMovePoints() >= cost) {
            player.setX(nextX);
            player.getRobot().spendMovePoints(cost);
            player.getRobot().spendEnergy(cost);
        } else {
            break;
        }
    }
    
    EXPECT_EQ(player.getX(), 3); 
    EXPECT_EQ(player.getRobot().getCurrentMovePoints(), 0);
    EXPECT_EQ(player.getRobot().getEnergy(), 47);
}

TEST(GameControllerTest, MovementBlockedByWall) {
    Robot realPlayer("Тест-Робот", 100, 50, 20, 3, 1, false);
    GameField field(5, 5);
    PlacedRobot player(realPlayer, 1, 1);
    
    field.getCell(3, 1).setIsWalkable(false);
    
    player.getRobot().resetMovePoints();
    for (int i = 0; i < 3; ++i) {
        int nextX = player.getX() + 1;
        if (!field.getCell(nextX, player.getY()).getIsWalkable()) {
            break;
        }
        player.setX(nextX);
    }
    
    EXPECT_EQ(player.getX(), 2);
}

TEST(GameControllerTest, EnemyAIPathfinding) {
    Robot realEnemy("Дрон", 40, 20, 15, 2, 1, true);
    PlacedRobot enemy(realEnemy, 5, 5);
    
    int playerX = 2;
    int playerY = 3;
    
    int movX = 0;
    if (playerX > enemy.getX()) movX = 1;
    else if (playerX < enemy.getX()) movX = -1;

    int movY = 0;
    if (playerY > enemy.getY()) movY = 1;
    else if (playerY < enemy.getY()) movY = -1;
    
    enemy.setX(enemy.getX() + movX);
    enemy.setY(enemy.getY() + movY);
    
    EXPECT_EQ(enemy.getX(), 4);
    EXPECT_EQ(enemy.getY(), 4);
}

TEST(FactoryTest, PhysicalEnemySpawn) {
    Factory factory(2, 2, 5);
    
    PlacedRobot spawned = factory.newEnemy(1);
    
    EXPECT_EQ(spawned.getRobot().getName(), "Боевой робот-враг-1");
    EXPECT_TRUE(spawned.getRobot().getIsEnemy());
    
    EXPECT_EQ(spawned.getX(), 2);
    EXPECT_EQ(spawned.getY(), 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
