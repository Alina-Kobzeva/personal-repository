#include <gtest/gtest.h>
#include "Robot.h"
#include "Cell.h"
#include "GameField.h"
#include "SlowdownStatus.h"
#include "OverloadStatus.h"
#include "BurnStatus.h"
#include "ShieldStatus.h" 


// Горение
TEST(StatusTest, BurnStatusAppliesDamageAndMerges) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.addStatus(std::make_unique<BurnStatus>(3, 8));
    EXPECT_EQ(bot.getActiveStatuses().size(), 1);
    
    bot.tickStatuses();
    EXPECT_EQ(bot.getHealth(), 92);
    EXPECT_EQ(bot.getActiveStatuses()[0]->getDuration(), 2);
    
    bot.addStatus(std::make_unique<BurnStatus>(3, 8));
    EXPECT_EQ(bot.getActiveStatuses().size(), 1);
    EXPECT_EQ(bot.getActiveStatuses()[0]->getDuration(), 3);
}

// Щит
TEST(StatusTest, ShieldStatusGetDamageAndAccumulates) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.addStatus(std::make_unique<ShieldStatus>(20));
    
    bot.takeDamage(15);
    EXPECT_EQ(bot.getHealth(), 100);
    EXPECT_EQ(bot.getActiveStatuses()[0]->getDuration(), 5);
    
    bot.addStatus(std::make_unique<ShieldStatus>(15));
    EXPECT_EQ(bot.getActiveStatuses()[0]->getDuration(), 20);
    
    bot.takeDamage(35);
    EXPECT_EQ(bot.getHealth(), 85);
    bot.tickStatuses();
    EXPECT_TRUE(bot.getActiveStatuses().empty());
}

// Замедление
TEST(StatusTest, SlowdownStatusBlocksMovement) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.addStatus(std::make_unique<SlowdownStatus>(1));
    
    EXPECT_FALSE(bot.getActiveStatuses()[0]->canMove());
    
    bot.tickStatuses();
    EXPECT_EQ(bot.getActiveStatuses().size(), 0); 
}

// Перегрузка
TEST(StatusTest, OverloadStatusBlocksAbilitiesAndExpiresOnAttempt) {
    Robot bot("Тест-Бот", 100, 50, 20, 3, 1, false);
    
    bot.addStatus(std::make_unique<OverloadStatus>());
    
    EXPECT_FALSE(bot.getActiveStatuses()[0]->canUseAbilities());
    
    bot.getActiveStatuses()[0]->onAbilityAttempt();
    
    bot.tickStatuses();
    EXPECT_TRUE(bot.getActiveStatuses().empty());
}

// Проверка перевода избыточного лечения в Щит
TEST(AbilityInteractionTest, TreatmentTriggersOverhealShield) {
    Robot realPlayer("Маленький робот", 100, 50, 20, 3, 1, false);
    
    realPlayer.takeDamage(10); 
    EXPECT_EQ(realPlayer.getHealth(), 90);
    
    int currentHp = realPlayer.getHealth();
    int maxHp = realPlayer.getMaxHealth();
    int missingHp = maxHp - currentHp;
    int healValue = 20;
    
    if (healValue > missingHp) {
        int overhealAmount = healValue - missingHp;
        realPlayer.heal(missingHp);
        realPlayer.addStatus(std::make_unique<ShieldStatus>(overhealAmount));
    }
    
    EXPECT_EQ(realPlayer.getHealth(), 100);
    EXPECT_EQ(realPlayer.getActiveStatuses()[0]->getDuration(), 10);
}


// Накопления ловушек и расход зарядов
TEST(FieldInteractionTest, CellStoresStatusesAndLosesChargeOnStep) {
    Cell cell;
    
    cell.addStatus(std::make_unique<BurnStatus>(3, 8));
    cell.addStatus(std::make_unique<SlowdownStatus>(1));
    
    EXPECT_TRUE(cell.hasStatuses());
    EXPECT_EQ(cell.getCellStatuses().size(), 2);
    
    Robot playerBot("Тест-Робот", 100, 50, 20, 3, 1, false);
    
    for (auto& status : cell.getCellStatuses()) {
        if (status->getName().find("Замедление") != std::string::npos) {
            playerBot.addStatus(std::make_unique<SlowdownStatus>(1));
        }
        else if (status->getName().find("Горение") != std::string::npos) {
            playerBot.addStatus(std::make_unique<BurnStatus>(3, 8));
        }
    }
    
    cell.removeOneRandomStatus();
    
    EXPECT_EQ(playerBot.getActiveStatuses().size(), 2); 
    EXPECT_EQ(cell.getCellStatuses().size(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
