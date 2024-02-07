#pragma once
#ifndef _SQLCONN_H_
#define _SQLCONN_H

#include <vector>
#include "Enemy.h"





class SQLCONN {
	SQLCONN();
	~SQLCONN();

public:
	SQLCONN(const SQLCONN& o) = delete;
	SQLCONN& operator=(const SQLCONN&) = delete;
	static SQLCONN& createInstance();
	bool connect();
	void disconnect();
	//display all chars
	bool displayNames();
	//gather all enemies from sql
	void getEnemies(int loc, int dunNum, std::vector<Enemy>& e);
	//bool getBoss(int loc, int dunNum, Boss& b);
	bool sqlSave();
	bool InvSave();
	std::vector<std::string>playerList;
	bool loadPlayerData(const std::string& a);
	bool saveplayerHitBox();
	bool loadPlayerHitbox(int);
	bool saveplayerSkills();
	bool playerSkillsLoading(int);
	/*bool loadPlayerInventory(int ID);
	bool grabStoreData(int lvl);
	bool getplayerID();
	bool deleteInventory(int ID);*/
	bool isConnectionActive();
};

#endif
