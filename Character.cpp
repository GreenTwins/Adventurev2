#include "Character.h"
#include <iostream>






Character::Character() {
	std::string BodyType = "";
	std::string Name = "";
	int HP = 0;
	int maxHP = 0;
	int MP = 0;
	int maxMP = 0;
	int stamina = 0;
	int maxStamina = 0;
	int fatigue = 0;
	int maxFatigue = 0;
	float prec = 0.00;
	float maxPrec = 0.00;
	int Str = 0;
	int Def = 0;
	int Spd = 0;
	
	dodge.clear();
	
}

Character::Character(std::string a, std::string type) {
	std::string BodyType = type;
	std::string Name = a;

	int HP = 0;
	int maxHP = 0;
	int MP = 0;
	int maxMP = 0;
	int stamina = 0;
	int maxStamina = 0;
	int fatigue = 0;
	int maxFatigue = 0;
	float prec = 0.00;
	float maxPrec = 0.00;
	int Str = 0;
	int Def = 0;
	int Spd = 0;

	dodge.clear();
	upperBody.clear();
	lowerBody.clear();

}

Character::~Character() {
	dodge.clear();
}

std::string Character::getBodyType()const {
	return BodyType;
}
std::string Character::getName() const {
	return Name;
}

int Character::getHP() const {
	return HP;
}

int Character::getMaxHP() const {
	return maxHP;
}

int Character::getMP() const {
	return MP;
}

int Character::getMaxMP() const {
	return maxMP;
}

float Character::getStamina() const {
	return stamina;
}

int Character::getMaxStamina() const {
	return maxStamina;
}

int Character::getFatigue() const {
	return fatigue;
}

int Character::getMaxFatigue() const {
	return maxFatigue;
}

float Character::getPrec() const {
	return prec;
}

float Character::getMaxPrec() const {
	return maxPrec;
}

std::vector<int> Character::getDodge() const {
	return dodge;
}

int Character::getStr() const {
	return Str;
}

int Character::getDef() const {
	return Def;
}

int Character::getSpd() const {
	return Spd;
}
int Character::getEnd()const {
	return enD;
}
int Character::getInt()const {
	return Int;
}
int Character::getDex()const {
	return dex;
}
// Setters
void Character::setBodyType(std::string bodyType) {
	BodyType = bodyType;
	if (bodyType == "Humanoid") {
		upperBody.emplace(std::make_pair<std::string, float>("Head", 0.20f));
		upperBody.emplace(std::make_pair<std::string, float>("Torso", 0.40f));
		upperBody.emplace(std::make_pair < std::string, float>("Left Arm", 0.10f));
		upperBody.emplace(std::make_pair < std::string, float>("Right Arm", 0.10f));

		lowerBody.emplace(std::make_pair<std::string, float>("Right Leg", 0.10f));
		lowerBody.emplace(std::make_pair<std::string, float>("Left Leg", 0.10f));
		
		Head.HP = 0.20f * (getHP());
		Torso.HP = 0.40f * (getHP());
		LeftArm.HP = 0.10f * (getHP());
		RightArm.HP= 0.10f * (getHP());
		LeftLeg.HP= 0.10f * (getHP());
		RightLeg.HP= 0.10f * (getHP());

		Head.def, Torso.def, LeftArm.def, RightArm.def, LeftLeg.def, LeftArm.def = getDef();

		return;
	}
	if (bodyType == "Insecta") {
		if (hasWings) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.24f));
			upperBody.emplace(std::make_pair<std::string, float>("Thorax", 0.40f));
			lowerBody.emplace(std::make_pair<std::string, float>("Abdomen", 0.24f));

			Head.HP = 0.24f * (getHP());
			Thorax.HP = 0.40f * (getHP());
			Abdomen.HP = 0.24f * (getHP());

			Head.def, Thorax.def, Abdomen.def = getDef();
		}
		else if (!hasLegs) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.40f));
			lowerBody.emplace(std::make_pair<std::string, float>("Abdomen", 0.60f));

			Head.HP = 0.40f * (getHP());
			Abdomen.HP = 0.60f * (getHP());

			Head.def, Abdomen.def = getDef();
		}
		else {


			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.24f));
			upperBody.emplace(std::make_pair<std::string, float>("Thorax", 0.40f));
			upperBody.emplace(std::make_pair < std::string, float>("Left Arms", 0.03f));
			upperBody.emplace(std::make_pair < std::string, float>("Right Arms", 0.03f));

			lowerBody.emplace(std::make_pair<std::string, float>("Abdomen", 0.24f));
			lowerBody.emplace(std::make_pair<std::string, float>("Right Legs", 0.03f));
			lowerBody.emplace(std::make_pair<std::string, float>("Left Legs", 0.03f));

			Head.HP = 0.24f * (getHP());
			Thorax.HP = 0.40f * (getHP());
			Abdomen.HP = 0.24f * (getHP());
			LeftArm.HP = 0.03f * (getHP());
			RightArm.HP = 0.03f * (getHP());
			LeftLeg.HP = 0.03f * (getHP());
			RightLeg.HP = 0.03f * (getHP());

			Head.def, Thorax.def, LeftArm.def, RightArm.def, LeftLeg.def, LeftArm.def, Abdomen.def = getDef();
		}
		return;
	}
	if (bodyType == "Reptillia") {
		if (hasWings) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.30f));
			lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.60f));
			lowerBody.emplace(std::make_pair<std::string, float>("Tail", 0.10f));

			Head.def, Trunk.def, Tail.def = getDef();
		}
		else if (!hasLegs) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.50f));
			lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.50f));

			Head.HP = 0.30f * (getHP());
			Trunk.HP = 0.40f * (getHP());

			Head.def, Trunk.def = getDef();
		}
		else {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.30f));
			lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.40f));
			lowerBody.emplace(std::make_pair<std::string, float>("Tail", 0.06f));
			upperBody.emplace(std::make_pair < std::string, float>("Left Arms", 0.06f));
			upperBody.emplace(std::make_pair < std::string, float>("Right Arms", 0.06f));
			lowerBody.emplace(std::make_pair<std::string, float>("Right Legs", 0.06f));
			lowerBody.emplace(std::make_pair<std::string, float>("Left Legs", 0.06f));

			Head.HP = 0.30f * (getHP());
			Trunk.HP = 0.40f * (getHP());
			Tail.HP = 0.06f * (getHP());
			LeftArm.HP = 0.06f * (getHP());
			RightArm.HP = 0.06f * (getHP());
			LeftLeg.HP = 0.06f * (getHP());
			RightLeg.HP = 0.06f * (getHP());

			Head.def, Trunk.def, LeftArm.def, RightArm.def, LeftLeg.def, LeftArm.def, Tail.def = getDef();
		}
		

		

		
		return;
	}
}

void Character::setName(std::string name) {
	Name = name;
}

void Character::setStamina(float st) {
	stamina = st;
}

void Character::setMaxStamina(int maxSt) {
	maxStamina = maxSt;
}

void Character::setFatigue(int ft) {
	fatigue = ft;
}
void Character::setMaxMp(int mmp) {
	maxMP = mmp;
}
void Character::setMaxFatigue(int maxFt) {
	maxFatigue = maxFt;
}

void Character::setPrec(float pr) {
	prec = pr;
}

void Character::setMaxPrec(float maxPr) {
	maxPrec = maxPr;
}

void Character::setDodge(int dod) {
	dodge.push_back(dod);
}
void Character::setStr(int str) {
	Str = str;
}

void Character::setDef(int def) {
	Def = def;
}

void Character::setSpd(int spd) {
	Spd = spd;
}

void Character::setInt(int intl) {
	Int = intl;
}

void Character::setDex(int d) {
	dex = d;
}

void Character::setEnD(int end) {
	enD = end;
}
void Character::setMaxHP(int mH) {
	maxHP = mH;
}
void Character::setHP(int h) {
	HP = h;
}
void Character::setMP(int m) {
	MP = m;
}

