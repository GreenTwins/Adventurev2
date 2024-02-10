#include "Game.h"
#include <iostream>
#include <random>
#include <string>
#include "SQLCONN.h"
/******************************************************************************************************
MAP CLASS init,cleaner, getters and setters

*******************************************************************************************************/
Map::Map() {
	//loadMapData();
	weaponXp = 0;
	weaponlvl = 0;
	totalGold = 0;
	totalXP = 0;
	//findXpCap(Game::getinstance().playerN.getLvl());
}
Map::~Map() {}

/******************************************************************************************************
MAP CLASS deterministic functions: creates maps based on certain flags and data inherited from player (lvl,
currentLocation)

*******************************************************************************************************/
//int 
std::unique_ptr<int>determinenextconnectedPath(int path, int locationSize) {

	std::map<int, int>& checker=Game::getinstance().nextPathTracker;

	if (checker.find(path) != checker.end() && checker[path] > 3) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distrib(0, locationSize - 1);
		return determinenextconnectedPath(distrib(gen), locationSize);
	}

	checker[path]++;
	return std::make_unique<int>(path);

}
void Map::upDateTracker() {
	if (!checkVisited(currentPlacement)) {
		addEdge(currentPlacement, nextPlacement);
	}
}
bool Map::checkVisited(int current) {
	q.push(1);
	visited[1] = 1;
	while (!q.empty()) {
		int curr = q.front();
		q.pop();
		/*	if (curr == current) {
				return true;
			}*/
		for (auto neigh : MoveTracker[curr]) {
			if (visited[neigh] != 1) {
				q.push(neigh);
				visited[neigh] = 1;
			}
		}

	}
	return false;
}
void Map::addEdge(int u, int v) {
	MoveTracker[u].push_back(v);
	MoveTracker[v].push_back(u);
}
void Map::displayTracker() {
	std::queue<int>tempq;
	std::map<int, int>vis;

	tempq.push(1);
	visited[1] = 1;
	while (!tempq.empty()) {
		int curr = tempq.front();
		tempq.pop();
		std::cout << curr << "\t";

		for (auto neighbor : MoveTracker[curr]) {
			std::cout << neighbor << std::endl;
			if (vis[neighbor] != 1) {
				tempq.push(neighbor);
				vis[neighbor] = 1;
			}
		}
	}
}
void Map::createPaths(int Location) {
	int connections = 0;
	if (Location <= 2) {
		end = SMALL_MAP;
		connections = 9;
	}
	else if (Location > 2 && Location <= 4) {
		end = MEDIUM_MAP;
		connections = 11;
	}
	else if (Location > 4 && Location <= 6) {
		end = LARGE_MAP;
		connections = 13;
	}
	else {
	}
	int first = 0, second = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(1, end - 1); // Avoiding 0 as a possible value

	second = distrib(gen);
	if (second == 1) {
		second = 2;
	}
	add(1, second);
	std::cout << "(1" << " , " << second << ")" << std::endl;

	for (int i = 1; i < connections; ++i) {
		first = distrib(gen);
		second = distrib(gen);

		while (first == 0 || first == second || mapp[first].size() > 0 && std::find(mapp[first].begin(), mapp[first].end(), second) != mapp[first].end()) {
			first = distrib(gen);
			second = distrib(gen);
		}

		if (first != second) {
			if (std::find(mapp[first].begin(), mapp[first].end(), second) == mapp[first].end()) {
				add(first, second);
				std::cout << "(" << first << " , " << second << ")" << std::endl;
			}
		}
		else {
			if (first + 1 < end) {
				if (std::find(mapp[first].begin(), mapp[first].end(), first + 1) == mapp[first].end()) {
					add(first, first + 1);
					Game::getinstance().nextPathTracker[first + 1]++;
				}
			}
			if (std::find(mapp[first].begin(), mapp[first].end(), first - 1) == mapp[first].end()) {
				add(first, first - 1);
				Game::getinstance().nextPathTracker[first - 1]++;
			}
		}

		if (std::find(mapp[first].begin(), mapp[first].end(), second) == mapp[first].end()) {
			add(first, second);
			std::cout << "(" << first << " , " << second << ")" << std::endl;
		}
	}

	first = distrib(gen);
	while (first == 0 || first == second || mapp[first].size() > 0 && std::find(mapp[first].begin(), mapp[first].end(), end) != mapp[first].end()) {
		first = distrib(gen);
	}
	second = end;

	if (std::find(mapp[first].begin(), mapp[first].end(), second) == mapp[first].end()) {
		add(first, second);

	}
	std::cout << "(" << first << " , " << second << ")" << std::endl;
	loadPathway(SMALL_MAP);


	std::cout << "created!" << std::endl;
}

void Map::findXpCap(int currentLvl) {
	//used to stop the increase of XP until lvl up requirements met
	switch (currentLvl) {
	case 1:
		XpCap = 200;
		break;
	case 2:
		XpCap = 450;
		break;
	case 3:
		XpCap = 800;
		break;
	default:
		std::cout << "Ran into an error calculating xpCap" << std::endl;
		break;
	}
}
void Map::add(int s, int d) {
	mapp[s].push_back(d);
	mapp[d].push_back(s);
}
int Map::inGameInputs(std::vector<int>a) {
	int movement = 0;
	std::string input;

	std::cout << "\nWhat would you like to do?: ";

	while (true) {
		if (std::cin >> movement) {
			// Handle integer input
			// Perform actions based on the integer input (if needed)
			if ((movement <= 8) && (movement > 0)) {
				std::vector<int>::iterator temp;
				auto it = find(a.begin(), a.end(), movement);
				if (it != a.end()) {
					break;// Exit the loop if an integer input is received and is part of the displayed moves available
				}

			}
			std::cout << "The attempted movement is invalid. Try again. ";

		}
		else {
			// Clear input stream state and discard invalid input
			std::cin.clear();
			std::cin >> input;

			// Handle string input
			if (input == "map") {
				if (nextPlacement != 0) {
					displayTracker();
				}
				else {
					std::cout << "You haven't made enough moves to show the map" << std::endl;
				}
			}
			//else if (input == "inv") {
			//	Game::getinstance().displayInventory();
			//}
			//else if (input == "stats") {
			//	Game::getinstance().playerN.displayStats(false);
			//}
			//else if (input == "reload") {
			//	//get the current weapon
			//	if (Game::getinstance().reloadWeapon()) {
			//		std::cout << "Weapon's reloaded\n";
			//	}
			//}
			else if (input == "turn") {
				if (!Game::getinstance().turnByturn) {
					Game::getinstance().turnByturn = true;
				}
				else {
					Game::getinstance().turnByturn = false;
				}
			}
			else {
				std::cout << "The input given wasn't a recognized option" << std::endl;
			}

			std::cout << "\nWhat would you like to do?: ";
		}
	}

	return movement;
}


void Map::loadPathway(int n) {//a list of which path has an enemy present
	srand(time(NULL));
	int reachThreeCount = 1;
	for (int i = 1; i <= n; ++i) {
		bool hasEnemy = rand() % 2;
		if (reachThreeCount % 3 == 0) {
			hasEnemy = true;
		}
		pathwayy.insert(std::pair<int, bool>(i, hasEnemy));
		if (!hasEnemy) {
			reachThreeCount++;
		}

	}
}
void Map::makeMove(int currLocation) {
	if (currLocation == end) {
		return;
	}
	if (currLocation == -1) {
		return;
	}
	makeMove(availableMoves(currLocation));

}
int Map::availableMoves(int a) {
	srand(time(NULL));
	currentPlacement = a;

	if (pathwayy[a]) {//this gets the .second of the map which is whether there is an enemy
		//std::cout << "There is an enemy in the room" << std::endl;
		//int listsize = Game::getinstance().enemyList.size();
		//int randomEnemy = rand() % listsize + 1;

		/*Enemy newEnemy(currentDungeonNum);
		if (randomEnemy >= listsize) {
			newEnemy = Game::getinstance().enemyList[randomEnemy - 1];
		}
		else {
			newEnemy = Game::getinstance().enemyList[randomEnemy];
		}
		std::cout << "\nYou've run into a " << newEnemy.getName() << "\n";*/


		//std::unique_ptr<Player> playerPtr = std::make_unique<Player>(Game::getinstance().playerN);
		//std::unique_ptr<Enemy> newEnemyPtr = std::make_unique<Enemy>(newEnemy);
		//std::unique_ptr<bool> battleResultPtr = DungeonBattle(Game::getinstance().playerN, newEnemyPtr);

		//std::cout << newEnemy.getName()<<" has appeared for battle" << std::endl;

		/*if (!(*battleResultPtr)) {
			return -1;
		}*/
		//std::string profLvl = Game::getinstance().playersCurrentWeapon->getproficiencyLvl();
		//int prof = Game::getinstance().playersCurrentWeapon->getproficiency();

		//if (Game::getinstance().playerN.checkweaponProficiency(profLvl, prof)) {
		//	if (profLvl == "Master") {
		//		Game::getinstance().playersCurrentWeapon->setWeaponLvl(Game::getinstance().playersCurrentWeapon->getWeaponLvl() + 1); //lvl increase after prev is mastered
		//	}

		//	Game::getinstance().playersCurrentWeapon->setproficiencyLvl(profLvl);
		//	Game::getinstance().playersCurrentWeapon->setproficiency(prof);
		//	Game::getinstance().playersCurrentWeapon->setWeaponDmg(Game::getinstance().playersCurrentWeapon->getWeaponDmg() + 1);
		//}
		//std::cout << "You're achievement: +" << newEnemy.getGold() << " gold and +" << newEnemy.getXP() << " XP" << std::endl;
		//totalXP += newEnemy.getXP();
		//totalGold += newEnemy.getGold();
	}
	//InGameDecisions(std::cin);
	std::vector<int>availableMoveList;
	std::cout << "The following pathways are available from here: ";
	for (auto neighbor : mapp[a]) {
		std::cout << "[ " << neighbor << " ";
		availableMoveList.push_back(neighbor);
	}
	std::cout << "]";

	int temp = inGameInputs(availableMoveList);
	availableMoveList.clear();
	nextPlacement = temp;
	upDateTracker();
	return temp;
}
/******************************************************************************************************
GAME CLASS init,cleaner, getters and setters

*******************************************************************************************************/

Game::Game() {
	/*MainMenu& master = MainMenu::getInstance();
	master.display();*/
	//enemyList.clear();
	locations.clear();
	//BossReq.push_back("Troll"); not used
	uploadWorldMap();
	turnByturn = false;
}
Game::~Game() {
	std::cout << "Game instance deleted" << std::endl;
}
void Game::fromSQL(bool i) {
	onserver = i;
}
void Game::fromLocal(bool i) {
	onlocal = i;
}
void Game::uploadWorldMap() {
	std::vector<std::string>world_mapNames = { "Realm of Qiteoria", "Realm of Reperion: Dark Forest","Ruined Settlement of Ibburyon","The Echo Lands","The Barrens","The Reach of Dreams","Lands of the Broken","The Hells Vale" };

	for (std::string maps : world_mapNames) {
		std::map<std::string, bool>mapItem;
		mapItem.insert(std::make_pair(maps, false));
		world_map.push_back(mapItem);
	}



}

Game& Game::getinstance() {
	static Game instance;
	return instance;
}


bool Game::isSQL()const {
	return onserver;
}
bool Game::isLocal()const {
	return onlocal;
}
void Game::getLocationName(int loc) {
	std::map<std::string, bool>currentLocation = world_map[loc - 1];
	for (auto& mapItem : currentLocation) {
		std::cout << "You've entered: " << mapItem.first << "\n";
	}
}
int Game::TravelonWorldMap() {
	int location_num = 1;
	for (auto& map : world_map) {
		for (auto& elements : map) {
			//elements.second
			if (playerN.getLvl() >= location_num) {
				std::cout << "Location " << location_num << ": " << elements.first << '\n';

			}
			else {
				std::cout << "Location " << location_num << ": Unavailable at your current level" << std::endl;
			}
			location_num++;
		}
	}
	int travelLocation = 0;
	std::cout << "\n Which locaiton do you want to travel to?: ";
	std::cin >> travelLocation;

	if (travelLocation > playerN.getLvl()) {//player level and the boolean should match for access
		std::cout << "This location isnt available to you \n";
		return -1;
	}
	else if (travelLocation > world_map.size()) {
		std::cout << "The choice given is invalid \n";
		return -1;
	}
	std::cout << "Travelling to new region....\n";
	//get the name
	for (auto location : world_map) {
		for (auto location_name : location) {
			std::cout << "You've arrived at: " << location_name.first << "\n";
		}
	}
	return travelLocation;
}
void Game::loadEnemies(int loc, int dunNum, std::vector<Enemy>& e) {
	try {
		/*SQLCONN& enemyGrab = SQLCONN::createInstance();
		enemyGrab.getEnemies(loc, dunNum, e);*/
		//enemyGrab.disconnect();
	}
	catch (bool result) {
		std::cout << "Database failed to connect" << std::endl;
	}
}
/******************************************************************************************************
GAME CLASS game loading and instances

******************************************************************************************************* */

bool Game::loadGame() {
	int choice{ 0 };
	std::string playername;
	bool conn_success = false;
	std::cout << "Are you loading from a server or local storage?: ";
	std::cout << "1.) Server" << "\t" << "2.) Local Storage" << std::endl;
	std::cin >> choice;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clear anything remaining in the stream so we can get playername data
	switch (choice) {
	case 1:
	{
		//check SQL server
		SQLCONN& connection = SQLCONN::createInstance();
		if (connection.connect()) {
			//gather avail chars in dB & display
			if (connection.displayNames()) {
				fromSQL(true);
				int choice;
				std::cout << "\n Which character would you like to load?:  ";
				std::cin >> choice;
				if (choice <= connection.playerList.size()) {
					std::cout << connection.playerList[choice - 1] << std::endl;
					playerN.setName(connection.playerList[choice - 1]);
					conn_success = true;
				}
				else {
					std::cout << "You didnt choose a given number" << std::endl;
				}
				//let user choose char
				//loadPlayer();

			}
			else {
				std::cout << "There is no data to load" << std::endl;
			}

			connection.disconnect();
		}


		//load data into Player class

		break;
	}
	case 2: {
		//check local storage
		break;
	}
	default:
		std::cout << "Please choose a correct option" << std::endl;
		break;
	}

	return conn_success;
}
bool Game::PrePlay() {
	GameInit = true;
	bool tryAgain = true;
	bool success = false;
	char option;
	currentDunLvl = 1;
	currentDunNum = 1;
	playerN.setLocation(currentDunLvl);
	//while (tryAgain) {
	//	getLocationName(1);//starting new
	//	Map newMap;
	//	std::cout << "creating paths" << std::endl;
	//	newMap.createPaths(currentDunLvl);
	//	Game::getinstance().loadEnemies(1, 1, Game::enemyList);
	//	std::cout << Game::getinstance().enemyList.size();
	//	if (play(newMap)) {
	//		//go back to island
	//		GameInit = false;
	//		success = true;
	//		tryAgain = false;
	//	}
	//	//got back to main menu or try again
	//	else {
	//		std::cout << "You have died. Would you like to try again? Type C to continue or Q to quit: ";
	//		std::cin >> option;
	//		if (option == 'Q' || option == 'q') {
	//			tryAgain = false;
	//		}
	//		Game::getinstance().playerN.refillHP();
	//	}
	//}
	//return success;
	return true;
}
/******************************************************************************************************
GAME CLASS player association

*******************************************************************************************************/
void Game::createPlayer(std::string n) {
	Player p(n);
	p.init();
	p.preLoadAllSkills();
	playerN =std::move(p);
	newChar = true;
}

//MAIN MENU

/******************************************************************************************************
MAIN MENU CLASS init,cleaner, getters and setters

*******************************************************************************************************/
MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

MainMenu& MainMenu::getInstance() {
	static MainMenu instance;
	return instance;
}


/******************************************************************************************************
MAIN MENU CLASS only visible function: display() shows the new game, load game and save game options

Each option chosen will determine how the user interacts with the framework

The end result should be to determine if you go to the game console
*******************************************************************************************************/
bool MainMenu::display()const {
	int option = 0;
	int _numofOptions = 2;
	bool GoToConsole = true;
	std::string name;
	std::cout << "---MAIN MENU---" << std::endl;
	std::cout << "\t 1.) NEW GAME" << std::endl;
	std::cout << "\t 2.) LOAD GAME" << std::endl;


	std::cout << "\n What would you like to do?: ";
	do {
		if (std::cin >> option) {
			if (option <= 2) {
				break;
			}
		}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nInvalid choice. Try again";
		
	} while (true);
	
	Game& gameInstance = Game::getinstance();
	switch (option) {

	case 1: {
		std::cout << " What name will you bestow upon your new character?: ";
		std::cin >> name;


		gameInstance.createPlayer(name);
		/*if (!gameInstance.PrePlay()) {
			GoToConsole = false;
		}*/
	}
		  break;
	case 2: {
		SQLCONN& sqlInstance = SQLCONN::createInstance();
		if (!gameInstance.loadGame()) {
			//make sure there actually is saved data
			//if there is user selects from available players
			GoToConsole = false;
		}
		//load chosen player
		if (!sqlInstance.loadAllData()){
			std::cout << "Error loading player \n";
			GoToConsole = false;
		}

	}
		  break;
	default:
		std::cout << "Please enter valid number" << std::endl;
		break;
	}

	return GoToConsole;
}

