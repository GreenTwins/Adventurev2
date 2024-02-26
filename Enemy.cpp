#include "Enemy.h"

Enemy::Enemy() :Character() {

}

Enemy::Enemy(const std::string bT, const std::string n, int hp, int mp, int stam,
	float pre, int intel, int str, int def, int spd, int dod) {
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
void Enemy::implementStats() {
	int level = rand() % (getLvl()) + 1;
	int currentLevel = getLvl();
	setLvl(level);

	int endurRoll = rand() % (getLvl() * 6) + 1;
	int intelRoll = rand() % (getLvl() * 6) + 1;
	int dexRoll = rand() % (getLvl() * 6) + 1;
	setatkNum(intelRoll + 1);
	if (getMP() == 0) {
		setMP(intelRoll * currentLevel);
	}

	if (currentLevel < level) {
		//upgrade everything else
		setHP(getHP() + ((endurRoll + dexRoll) * level));
		setMP(getMP() + (intelRoll * level));
		setStr(getStr() * level);
		setDef(getDef() * level);
		setSpd(getSpd() * level);

	}
	setEnD(endurRoll);
	setInt(intelRoll);
	setDex(dexRoll);
	setAtkCap(intelRoll, getLvl());
}
void Enemy::setatkNum(int num) {
	atkNum = num;
}
void Enemy::setAtkCap(int intel, int lvl) {
	if (intel < (lvl * 3)) {
		atkCap = "minimum";
		setatkNum(2);
	}
	else if ((intel > (lvl * 3)) && (intel < (lvl * 4))) {
		atkCap = "median";
		setatkNum(3);
	}
	else {
		atkCap = "maximum";
		setatkNum(4);
	}
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
void Enemy::setLvl(int level) {
	lvl = level;
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
int Enemy::getLvl()const {
	return lvl;
}