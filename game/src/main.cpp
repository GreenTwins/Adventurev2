
#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <string>
//#include "../hiredis-1.2.0/hiredis-1.2.0/hiredis.h"
//#include "Player.h"
#include "../include/Game.h"

#include <crtdbg.h>

int main() {

	//REDIS Cache load
	/*redisContext* context = redisConnect("redis-18805.c322.us-east-1-2.ec2.cloud.redislabs.com", 18805);
	if (context == NULL || context->err) {
		std::cerr << "Error connecting to Redis: " << context->errstr << std::endl;
		return 1;
	}
	std::cout << "Connected to Redis" << std::endl;
	redisFree(context);*/


	
	/*MainMenu& mainmenu = MainMenu::getInstance();
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

	//EnemyThreadSQL Load
	//Game& game = Game::getinstance();
	//SQLCONN& sqlconn = SQLCONN::createInstance();
	//sqlconn.LoadEnemies("Cave", 1, game.enemyList);

	/*for (auto enemy : game.enemyList) {
		std::cout << enemy.getName() << " "<< enemy.getHP()<<" uses ";
		std::cout << enemy.listofSkills[0].getSkillName() << "\n";
		
	}*/

	//BatteTest newBattle;

	//Game& game = Game::getinstance();
	//SQLCONN& sqlconn = SQLCONN::createInstance();

	//game.loadAllMissions(1);
	//
	//for (auto enemy : game.enemyList) {
	//		std::cout << enemy.getName() << " "<< enemy.getHP()<<" uses ";
	//		std::cout << enemy.listofSkills[0].getSkillName() << "\n";

	//}


	MainMenu &mmenu = MainMenu::getInstance();
	mmenu.display();

	return 0;
}