
#include "FlavianBattleTester.h"
#include <iostream>



BattleTest::BattleTest() {
	char playerLoadOption;
	char enemyLoadOption;
	int numOptions;
	int userChoice;
	bool exit_app = false;
	std::cout << "\nWelcome to the testing arena\n";
	
	while (!exit_app) {
		std::cout << "1.) Get Player \n";
		std::cout << "2.) Load Enemy \n";
		std::cout << "3.) Load Boss \n";
		numOptions = 3;
		if ((playerisLoaded) && (enemyisLoaded)) {
			std::cout << "4.) adjust player \n";
			std::cout << "5.) adjust enemy \n";
			std::cout << "6.) Battle \n";
			std::cout << "7.) Exit tester \n";
			numOptions = 7;
		}
		else {
			std::cout << "4.) Exit tester\n";
			numOptions = 4;
		}
		std::cout << "Choose how you want to proceed: \n";


		std::cin >> userChoice;
		if ((userChoice <= numOptions) || (userChoice >= 0)) {
			if (numOptions == 7) {
				exit_app = execute_request_unlocked(userChoice);
			}
			else {
				exit_app = execute_request_locked(userChoice);
			}
		}
		else {
			
		}
	}

	

}
bool BattleTest::execute_request_unlocked(int request) {
	bool exit = false;
	switch (request) {
	case 1:
		loadPlayer();
		break;
	case 2:
		loadEnemy();
		break;
	case 3:
		break;
	case 4: {
		int lvladjust{ 0 };
		int TEST_LvLLow = 1;
		int TEST_LvLHigh = 5;
		std::cout << "\nWhat level do you want to adjust to?: ";
		std::cin >> lvladjust;
		if ((lvladjust < TEST_LvLHigh) && (lvladjust > TEST_LvLLow)) {
			lvladjust = 1;
			std::cout << "\nRequested outside of params. Set to lvl 1\n";
		}
		adjPlStats(lvladjust);
		break;
		}
	case 5:
		break;
	case 6:
		Arena();
		break;
	case 7:
		exit = true;
		break;
	default:
		std::cout << "Reached failure\n";
		break;
	}
	
	return exit;
}

bool BattleTest::execute_request_locked(int request) {
	bool exit = false;
	char playerLoadOption;
	switch (request) {
	case 1: {
			std::cout << "Will you be loading a player? (Y/N): ";
			std::cin >> playerLoadOption;
			if (toupper(playerLoadOption) != 'Y') {
				std::cout << "\n We will load a dummy player in your stead\n";
				createPlayer();
			}
			else {
				if (!loadPlayer()) {
					std::cout << "\n We will load a dummy player in your stead\n";
					createPlayer();
				}
				else {
					std::cout << player.getName();
					}
				}
			}
		  playerisLoaded = true;
		break;
	case 2: {
		if (!loadEnemy()) {
			std::cout << "Try loading enemy again\n";
		}
		else {
			enemyisLoaded = true;
			std::cout << enemy.getName() << std::endl;
		}
		}
		 break;
	case 3:
		break;
	case 4:
		exit = true;
		break;
	default:
		std::cout << "Reached failure\n";
		break;
	}

	return exit;
}
void BattleTest::adjPlStats(int lvl) {
	float ptWorth = 8.33;
	bool accepted = false;
	do {
		int str = player.getStr();
		int def = player.getDef();
		int spd = player.getSpd();
		int intel = player.getInt();
		int dext = player.getDex();
		int end = player.getEnd();
	
		int total = str + def + spd + dext + intel + end;
		player.CustomizecreateStats(total);

	    int HPamt = (ptWorth * (player.getEnd()));
	    player.setHP(HPamt + 50);
	    player.setMaxHP(HPamt + 50);
	    int MPamt = (ptWorth * (player.getInt()));
	    player.setMP(MPamt + 50);
	    player.setMaxMp(MPamt + 50);
	    player.setFatigue(0);
	    float staminaamt = player.calculateStamin(player.getDex(), player.getEnd(),ptWorth);
	    
	    player.setStamina(10 + staminaamt);
	    player.setMaxStamina(player.getStamina());
	    int fatHolder = player.getFatigue();
	    float prec = player.calculatePrec(player.getStamina(), fatHolder, player.getMaxStamina(), player.getDex(), player.getInt());
	    
	    player.setPrec(prec);
	    player.setMaxPrec(prec);
	    player.setXP(0);
	    player.setLvl(1);
	    
	    player.setActiveJob("None");

		player.displayPrimaryStats();


		char updatestat; 
		std::cout << "\n Use these base stats?(Y/N):"; 
		std::cin >> updatestat;
		if (std::toupper(updatestat) == 'Y') {
			accepted = true;
		}
	} while (!accepted);

	if ((player.getLvl() > lvl)&&(lvl > 1)) {
		//reduce
		player.setStr(player.getStr() / lvl);
		player.setDef(player.getDef() / lvl);
		player.setSpd(player.getSpd() / lvl);
		player.setInt(player.getInt() / lvl);
		player.setDex(player.getDex() / lvl);
		player.setEnD(player.getEnd() / lvl);
		
		int HPamt = (ptWorth * (player.getEnd()));
		player.setHP(HPamt + 50);
		player.setMaxHP(HPamt + 50);
		int MPamt = (ptWorth * (player.getInt()));
		player.setMP(MPamt + 50);
		player.setMaxMp(MPamt + 50);
		player.setFatigue(0);
		float staminaamt = player.calculateStamin(player.getDex(), player.getEnd(), 8.33);

		player.setStamina(10 + staminaamt);
		player.setMaxStamina(player.getStamina());
		int fatHolder = player.getFatigue();
		float prec = player.calculatePrec(player.getStamina(), fatHolder, player.getMaxStamina(), player.getDex(), player.getInt());

		player.setPrec(prec);
		player.setMaxPrec(prec);
	}
}
bool BattleTest::loadPlayer() {
	SQLCONN& playerConn = SQLCONN::createInstance();
	Game& TESTING_game = Game::getinstance();
	playerConn.connect();
	playerisLoaded = false;
	if (!playerConn.displayNames()) {
		std::cout << " Nothing is on the database. Loading dummy player\n";
	}
	else {
		int choice;
		std::cout << "\n Which character would you like to load?:  ";
		std::cin >> choice;
		if (choice <= playerConn.playerList.size()) {
			//std::cout << playerConn.playerList[choice - 1] << std::endl;
			std::string name = playerConn.playerList[choice - 1];
			if (playerConn.loadPlayerData(name)) {
				playerisLoaded = true;
				player = std::move(TESTING_game.playerN);
			}
			
		}
		else {
			std::cout << "You didnt choose a given number\n";
		}
	}
	playerConn.disconnect();
	return playerisLoaded;
}

void BattleTest::createPlayer() {
	Game& createplayer = Game::getinstance();
	std::string demoChar;
	std::cout << "\n Name your practice character: ";
	std::cin >> demoChar;
	createplayer.createPlayer(demoChar);
	player = std::move(createplayer.playerN);

	playerisLoaded = true;
}

bool BattleTest::loadEnemy() {
	SQLCONN& enemyConn = SQLCONN::createInstance();
	Game& TESTING_game = Game::getinstance();
	enemyConn.connect();
	//std::vector<Enemy>enemyList;
	/*int dunLoc = 0;
	int dunNum = 0;*/
	std::string locationtype;
	int level;
	enemyisLoaded = false;
	std::cout << "Choose the dungeon location: ";
	std::cin >> locationtype;
	std::cout << "\n What level do you want the enemy to be?: ";
	std::cin >> level;
	
	enemyConn.LoadEnemies(locationtype, level,TESTING_game.enemyList);
	/*enemyConn.getEnemies(dunLoc, dunNum, TESTING_game.enemyList);
	enemyConn.getEnemySkills();*/
	if (TESTING_game.enemyList.size() > 0) {
		enemyisLoaded = true;
		int enemyChoice = 0;
		std::cout << "Which enemy do you choose to load?\n";
		int i = 1;
		for (auto& enemy : TESTING_game.enemyList) {
			std::cout << i << ".) Name: " << enemy.getName() << "\t Level: " << enemy.getLvl() << "\t Type: " << enemy.getBodyType() << "\n";
			++i;
		}
		std::cin >> enemyChoice;
		enemy = (enemyChoice > (TESTING_game.enemyList.size() - 1)) ?
			std::move(TESTING_game.enemyList[enemyChoice - 1]) :
			std::move(TESTING_game.enemyList[enemyChoice]);
	}
	else {
		std::cerr << "ENcountered an error loading enemy \n";
		return enemyisLoaded;
	}

	enemyConn.disconnect();
	return enemyisLoaded;
}
BattleTest::~BattleTest() {

}

void BattleTest::Arena() {
	//is this 1v1 or group?
	Player dupPlayer =std::move(player);
	Enemy* currEnemy = &enemy;
	std::string name = enemy.getName();
	auto it = name.find("Group");
	if (it != std::string::npos) {
		GroupBattle(dupPlayer,*currEnemy );
	}
	else {
		SoloBattle(dupPlayer, *currEnemy);
	}
	//this should be a a refesh
	player = std::move(dupPlayer);
	delete currEnemy;
}
void BattleTest::GroupBattle(Player& p , Enemy& e) {
	std::cout << "in group";

	//check speed first

	//check passives next

	//official fight 

	//loop
}
void BattleTest::SoloBattle(Player& p, Enemy& e) {
	//check speed first

	while ((p.getHP() > 0) || (e.getHP() > 0)) {
		if (p.getSpd() > e.getSpd()) {
			PlayerAttack(p, e);
		}
		else {
			EnemyAttack(p, e);
		}
	}
	if (p.getHP() > 0) {
		std::cout << p.getName() << " has defeated " << e.getName() << std::endl;
	}
	else {
		std::cout << "\n You've lost\n";
	}
	
	//check passives next

	//official fight 

	//loop
}
void BattleTest::PlayerAttack(Player& p, Enemy& e) {
	auto displaySkills = [&](const Skills& skill, int index) {
		std::cout << index << ".) " << skill.getSkillName() << ": " << "costs " << skill.getRequirementPayment() << " " << skill.getRequirementType() << "\n";
		std::cout << "Desc: " << skill.getSkillEffect() << " " << skill.getSkillEffectAmt();
		};

	for (int i = 0; i < p.listofSkills.size(); ++i) {
		displaySkills(p.listofSkills[i], i + i);
	}
}
void BattleTest::EnemyAttack(Player& p, Enemy& e) {

}