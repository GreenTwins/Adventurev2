#include <gtest/gtest.h>
#include "Game.h"

TEST(makePlayerTest, CreatePlayer){
       std::string testName= "Jubilee";
       Game &testGame = Game::getinstance();
       testGame.createPlayer(testName);
       EXPECT_EQ(testGame.playerN.getName(), "Jubilee");
}

