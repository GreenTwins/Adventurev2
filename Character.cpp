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
Character::Character(const Character& o) {
	HP = o.HP;
	maxHP = o.maxHP;
	MP = o.MP;
	maxMP = o.maxMP;
	maxStamina = o.maxStamina;
	maxPrec = o.maxPrec;
	maxFatigue = o.maxFatigue;
	stamina = o.stamina;
	fatigue = o.fatigue;
	prec = o.prec;
	dodge = o.dodge;
	upperBody = o.upperBody;
	lowerBody = o.lowerBody;
	Str = o.Str;
	Def = o.Def;
	Spd = o.Spd;
	Int = o.Int;
	dex = o.dex;
	enD = o.enD;
	hasWings = o.hasWings;
	hasLegs = o.hasLegs;
	Head = o.Head;
	Torso = o.Torso;
	LeftArm = o.LeftArm;
	LeftLeg = o.LeftLeg;
	RightArm = o.RightArm;
	RightLeg = o.RightLeg;
	Trunk = o.Trunk;
	Tail = o.Tail;
	Thorax = o.Thorax;
	Abdomen = o.Abdomen;
	BodyType = o.BodyType;
	upperHitBox = o.upperHitBox;
	lowerHitBox = o.lowerHitBox;
}
Character::Character(Character&& o)noexcept {
	HP =          o.HP;
	maxHP =       o.maxHP;
	MP =          o.MP;
	maxMP =       o.maxMP;
	maxStamina =  o.maxStamina;
	maxPrec =     o.maxPrec;
	maxFatigue =  o.maxFatigue;
	stamina =     o.stamina;
	fatigue =     o.fatigue;
	prec =        o.prec;
	dodge =       o.dodge;
	upperBody =   o.upperBody;
	lowerBody =   o.lowerBody;
	Str =         o.Str;
	Def =         o.Def;
	Spd =         o.Spd;
	Int =         o.Int;
	dex =         o.dex;
	enD =         o.enD;
	hasWings =    o.hasWings;
	hasLegs =     o.hasLegs;
	Head =        o.Head;
	Torso =       o.Torso;
	LeftArm =     o.LeftArm;
	LeftLeg =     o.LeftLeg;
	RightArm =    o.RightArm;
	RightLeg =    o.RightLeg;
	Trunk =       o.Trunk;
	Tail =        o.Tail;
	Thorax =      o.Thorax;
	Abdomen =     o.Abdomen;
	BodyType =	  o.BodyType;
	upperHitBox = o.upperHitBox;
	lowerHitBox = o.lowerHitBox;
}
Character::~Character() {
	dodge.clear();
}
float Character::calculatePrec(int stamina, int& fatigue, int MaxStamina, int dex, int intel) {
	float indicator = 0.60f * MaxStamina;
	//std::cout << "indicator: " << indicator << "\n";
	if (stamina < indicator) {
		//std::cout << "stamina: " << stamina << " \n";
		int fallenamt = indicator - stamina;
		fatigue = fallenamt;
	}
	float precision = 0.00f;
	precision = static_cast<float>(intel) / dex;

	precision *= static_cast<float>(stamina - fatigue) / MaxStamina;
	if (precision < 0.01f) {
		precision = 0;
	}
	if (precision > 1) {
		precision = 1.00f;
	}
	return precision * 100.0f;
}
float Character::calculateStamin(int dex, int endurance, int ptWorth) {
	return (((1.0f / 3.0f) * (ptWorth * dex) + ((1.0f / 3.0f) * (ptWorth * endurance))));
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
bool Character::withWings()const {
	return hasWings;
}
bool Character::withLegs()const {
	return hasLegs;
}
int Character::getUpperBodysize()const {
	return upperBody.size();
}

int Character::getLowerBodysize()const {
	return lowerBody.size();
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

		Head.def = getDef();
		Torso.def = getDef(); 
		LeftArm.def = getDef();
		RightArm.def = getDef(); 
		LeftLeg.def = getDef();
		RightLeg.def = getDef();

		Head.bodyName = "Head";
		Torso.bodyName = "Torso";
		LeftArm.bodyName = "Left Arm";
		RightArm.bodyName = "Right Arm";
		LeftLeg.bodyName = "Left Leg";
		RightLeg.bodyName = "Right Leg";

		lowerHitBox.push_back(RightLeg);
		lowerHitBox.push_back(LeftLeg);

		upperHitBox.push_back(Head);
		upperHitBox.push_back(Torso);
		upperHitBox.push_back(LeftArm);
		upperHitBox.push_back(RightArm);
		upperHitBox.push_back(LeftLeg);
		upperHitBox.push_back(RightLeg);


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
			Thorax.bodyName = "Thorax";
			Head.def = getDef();
			Thorax.def = getDef(); 
			Abdomen.def = getDef();

			lowerHitBox.push_back(RightLeg);
			lowerHitBox.push_back(LeftLeg);

			upperHitBox.push_back(Head);
			upperHitBox.push_back(Torso);
			upperHitBox.push_back(LeftArm);
			upperHitBox.push_back(RightArm);
			upperHitBox.push_back(LeftLeg);
			upperHitBox.push_back(RightLeg);
		}
		else if (!hasLegs) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.40f));
			lowerBody.emplace(std::make_pair<std::string, float>("Abdomen", 0.60f));

			Head.HP = 0.40f * (getHP());
			Abdomen.HP = 0.60f * (getHP());

			Head.def = getDef(); 
			Abdomen.def = getDef();

			lowerHitBox.push_back(RightLeg);
			lowerHitBox.push_back(LeftLeg);

			upperHitBox.push_back(Head);
			upperHitBox.push_back(Torso);
			upperHitBox.push_back(LeftArm);
			upperHitBox.push_back(RightArm);
			upperHitBox.push_back(LeftLeg);
			upperHitBox.push_back(RightLeg);
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

			LeftArm.bodyName = "Left Arm";
			RightArm.bodyName = "Right Arm";
			LeftLeg.bodyName = "Left Leg";
			RightLeg.bodyName = "Right Leg";
			Thorax.bodyName = "Thorax";


			lowerHitBox.push_back(RightLeg);
			lowerHitBox.push_back(LeftLeg);

			upperHitBox.push_back(Head);
			upperHitBox.push_back(Torso);
			upperHitBox.push_back(LeftArm);
			upperHitBox.push_back(RightArm);
			upperHitBox.push_back(LeftLeg);
			upperHitBox.push_back(RightLeg);
			Head.def = getDef(); 
			Thorax.def = getDef(); 
			LeftArm.def = getDef(); 
			RightArm.def = getDef(); 
			LeftLeg.def = getDef(); 
			LeftArm.def = getDef(); 
			Abdomen.def = getDef();
		}

		Head.bodyName = "Head";
		Abdomen.bodyName = "Abdomen";
		lowerHitBox.push_back(RightLeg);
		lowerHitBox.push_back(LeftLeg);

		upperHitBox.push_back(Head);
		upperHitBox.push_back(Torso);
		upperHitBox.push_back(LeftArm);
		upperHitBox.push_back(RightArm);
		upperHitBox.push_back(LeftLeg);
		upperHitBox.push_back(RightLeg);
		return;
	}
	if (bodyType == "Reptillia") {
		if (hasWings) {
			if (hasLegs) { 
				//dragon
				upperBody.emplace(std::make_pair < std::string, float>("Left Arms", 0.06f));
				upperBody.emplace(std::make_pair < std::string, float>("Right Arms", 0.06f));
				lowerBody.emplace(std::make_pair<std::string, float>("Right Legs", 0.06f));
				lowerBody.emplace(std::make_pair<std::string, float>("Left Legs", 0.06f));
				upperBody.emplace(std::make_pair<std::string, float>("Head", 0.30f));
				lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.40f));
				lowerBody.emplace(std::make_pair<std::string, float>("Tail", 0.06f));

				LeftArm.bodyName = "Left Arm";
				RightArm.bodyName = "Right Arm";
				LeftLeg.bodyName = "Left Leg";
				RightLeg.bodyName = "Right Leg";
				Tail.bodyName = "Tail";


				lowerHitBox.push_back(RightLeg);
				lowerHitBox.push_back(LeftLeg);
				lowerHitBox.push_back(Tail);
			

				upperHitBox.push_back(LeftArm);
				upperHitBox.push_back(RightArm);

				Head.def = getDef(); 
				Trunk.def = getDef(); 
				LeftArm.def = getDef(); 
				RightArm.def = getDef(); 
				LeftLeg.def = getDef(); 
				LeftArm.def = getDef(); 
				Tail.def = getDef();
			}
			else {
				//no legs
				upperBody.emplace(std::make_pair<std::string, float>("Head", 0.40f));
				lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.50f));
				lowerBody.emplace(std::make_pair<std::string, float>("Tail", 0.10f));


				Tail.bodyName = "Tail";


				lowerHitBox.push_back(Tail);
				

				Head.def = getDef(); 
				Trunk.def = getDef(); 
				Tail.def = getDef();
			}
		
		}
		else if (!hasLegs) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.50f));
			lowerBody.emplace(std::make_pair<std::string, float>("Trunk", 0.50f));

			Head.HP = 0.30f * (getHP());
			Trunk.HP = 0.40f * (getHP());

			upperHitBox.push_back(Head);
			lowerHitBox.push_back(Trunk);

		
			
			Head.def = getDef();
			Trunk.def = getDef();
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

			
			
			LeftArm.bodyName = "Left Arm";
			RightArm.bodyName = "Right Arm";
			LeftLeg.bodyName = "Left Leg";
			RightLeg.bodyName = "Right Leg";
			Tail.bodyName = "Tail";

			lowerHitBox.push_back(RightLeg);
			lowerHitBox.push_back(LeftLeg);
			lowerHitBox.push_back(Tail);
			upperHitBox.push_back(LeftArm);
			upperHitBox.push_back(RightArm);
		

			Head.def = getDef();
			Trunk.def = getDef(); 
			LeftArm.def = getDef(); 
			RightArm.def = getDef(); 
			LeftLeg.def = getDef(); 
			LeftArm.def = getDef(); 
			Tail.def = getDef();
		}
		

		upperHitBox.push_back(Head);
		lowerHitBox.push_back(Trunk);
		

		Head.bodyName = "Head";
		Trunk.bodyName = "Trunk";
		
		return;
	}

	if (bodyType == "Ooze") {
		//ooze requires complete destruction
		upperBody.emplace(std::make_pair<std::string, float>("Torso", 1.00f));
		
		Torso.HP = 1.00f * (getHP());
		Torso.bodyName = "Torso";
		Torso.def = getDef();
		upperHitBox.push_back(Torso);
		return;
	}
	if (bodyType == "Beast") {
		upperBody.emplace(std::make_pair<std::string, float>("Head", 0.20f));
		upperBody.emplace(std::make_pair<std::string, float>("Torso", 0.36f));
		upperBody.emplace(std::make_pair < std::string, float>("Left Arm", 0.11f));
		upperBody.emplace(std::make_pair < std::string, float>("Right Arm", 0.11f));

		lowerBody.emplace(std::make_pair<std::string, float>("Right Leg", 0.11f));
		lowerBody.emplace(std::make_pair<std::string, float>("Left Leg", 0.11f));

		Head.HP = 0.20f * (getHP());
		Torso.HP = 0.36f * (getHP());
		LeftArm.HP = 0.11f * (getHP());
		RightArm.HP = 0.11f * (getHP());
		LeftLeg.HP = 0.11f * (getHP());
		RightLeg.HP = 0.11f * (getHP());

		Head.def = getDef();
		Torso.def = getDef();
		LeftArm.def = getDef();
		RightArm.def = getDef();
		LeftLeg.def = getDef();
		RightLeg.def = getDef();

		Head.bodyName = "Head";
		Torso.bodyName = "Torso";
		LeftArm.bodyName = "Left Arm";
		RightArm.bodyName = "Right Arm";
		LeftLeg.bodyName = "Left Leg";
		RightLeg.bodyName = "Right Leg";

		lowerHitBox.push_back(RightLeg);
		lowerHitBox.push_back(LeftLeg);

		upperHitBox.push_back(Head);
		upperHitBox.push_back(Torso);
		upperHitBox.push_back(LeftArm);
		upperHitBox.push_back(RightArm);
		upperHitBox.push_back(LeftLeg);
		upperHitBox.push_back(RightLeg);


		return;
	}
	if (bodyType == "Plant") {
		//will never have wings
		if (hasLegs) {
			upperBody.emplace(std::make_pair<std::string, float>("Head", 0.20f));
			upperBody.emplace(std::make_pair<std::string, float>("Torso", 0.36f));
			upperBody.emplace(std::make_pair < std::string, float>("Left Arm", 0.11f));
			upperBody.emplace(std::make_pair < std::string, float>("Right Arm", 0.11f));

			lowerBody.emplace(std::make_pair<std::string, float>("Right Leg", 0.11f));
			lowerBody.emplace(std::make_pair<std::string, float>("Left Leg", 0.11f));

			Head.HP = 0.20f * (getHP());
			Torso.HP = 0.36f * (getHP());
			LeftArm.HP = 0.11f * (getHP());
			RightArm.HP = 0.11f * (getHP());
			LeftLeg.HP = 0.11f * (getHP());
			RightLeg.HP = 0.11f * (getHP());

			Head.def = getDef();
			Torso.def = getDef();
			LeftArm.def = getDef();
			RightArm.def = getDef();
			LeftLeg.def = getDef();
			RightLeg.def = getDef();

			Head.bodyName = "Head";
			Torso.bodyName = "Torso";
			LeftArm.bodyName = "Left Arm";
			RightArm.bodyName = "Right Arm";
			LeftLeg.bodyName = "Left Leg";
			RightLeg.bodyName = "Right Leg";

			lowerHitBox.push_back(RightLeg);
			lowerHitBox.push_back(LeftLeg);

			upperHitBox.push_back(Head);
			upperHitBox.push_back(Torso);
			upperHitBox.push_back(LeftArm);
			upperHitBox.push_back(RightArm);
			upperHitBox.push_back(LeftLeg);
			upperHitBox.push_back(RightLeg);


			return;
		}
		upperBody.emplace(std::make_pair<std::string, float>("Head", 0.40f));
		lowerBody.emplace(std::make_pair<std::string, float>("Torso", 0.60f));
		

		Head.HP = 0.40f * (getHP());
		Torso.HP = 0.60f * (getHP());
	

		Head.def = getDef();
		Torso.def = getDef();
		

		Head.bodyName = "Head";
		Torso.bodyName = "Torso";
	

		

		upperHitBox.push_back(Head);
		lowerHitBox.push_back(Torso);
		


		return;
	}
}
void Character::setHasLegs(bool legs) {
	hasLegs = legs;
}
void Character::setHasWings(bool wings) {
	hasWings = wings;
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

void Character::lessThan(const std::string& subject, int checker, int result, Character& ch) {
	if (subject == "HP") {
		if (result < 0) {
			std::cout << "subtracting\n";
			ch.setHP(ch.getHP() + result);
			return;
		}
		else {
			if (ch.getHP() < checker) {
				ch.setHP(result);
				return;
			}
		}
		std::cout << " The opponent has too much health to complete\n";
	}
}
