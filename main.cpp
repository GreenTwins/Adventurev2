
#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <string>
//#include "../hiredis-1.2.0/hiredis-1.2.0/hiredis.h"
//#include "Player.h"
#include "Game.h"
#include "SQLCONN.h"
#include <crtdbg.h>
int main() {
	/*redisContext* context = redisConnect("redis-18805.c322.us-east-1-2.ec2.cloud.redislabs.com", 18805);
	if (context == NULL || context->err) {
		std::cerr << "Error connecting to Redis: " << context->errstr << std::endl;
		return 1;
	}
	std::cout << "Connected to Redis" << std::endl;
	redisFree(context);*/


	/*
	MainMenu& mainmenu = MainMenu::getInstance();
	SQLCONN& sqlconn = SQLCONN::createInstance();
	if (!mainmenu.display()) {
		return -1;
	}
	Game::getinstance().playerN.displayAllStats();
	if (Game::getinstance().newChar) {
		if (!sqlconn.saveAllData()) {
			return -1;
		}
	}*/
	Game& game = Game::getinstance();
	SQLCONN& sqlconn = SQLCONN::createInstance();
	sqlconn.LoadEnemies(1, 1, game.enemyList);

	for (auto enemy : game.enemyList) {
		std::cout << enemy.getName() << " uses ";
		std::cout << enemy.listofSkills[0].getSkillName() << "\n";
	}
	return 0;
}