#include "Game.h"
#include <sstream>
#include <iostream>
#include <map>
#include <memory>
#include <functional>
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

	std::map<int, int>& checker = Game::getinstance().nextPathTracker;

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
		int listsize = Game::getinstance().enemyList.size();
		int randomEnemy = rand() % listsize + 1;

		if (randomEnemy >= listsize) {
			randomEnemy -= 1;
			
		}

		Enemy newEnemy(Game::getinstance().enemyList[randomEnemy]);

		std::cout << "\nYou've run into a " << newEnemy.getName() << "\n";


		//std::unique_ptr<Player> playerPtr = std::make_unique<Player>(Game::getinstance().playerN);
		//std::unique_ptr<Enemy> newEnemyPtr = std::make_unique<Enemy>(newEnemy);
		std::unique_ptr<bool> battleResultPtr = DungeonBattle(Game::getinstance().playerN, newEnemy);

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

void EnemyAttacks(Player& p1, Enemy& en) {
	int enRoll = rand()%en.getatkNum();
	Skills *enSkill=  &en.listofSkills[enRoll];
	
	if (enSkill->getSkillType() != "On") {
		//could be regular attack or toggles
		if (enSkill->getSkillType() == "Toggle") {
			enSkill->setatkType("On");
			Game::getinstance().EnemyskillApplication(enSkill->getSkillEffect(), en, p1);
		}
		//something about effects
		else {
			std::cout << "regular atk";
		}
	}
	else {
		//its already on so redo
	}
}

void PlayerAttacks(Character& p1, Character& en) {

}
std::unique_ptr<bool>Map::DungeonBattle(Player& p1, Enemy& en) {
	int currentRound = 1; 
	srand(time(NULL));

	std::cout << (p1.getSpd() <= en.getSpd() ? en.getName() + " is faster!\n" : "Your speed is greater\n");

	while ((p1.getHP() > 0) && (en.getHP() > 0)) {
		if (p1.getSpd() <= en.getSpd()) {
			//Enemy atk first
			EnemyAttacks(p1, en);
			if (p1.getHP() > 0) {
				//if player alive then can atk
				PlayerAttacks(p1, en);
			}
		}
		else {
			//player atk first
			PlayerAttacks(p1, en);
			if (en.getHP() > 0) {
				EnemyAttacks(p1, en);
			}
		}
	}
	if (en.getHP() > 0) {
		return std::make_unique<bool>(true);
   }
	return std::make_unique<bool>(false);
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
	for (auto location : world_map) {
		for (auto location_name : location) {
			std::cout << "You've arrived at: " << location_name.first << "\n";
		}
	}
	return travelLocation;
}
std::shared_ptr<AttackMod>Game::getUniversalAtk() {
	return _globalatk;
}
void Game::loadAllMissions(int currLoc) {
	createDungeon(currLoc); //what dun is available at given loc and load types -> end shows active mission  
	loadEnemies(currLoc, enemyList);
}
std::string Game::SpliceConstructedWord(std::string& word, int mapItem) {
	try {
		if ((mapItem > 2)||(mapItem < 0)) {
			throw(mapItem);
		}
		std::string splicedWord;
		size_t posofSpace = word.find(' ');
		//int countToSpace = std::stoi(word.substr(0, posofSpace));
		if (mapItem == 1) {
			splicedWord = word.substr(0, posofSpace);
		}
		else {
		
			splicedWord = word.substr(posofSpace + 1);
		}

		return splicedWord;
	}
	catch (...) {
		std::cout << "\nError detected. MapItem not a 1 or a 2. Redoing: \n";
		return SpliceConstructedWord(word, 1);
	}
}
void Game::createName(std::string& placement) {
	while (true) {
		if (_createNamemtx_.try_lock()) {
			break; // Break out of the loop once we acquire the lock
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	std::vector<std::string>_desc = {};
	std::vector<std::string>_name = {};
	if (placement.length() > 6) {
		placement = SpliceConstructedWord(placement, 1);
	}
	try {
		if (placement == "Cave") {
			_desc = { "Blyke ","Blue ","Baker ","Mammoth ","Ellora ","Reed Flute ","The Silent " };
			_name = { "Cave","Grotto","Cavern","Cave","Hallows","Hole","OverHang","Hideout","Den","Shelter","Abyss" };
		}
		else if (placement == "Forest") {
			_desc = { "Thick Basin","Hallow","Misty Oak","Loch","Treacherous" };
			_name = { "Grove","Woods","Wilds","Forest","Woodland","Thicket" };
		}
		else if (placement == "Deep Forest") {

		}
		else if (placement == "Hillside") {

		}
		else if (placement == "Dungeon") {

		}
		else {
			throw (placement);
		}

		srand(time(NULL));
		std::string locationName = (_desc[rand()%_desc.size()] + _name[rand()%_name.size()]);
		placement = placement + " " + locationName;
	}
	catch (...) {
		std::cerr << "\nexception caught inside createDungeon: \n";
		
	}
	//load new mission name this will be thread safe

	//std::lock_guard<std::mutex>lock(_createNamemtx_);
	
	_createNamemtx_.unlock();
}
void Game::createDungeon(int loc) {
	std::map<int, std::string>_dunLoc;
	int _missionchoice = 0;
	switch (loc) {
	case 1: {
		_dunLoc[1] = "Cave";
		//availableMissions.push_back(_dunLoc);
		_dunLoc[2] = "Forest";
		//availableMissions.push_back(_dunLoc);
		int randomDun = rand()% 2;
		_dunLoc[3] = (randomDun == 0) ? "Cave" : "Forest";
		
	}
		  break;
	case 2: {
		_dunLoc[1] = "Deep Forest";
		//availableMissions.push_back(_dunLoc);
		_dunLoc[2] = " Hillside";
		//availableMissions.push_back(_dunLoc);
		_dunLoc[3] = "Dungeon";
		//availableMissions.push_back(_dunLoc);
	}
		  break;
	case 3: {
		_dunLoc[1] = "Ruins";
		_dunLoc[2] = "Ruins";
		_dunLoc[3] = "Ruins";
		//availableMissions.push_back(_dunLoc);
	}
		  break;
	case 4: {
		_dunLoc[1] = "Town";
		//availableMissions.push_back(_dunLoc);
		_dunLoc[2] = "Swamp";
		//availableMissions.push_back(_dunLoc);
	}
		  break;
	default:
		std::cerr << "\nWe run an error in the location loading\n";
		break;
	}
	//now currMiss is loaded with loc type and you have the current LocLevel
	//pick 3 types to send to name creator func()
	//from name creator, player chooses which mission to go on
	//chosen mission type is used to call the dB, the locLevel is used (+1) to determine enemy lvl
	//any failure will be caught and throw to a higher level rerun
	//std::vector<std::string>_mapTypes;
	/*int _missionsSize = availableMissions.size();
	std::string Mapsavailable;
	for (int i = 0; i < 3; ++i) {
		int randNum = rand() % _missionsSize;
		for (auto type : availableMissions[randNum]) {
			Mapsavailable = type.second;
		}
		_mapTypes.push_back(Mapsavailable);
	}*/
	//using dunLoc to get the type of Location and the have the threads create the 3 names. Inside of the threads the & of the string obj has its contents changed
	//to hold the newly formed name. This can then be used again to grab the enemies func using 
	//_dunLoc.size();
	try {
		std::vector<std::thread>_workerThreads;

	
		
		for (auto& ot : _dunLoc)
			_workerThreads.push_back(std::thread(&Game::createName, this, std::ref(ot.second)));
		
		for (auto& pt : _workerThreads)
			pt.join();
	}
	catch (std::string e) {
		std::cout << "Failure. This is what was given as param to createName: " << e;
	}
	

	/*std::thread first(&Game::createName, this, std::ref(_mapTypes[0]));
	std::thread second(&Game::createName, this, std::ref(_mapTypes[1]));
	std::thread third(&Game::createName, this, std::ref(_mapTypes[2]));*/

	/*first.join();
	second.join();
	third.join();*/


	//display available missions
	int index = 1;
	for (auto& mapelem : _dunLoc) {

		std::cout << index << ".) " << mapelem.second << std::endl;
		mapelem.second = SpliceConstructedWord(mapelem.second, 1); //we dont need the name anymore
		++index;
	}
	std::cout << "\n Which mission will you choose?: ";
	std::cin >> _missionchoice;

	int finder = 1;
	for (auto& choice : _dunLoc) {
		if (finder == _missionchoice) {
			//choice.second= SpliceConstructedWord(choice.second, 2);
			ActiveMissionType = choice.second;
		}
		++finder;
	}
	//ActiveMissionType = "blah";//gotta come up with a way to get not just the name but also the type and pass the type to LoadEnemies(type, e);

}

void Game::loadEnemies(int currLoc, std::vector<Enemy>& e) {
	try {
		SQLCONN& enemyGrab = SQLCONN::createInstance();
		//loadAllMissions(currLoc);
		enemyGrab.LoadEnemies(ActiveMissionType, currLoc, e);
		enemyGrab.disconnect();
	}
	catch (bool result) {
		std::cout << "Database failed to connect" << std::endl;
	}
}


bool Game::PrePlay() {
	GameInit = true;
	bool tryAgain = true;
	bool success = false;
	char option;
	currentDunLvl = 1;
	currentDunNum = 1;
	playerN.setLocation(currentDunLvl);
	while (tryAgain) {
		getLocationName(1);//starting new
		Map newMap;
		std::cout << "creating paths" << std::endl;
		newMap.createPaths(currentDunLvl);
		Game::getinstance().loadEnemies(1, enemyList);
		std::cout << enemyList.size();
		if (play(newMap)) {
			//go back to island

			GameInit = false;
			success = true;
			tryAgain = false;
		}
		//got back to main menu or try again
		else {
			std::cout << "You have died. Would you like to try again? Type C to continue or Q to quit: ";
			std::cin >> option;
			if (option == 'Q' || option == 'q') {
				tryAgain = false;
			}
			playerN.setHP(playerN.getMaxHP());
		}
	}
	return success;
}


bool Game::play(Map& currentMap) {
	//system(CLEAR_SCREEN);
	currentMap.makeMove(1);

	if (playerN.getHP() > 0) {
		//system(CLEAR_SCREEN);
		std::cout << "You've entered the Boss room" << std::endl;
		//if (currentMap.bossBattle(currentDunLvl, currentDunNum, playerN)) {
		//	/*playerN.setGold(playerN.getcurrentMap.totalGold);
		//	playerN.setXP(currentMap.totalXP);*/
		//	return true;
		//}
		return false;
	}
	//std::cout << "You've died" << std::endl;
	//return false;
	return true;
}
Game::Game() : _globalatk(std::make_shared<AttackMod>()) {}

void reduce(const std::string& obj, int duration, int amount, Enemy& en, std::shared_ptr<AttackMod>_gA) {
	if (obj == "HP") {
		if (duration == 0) {
			std::cout << "removing " << amount << " from " << en.getHP() << std::endl;
			_gA->effectAmt = 100;
			_gA->atkAmt = amount;
		}
	}
}
void lessThan(const std::string& subj, int checker, int result, Enemy& en, std::shared_ptr<AttackMod>_gA) {
	if (subj == "HP"){
		if (result < 0) {
			std::cout << "subtracting\n";
			en.setHP(en.getHP() + result);
			//effectamt
			_gA->atkAmt = result;
		}
		else {
			if (en.getHP() < checker) {
				en.setHP(result);
				_gA->atkAmt = result;
			}
			else {
				std::cout << "Your opponent has too much health to complete \n";
			}
		}
	}
}
void greaterThan(const std::string& subj, int checker, int result, Enemy& en, std::shared_ptr<AttackMod>_gA) {

}
void equalTo(const std::string& subj, int checker, int result, Enemy& en, std::shared_ptr<AttackMod>_gA) {

}
std::map<std::string, std::function<void(const std::string&, int, int, Enemy&, std::shared_ptr<AttackMod>)>>operatorFunctions = {
	{"less", lessThan},
	{"greater", greaterThan},
	{"equal", equalTo},
	{"reduce", reduce}
};
void Game::EnemyskillApplication(const std::string& effectinput, Enemy& en, Player& p1) {
	std::istringstream iss(effectinput);
	std::string word;
	std::vector<std::string>words;

	while (iss >> word) {
		words.push_back(word);
	}

	if (words[0] == "-") {
		std::string subject = words[2];
		std::string op = "reduce";
		int duration = 0, amount;
		std::istringstream(words[1]) >> amount;

		try {
			std::istringstream durationStream(words[4]);
			durationStream.exceptions(std::ios::failbit | std::ios::badbit);
			durationStream >> duration;
		}
		catch (const std::ios_base::failure& e) {
			duration = 0;
		}

		/*if (operatorFunctions.find(op) != operatorFunctions.end()) {
			operatorFunctions[op](subject, duration, amount, en, );
		}*/
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

//Create code from input#include <iostream>
//#include <iostream>
//#include <sstream>
//#include <vector>
//#include <map>
//#include <functional>
//867688a60b0ee4f282fd0bafa50fbbde0cecdea6
//
//class Enemy {
//public:
//	Enemy(int hp) : hp(hp) {}
//
//	int getHP() const {
//		return hp;
//	}
//
//	void setHP(int newHP) {
//		hp = newHP;
//	}
//
//private:
//	int hp;
//};
//
//void lessThan(const std::string& subject, int checker, int result, Enemy& enemy) {
//	if (subject == "HP") {
//		if (result < 0) {
//			//we are then subtracting this amount from HP
//			std::cout << "subtracting\n";
//			enemy.setHP(enemy.getHP() + result);
//			return;
//		}
//		else {
//			if (enemy.getHP() < checker) {
//				enemy.setHP(result);
//				return;
//			}
//		}
//
//		std::cout << "Your opponent has too much health to complete\n";
//	}
//	// Add more conditions for other subjects if needed
//}
//
//void greaterThan(const std::string& subject, int checker, int result, Enemy& enemy) {
//	// Implement similar logic for greater than
//}
//
//void equalTo(const std::string& subject, int checker, int result, Enemy& enemy) {
//	// Implement similar logic for equal to
//}
//
//// Map operators to corresponding functions
//std::map<std::string, std::function<void(const std::string&, int, int, Enemy&)>> operatorFunctions = {
//	{"less", lessThan},
//	{"greater", greaterThan},
//	{"equal", equalTo}
//};
//
//void parseAndExecute(const std::string& userInput, Enemy& enemy) {
//	std::istringstream iss(userInput);
//	std::string word;
//	std::vector<std::string> words;
//
//	while (iss >> word) {
//		words.push_back(word);
//	}
//
//	if (words.size() >= 6) {
//		std::string subject = words[1];
//		std::string op = words[3];
//		int checker, result;
//		std::istringstream(words[5]) >> checker;
//		std::istringstream(words[9]) >> result;
//
//		if (operatorFunctions.find(op) != operatorFunctions.end()) {
//			operatorFunctions[op](subject, checker, result, enemy);
//		}
//		else {
//			std::cout << "Invalid operator: " << op << "\n";
//		}
//	}
//	else {
//		std::cout << "Invalid input format\n";
//	}
//}
//
//int main() {
//	Enemy enemy(2);
//	std::string userInput = "if HP is less than 15 set HP to 0";
//	parseAndExecute(userInput, enemy);
//
//	std::cout << "Enemy's HP after operation: " << enemy.getHP() << "\n";
//
//	return 0;
//}
/******************************************************************************************************
GAME CLASS player association

*******************************************************************************************************/
void Game::createPlayer(std::string n) {
	Player p(n);
	p.init();
	p.preLoadAllSkills();
	playerN = std::move(p);
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
		if (!gameInstance.PrePlay()) {
			GoToConsole = false;
		}
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
		if (!sqlInstance.loadAllData()) {
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

