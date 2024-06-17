#include "Character.h"
#include "Skills.h"
#pragma once

#ifndef _ENEMY_H_
#define _ENEMY_H_

class Enemy : public Character {
	int atkNum;
	int givenGold;
	int givenXP;
	int dunlvl;
	int dunLoc;
	int lvl;
	std::string atkCap;
	int enemyID; //this is only pulled from dB
	
public:
	Enemy();
	Enemy(const std::string, const std::string, int, int, int, float, int, int, int, int, int);
	Enemy(const Enemy&);
	Enemy(Enemy&&)noexcept;
	~Enemy();
	std::vector<Skills>listofSkills;
	//setters
	void setatkNum(int);
	void setGivenGold(int);
	void setDunLvl(int);
	void setDunLoc(int);
	void setLvl(int);
	void setAtkCap(int, int);
	void setEnemyID(int);
	void implementStats(int);
	//getters
	int getatkNum()const;
	int getDunLvl()const;
	int getDunLoc()const;
	int getGivenXp()const;
	int getGivenGold()const;
	int getLvl()const;
	int getID()const;
	void loadSkills(Skills&&);
	Skills&& removeSkills(std::string&);//this is for Hunter subclass
	//combine removeSkills and loadSkills for taking and receiving skills from player
	//only Slime has that skill
	Enemy& operator=(Enemy&& other)noexcept;

	std::tuple<std::string, int> attack(Skills& currSkill);
	
	
};

#endif