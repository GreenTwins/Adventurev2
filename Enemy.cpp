#include "Enemy.h"

Enemy::Enemy():Character() {

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
Enemy::Enemy(const Enemy& other) {
	setBodyType(other.getBodyType());
	setName(other.getName());
	setHP(other.getHP());
	setMaxHP(other.getMaxHP());
	setMP(other.getMP());
	setMaxMp(other.getMaxMP());
	setStamina(other.getStamina());
	setPrec(other.getPrec());
	setStr(other.getStr());
	setDef(other.getDef());
	setSpd(other.getSpd());
	setInt(other.getInt());

	listofSkills = other.listofSkills;
	atkNum = other.getatkNum();
	givenGold = other.getGivenGold();
	givenXP = other.getGivenXp();
	dunlvl = other.getDunLvl();
	dunLoc = other.getDunLoc();
}
Enemy::Enemy(Enemy&& other)noexcept {
	setBodyType(other.getBodyType());
	setName(other.getName());
	setHP(other.getHP());
	setMaxHP(other.getMaxHP());
	setMP(other.getMP());
	setMaxMp(other.getMaxMP());
	setStamina(other.getStamina());
	setPrec(other.getPrec());
	setStr(other.getStr());
	setDef(other.getDef());
	setSpd(other.getSpd());
	setInt(other.getInt());

	listofSkills = std::move(other.listofSkills);
	atkNum = other.getatkNum();
	givenGold = other.getGivenGold();
	givenXP = other.getGivenXp();
	dunlvl = other.getDunLvl();
	dunLoc = other.getDunLoc();
}
void Enemy::setatkNum(int num) {
	atkNum = num;
}

void Enemy::loadSkills(Skills&& o) {
	listofSkills.push_back(std::move(o));
}
void Enemy::setGivenGold(int gold) {
	givenGold = gold;
}
void Enemy::setDunLvl(int dL) {
	dunlvl = dL;
}
void Enemy::setDunLoc(int dL) {
	dunLoc = dL;
}

int Enemy::getDunLvl()const {
	return dunlvl;
}
int Enemy::getDunLoc()const {
	return dunLoc;
}
int Enemy::getatkNum()const {
	return atkNum;
}
Skills&& Enemy::removeSkills(std::string& choice) {
	Skills removedSkill;
	for (auto& it : listofSkills) {
		if (it.getSkillName() == choice) {
			removedSkill = std::move(it);
			listofSkills.erase(std::remove(listofSkills.begin(), listofSkills.end(), it));
			break;
		}
	}
	return std::move(removedSkill);
}
int Enemy::getGivenXp()const {
	return givenXP;
}
int Enemy::getGivenGold()const {
	return givenGold;
}