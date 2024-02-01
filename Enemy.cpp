#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::Enemy(const std::string bT, const std::string n, int hp, int mp, int stam, 
	float pre, int intel, int str, int def,  int spd, int dod) {
	setBodyType(bT);
	setName(n);
	setHP(hp);
	setMaxHP(hp);
	setMP(mp);
	setMaxMp(mp);
	setStamina(stam);
	setPrec(pre);
	setStr(str);
	setDef(def);
	setSpd(spd);
	setDodge(dod);
	setInt(intel);
}

Enemy::~Enemy() {

}
void Enemy::setatkNum(int num) {
	atkNum = num;
}
int Enemy::getatkNum()const {
	return atkNum;
}


