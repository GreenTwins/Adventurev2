#include <gtest/gtest.h>
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Skills.h"

/*TEST(makePlayerTest, CreatePlayer){
       std::string testName= "Jubilee";
       Game &testGame = Game::getinstance();
       testGame.createPlayer(testName);
       EXPECT_EQ(testGame.playerN.getName(), "Jubilee");
}*/
class DungeonBattleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any resources needed for the test
    }

    void TearDown() override {
        // Clean up any resources used in the test
    }
};

// A helper function to create a sample player with predefined stats and skills
Player createSamplePlayer() {

    Player player;  
    player.setName("Hero");
    player.setHP(100);
    player.setMP(50);
    player.setStr(4);
    player.setDef(3);
    player.setSpd(4);
    player.setDex(3);
    player.setEnD(5);
    player.setInt(2);
    player.compileAllStats();
    player.setClass("Warrior");
    player.setSubClass("Swordsmen");
    player.loadClassSkills();
    Skills WarriorSkill("Combo Rush", "Stamina", (rand() % 6 + 1), "Attack",":open handed attacks backed by pure innate strength ", 0, (rand() % 6 + 1), "AOE", 1, "Humanoid", "Melee Attack");
    Skills skill1("Lacerate", "MP", 12, "Attack", ": magical strength gathers into your blade to add extra dmg", 0, (rand() % 20 + 1), "Specific ",1, "Humanoid", "Magical Attack");   // Name, Type, Requirement, Attack Amount, Effect Amount
    Skills skill2("Piercing Strike", "Stamina", 1, "Attack", ": basic bladed attack", 0, (rand() % 6 + 1), "Specific",1, "Humanoid", "Melee Attack");     // Name, Type, Requirement, Attack Amount, Effect Amount
   

   player.addSkill(std::move(WarriorSkill));
   player.addSkill(std::move(skill1));
   player.addSkill(std::move(skill2));
    return player;
}

// A helper function to create a sample enemy with predefined stats and skills
Enemy createSampleEnemy() {
    
   Enemy enemy; 
   enemy.setName("Hero");
    enemy.setHP(100);
    enemy.setMP(50);
    enemy.setStr(4);
    enemy.setDef(3);
    enemy.setSpd(4);
    enemy.setDex(3);
    enemy.setEnD(5);
    enemy.setInt(2);
    enemy.implementStats(1);
   
    Skills WarriorSkill("Combo Rush", "Stamina", (rand() % 6 + 1), "Attack",":open handed attacks backed by pure innate strength ", 0, (rand() % 6 + 1), "AOE", 1, "Humanoid", "Melee Attack");
    Skills skill1("Lacerate", "MP", 12, "Attack", ": magical strength gathers into your blade to add extra dmg", 0, (rand() % 20 + 1), "Specific ",1, "Humanoid", "Magical Attack");   // Name, Type, Requirement, Attack Amount, Effect Amount
    Skills skill2("Piercing Strike", "Stamina", 1, "Attack", ": basic bladed attack", 0, (rand() % 6 + 1), "Specific",1, "Humanoid", "Melee Attack");     // Name, Type, Requirement, Attack Amount, Effect Amount
   

   enemy.loadSkills(std::move(WarriorSkill));
   enemy.loadSkills(std::move(skill1));
   enemy.loadSkills(std::move(skill2));    


    return enemy;
}

// Test case to check the DungeonBattle function
TEST_F(DungeonBattleTest, BattleSimulation) {
    Player player = createSamplePlayer();
    Enemy enemy = createSampleEnemy();
    Map map;

    std::unique_ptr<bool> result = map.DungeonBattle(player, enemy);
    
    // Check the result of the battle
    ASSERT_TRUE(result != nullptr);
    EXPECT_FALSE(*result);  // Assuming the enemy should lose in this case

    // Check the state of the player and enemy after the battle
    EXPECT_GT(player.getHP(), 0);
    EXPECT_LE(enemy.getHP(), 0);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
