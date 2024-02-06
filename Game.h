#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <list>

#include "Player.h"
#include "Enemy.h"
#pragma once
#ifndef _MAP_H_
#define _MAP_H_

/******************************************************************************************************
MAP CLASS

Holds the overview of the concept of "Map", containing various map sizes, map creation, start and end map data,
character movements, enemy placement


*******************************************************************************************************/
class Map {

public:
	const int SMALL_MAP = 8;
	const int MEDIUM_MAP = 10;
	const int LARGE_MAP = 12;
	int data;
	int beg, end;
	int currentDungeonLvl = 0;
	int currentDungeonNum = 0;
	bool isBossPresent = false;
	std::map<int, std::list<int>>mapp;//provides the moves
	std::map<int, bool>pathwayy;//provides enemy location


	Map();
	~Map();
	void createPaths(int Location);
	void loadPathway(int n);
	void add(int s, int d);
	int availableMoves(int a);
	void makeMove(int currLocation);
	//void loadMapData();
	bool bossBattle(int loc, int dunNum, Player& p1);
	std::unique_ptr<bool> DungeonBattle(Player& pl, std::unique_ptr<Enemy>& en);



	//SPOILS FROM BATTLE
	int totalGold;
	int totalXP;
	int XpCap;
	int weaponXp;
	int weaponlvl;
	void findXpCap(int playerLvl);

	//Players Map/Move tracker
	std::map<int, std::list<int>>MoveTracker;
	std::queue<int>q;
	std::map<int, int>visited;
	int currentPlacement, nextPlacement;

	void addEdge(int u, int v);
	bool checkVisited(int n);
	void upDateTracker();
	void displayTracker();
	int inGameInputs(std::vector<int>currentMoves);
};

#endif

/******************************************************************************************************
GAME CLASS

Holds the overview of the concept of "Game", loaded data location (server/local) map locations, holds
list of enemies based on location given, player interactions (load, fight, create) as well as game playability


*******************************************************************************************************/
#pragma once
#ifndef _GAME_H_
#define _GAME_H_
class Game {


	Game(); //
	~Game();
	Game(Game& o) = delete;
	Game& operator=(const Game&) = delete;
	bool onserver = false;
	bool onlocal = false;

public:
	int currentDunNum = 0;
	int currentDunLvl = 0;
	bool newChar = false;
	bool turnByturn;
	Player playerN;
	//map and dungeon creation
	std::vector<Map>maps;
	std::map<int, int>nextPathTracker;
	std::vector<std::map<int, std::string>>AllMissions;
	std::string ActiveMissionName;
	std::map<int, std::string>locationforMissions; //holds dunNum grabbed and name
	std::vector<std::map<int, int>>locations;//first int has mapLocation and 2nd is the num of dungeons at that location. The location also determines the size of the map
	//std::vector<Enemy>enemyList;//this gets loaded based on the location
	bool GameInit = false;
	static Game& getinstance();
	void createPlayer(std::string n);
	//enter map-> map holds the dungeons and expects a player and a list of enemies as its input


	bool loadGame();
	bool startMission();
	void loadAllMissions();
	void displayMapsAvailable();
	void loadEnemies(int loc, int dunNum, std::vector<Enemy>& enemyList);
	bool PrePlay();
	bool play(Map& currentMap);
	void getLocationName(int loc);
	bool isSQL()const;
	void fromSQL(bool i);
	void fromLocal(bool i);
	bool isLocal()const;
	void SkillTree();

	//STORE
	void displayStore(int dunLvl);
	void loadStore();



	//PLAYERS INVENTORY


	std::map<std::string, bool>activeItems;
	//std::vector<std::string>BossReq;


	//world map
	std::vector<std::map<std::string, bool>>world_map; //bool for visibility based on level if false user cant see name nor choose
	void uploadWorldMap();
	int TravelonWorldMap();

};


#endif
/******************************************************************************************************
MAIN MENU CLASS

Holds the overview of the concept of "Main Menu", containing all overarching game functions such as
New game instance, load game instance and save game instance


*******************************************************************************************************/
#pragma once
#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_
class MainMenu {
	MainMenu();
	~MainMenu();
	MainMenu& operator=(const MainMenu&) = delete;
	MainMenu(MainMenu& o) = delete;
public:
	static MainMenu& getInstance();
	void Save();
	bool display()const;
};


#endif