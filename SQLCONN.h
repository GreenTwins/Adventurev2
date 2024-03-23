#pragma once
#ifndef _SQLCONN_H_
#define _SQLCONN_H

#include <vector>
#include "Enemy.h"
#include "mutex"
#include <condition_variable>



class SQLCONN {
	SQLCONN();
	~SQLCONN();

	
public:
	std::mutex _mu;
	std::condition_variable _conVa;
	SQLCONN(const SQLCONN& o) = delete;
	SQLCONN& operator=(const SQLCONN&) = delete;
	static SQLCONN& createInstance();
    int  _vectorSize;
	bool connect();
	void disconnect();
	//display all chars
	bool displayNames();
	//gather all enemies from sql
	void LoadEnemies(int loc, int dunNum, std::vector<Enemy>&e);
	bool getEnemies(int loc, int dunNum, std::vector<Enemy>& e);
	bool getEnemySkills();
	//bool getBoss(int loc, int dunNum, Boss& b);
	bool saveAllData();
	bool loadAllData();
	bool sqlSave();
	bool InvSave();
	bool getplayerID();
	std::vector<std::string>playerList;
	bool loadPlayerData(const std::string& a);
	bool saveplayerHitBox();
	bool loadPlayerHitbox();
	bool saveplayerSkills();
	bool playerSkillsLoading();
	/*bool loadPlayerInventory(int ID);
	bool grabStoreData(int lvl);
	bool deleteInventory(int ID);*/
	bool isConnectionActive();
};

#endif