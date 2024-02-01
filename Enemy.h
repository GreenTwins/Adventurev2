#include "Character.h"
#pragma once

#ifndef _ENEMY_H_
#define _ENEMY_H_

class Enemy : public Character {
	int atkNum;
public:
	Enemy();
	Enemy(const std::string, const std::string, int, int, int, float, int, int, int, int, int);
	Enemy(const Enemy&);
	Enemy(Enemy&&);
	~Enemy();

	//setters
	void setatkNum(int);

	//getters
	int getatkNum()const;
};

#endif