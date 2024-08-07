#include "Enemy.h"
#include <tuple>

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
	//spd, dex, fatigue, prec, int
}

Enemy::~Enemy() {

}
Enemy::Enemy(const Enemy& other): Character(other) {
	setBodyType(other.getBodyType());
	setName(other.getName());
	setHP(other.getHP());
	setLvl(other.getLvl());
	setMaxHP(other.getMaxHP());
	setMP(other.getMP());
	setMaxMp(other.getMaxMP());
	setStamina(other.getStamina());
	setPrec(other.getPrec());
	setStr(other.getStr());
	setDef(other.getDef());
	setSpd(other.getSpd());
	setInt(other.getInt());
	setEnemyID(other.getID());
	listofSkills = other.listofSkills;
	atkNum = other.getatkNum();
	givenGold = other.getGivenGold();
	givenXP = other.getGivenXp();
	dunlvl = other.getDunLvl();
	dunLoc = other.getDunLoc();
}
Enemy::Enemy(Enemy&& other)noexcept: Character(std::move(other)){
	setBodyType(other.getBodyType());
	setName(other.getName());
	setHP(other.getHP());
	setLvl(other.getLvl());
	setMaxHP(other.getMaxHP());
	setMP(other.getMP());
	setMaxMp(other.getMaxMP());
	setStamina(other.getStamina());
	setPrec(other.getPrec());
	setStr(other.getStr());
	setDef(other.getDef());
	setSpd(other.getSpd());
	setInt(other.getInt());
	setEnemyID(other.getID());
	listofSkills = std::move(other.listofSkills);
	atkNum = other.getatkNum();
	givenGold = other.getGivenGold();
	givenXP = other.getGivenXp();
	dunlvl = other.getDunLvl();
	dunLoc = other.getDunLoc();
}
Enemy& Enemy::operator=(Enemy&& other)noexcept {
	if (this != &other) {
		// Move-assign members from 'other' to 'this'
		// Example:
		setName(other.getName());
		enemyID = other.enemyID;
		lvl = other.lvl;
		setStr(other.getStr());
		setDef(other.getDef());
		setSpd(other.getSpd());
		setInt(other.getInt());
		setDex(other.getDex());
		setEnD(other.getEnd());
		setPrec(other.getPrec());
		setMaxPrec(other.getMaxPrec());
		setStamina(other.getStamina());
		setMaxStamina(other.getMaxStamina());
		setFatigue(other.getFatigue());
		setHP(other.getHP());
		setMaxHP(other.getMaxHP());
		setMP(other.getMP());
		setMaxMp(other.getMaxMP());
		setBodyType(other.getBodyType());
		listofSkills = other.listofSkills;
		givenGold = other.givenGold;
		givenXP = other.givenXP;
		dunlvl = other.dunlvl;
		dunLoc = other.dunLoc;
		atkCap = other.atkCap;

	}
	return *this;
}
void Enemy::implementStats(int loc) {
	int level = rand() % (getLvl()) + loc;
	int currentLevel = getLvl();
	setLvl(level);

	int endurRoll = ((rand() % 6+1)*(getLvl()));
	int intelRoll = ((rand() % 6 + 1) * (getLvl()));
	int dexRoll = ((rand() % 6 + 1) * (getLvl()));
	//setatkNum(intelRoll + 1);
	

	if (currentLevel < level) {
		//upgrade everything else
		setHP(getHP() + ((endurRoll + dexRoll) * level));
		setMP(getMP() + (intelRoll * level));
		setStr(getStr() * level);
		setDef(getDef() * level);
		setSpd(getSpd() * level);

	}
	if (getMP() <= 0) {
		setMP(intelRoll * currentLevel);
	}
	setEnD(endurRoll);
	setInt(intelRoll);
	setDex(dexRoll);
	setAtkCap(intelRoll, getLvl());
	setMaxHP(getHP());
	setMaxMp(getMP());
	setStamina(calculateStamin(getDex(), getEnd(),6.33 ));//dex, endurance, ptWorth which is 6.33 for enemies
	setMaxStamina(getStamina());
	int fati = 0;
	setPrec(calculatePrec(getStamina(), fati, getMaxStamina(), getDex(), getInt()));
	//setdodgingSkill(calculatedodgeSkill(getSpd(), getInt(), getDex(), getPrec()));

}
std::tuple<std::string, int> Enemy::attack(Skills& currSkill) {
	//std::cout<< Enemy::getName()<<" uses "<<currSkill.getSkillName();
	//what type is it?
	std::string atktype = currSkill.getAtkType();
	std::string skilltype = currSkill.getSkillType();
	
	//temp
	std::tuple<std::string, int>temp = { "blah", 0 };

	return temp;
}
void Enemy::setatkNum(int num) {
	atkNum = num;
}
double Enemy::getdodgingSkill()const{
	return dodgingSkill;
}
double Enemy::calculatedodgeSkill(int spd, int Intel, int dex, float prec) {
        double normalizedSpd = spd / 100.0;
        double normalizedIntel = Intel / 100.0;
        double normalizedDex = dex / 100.0;
        double normalizedPrec = prec / 100.0;

        double combined = normalizedSpd * normalizedDex / (normalizedPrec * normalizedIntel);

        // Logistic function to keep the value between 0 and 1
        double k = 10.0;
        double x0 = 0.5;
        double logisticValue = 1.0 / (1.0 + exp(-k * (combined - x0)));

        // Scaling the logistic output to be between 0.10 and 1.00
        return 0.10 + 0.90 * logisticValue;
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
void Enemy::setEnemyID(int enID) {
	enemyID = enID;
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

int Enemy::getID()const {
	return enemyID;
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