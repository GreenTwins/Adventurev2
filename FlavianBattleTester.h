#pragma once

#ifndef _BATTLE_TEST_H_
#define _BATTLE_TEST_H_


#include "Game.h"
#include "SQLCONN.h"


class BattleTest{
	Player player;
	bool playerisLoaded;
	Enemy enemy;
	bool enemyisLoaded;
	
public:
	BattleTest();
	~BattleTest();
	bool loadPlayer();
	bool loadEnemy();
	bool TransactionLoad(char t);
	void adjEnStats(int);
	void adjPlStats(int);
	bool execute_request_unlocked(int);
	bool execute_request_locked(int);
	void createPlayer();
	void Arena();
	void SoloBattle(Player& p, Enemy&);
	void GroupBattle(Player& p, Enemy&);
	void PlayerAttack(Player& p, Enemy& e);
	void EnemyAttack(Player& p, Enemy& e);
};


#endif
