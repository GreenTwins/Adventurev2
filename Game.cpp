#include "Game.h"
#include <iostream>
#include <random>
#include <string>
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

/******************************************************************************************************
GAME CLASS init,cleaner, getters and setters

*******************************************************************************************************/

Game::Game() {
	/*MainMenu& master = MainMenu::getInstance();
	master.display();*/
	enemyList.clear();
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
	/*for (auto location : world_map) {
		for (auto location_name : location) {
			std::cout << "You've arrived at: " << location_name.first << "\n";
		}
	}*/
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
GAME CLASS player association

*******************************************************************************************************/
void Game::createPlayer(std::string n) {
	Player p(n);
	playerN = std::move(p);
	newChar = true;
	playerN.init();
}