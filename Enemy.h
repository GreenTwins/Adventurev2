#include "Character.h"
#pragma once

#ifndef _ENEMY_H_
#define _ENEMY_H_

class Enemy : public Character {
	int atkNum;
	int givenGold;
	int givenXP;
	int dunlvl;
	int dunLoc;
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
	//getters
	int getatkNum()const;
	int getDunLvl()const;
	int getDunLoc()const;
	int getGivenXp()const;
	int getGivenGold()const;

	void loadSkills(Skills&&);
	Skills&& removeSkills(std::string&);//this is for Hunter subclass
    //combine removeSkills and loadSkills for taking and receiving skills from player
	//only Slime has that skill
};

#endif