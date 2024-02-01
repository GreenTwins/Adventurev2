#include "Player.h"
#include <time.h>
#include <iostream>
#include <iomanip>

/*
		PLAYER CLASS DATA
*/
Player::Player() {

}
Player::Player(std::string a) :Character{ a, "Humanoid" } {
	setName(a);
	chooseAtk = false;
	if (!loaded) {
		init();
	}
	setBodyType("Humanoid");

	//load all skills
	for (auto& atks : Class->listofAllSkills) {
		addSkill(std::move(atks));
	}
	setNumAtks(); 
}

Player::Player(Player && other)
	: Character(std::move(other)),  // Call base class move constructor
	dodge(std::move(other.dodge)),
	dodgingSkill(std::exchange(other.dodgingSkill, 0.0f)),
	level(std::exchange(other.level, 0)),
	XP(std::exchange(other.XP, 0)),
	Gold(std::exchange(other.Gold, 0)),
	Class(std::move(other.Class)),
	subClass(std::move(other.subClass)),
	Job(std::move(other.Job)),
	ActiveJob(std::move(other.ActiveJob)),
	ptWorth(other.ptWorth),
	chooseAtk(other.chooseAtk),
	loaded(other.loaded) {
	// No need to call init() as the object is already initialized
}
Player& Player::operator=(Player&& other) noexcept {
	if (this != &other) {
		// Move-assign members from 'other' to 'this'
		// Example:
		dodge = std::move(other.dodge);
		dodgingSkill = other.dodgingSkill;
		level = other.level;
		XP = other.XP;
		Gold = other.Gold;
		// Move-assign the unique pointer
		Class = std::move(other.Class);
		subClass = std::move(other.subClass);
		Job = std::move(other.Job);
		ActiveJob = std::move(other.ActiveJob);
		const_cast<int&>(ptWorth) = other.ptWorth;
		chooseAtk = other.chooseAtk;
		// Move-assign base class members
		Character::operator=(std::move(other));
	}
	return *this;
}

Player::~Player() {

}
void Player::init() {
	//load req here
	compileAllStats();
	displayPrimaryStats();
	classSelection();
	loadClassSkills();
	
}
void Player::loadClassSkills() {
	//first give class bonus
	if (Class->getClassName() == "Mage") {
		setMP(getMP() + 100);
		setMaxMp(getMP());
	}
	else if (Class->getClassName() == "Warrior") {
		setHP(getHP() + 40);
		setMaxHP(getHP());
	}
	else if(Class->getClassName()=="Assassin"){
		setSpd(getSpd() + 2);

	}
	else {
		std::cout << "\nThe given class isnt a valid class. No bonus given";
		return;
	}
	//then display available skills
	std::cout << "\nYour bonus has been distributed\n";
}
void Player::compileAllStats() {
	AutocreateStats();
	//stitch stats to make HP, MP, stamina, perc, fatigue
	int HPamt = (ptWorth * (getEnd()));
	setHP(HPamt + 50);
	setMaxHP(HPamt + 50);
	int MPamt = (ptWorth * (getInt()));
	setMP(MPamt + 50);
	setMaxMp(MPamt + 50);
	setFatigue(0);
	float staminaamt = (((1.0f/3.0f) * (ptWorth *getDex()) + ((1.0f / 3.0f) * (ptWorth*getEnd()))));
	//std::cout << staminaamt << "\n";
	setStamina(10 + staminaamt);
	setMaxStamina(getStamina());
	int fatHolder = getFatigue();
	float prec = calculatePrec(getStamina(), fatHolder, getMaxStamina(), getDex(), getInt());
	setPrec(prec);
	setXP(0);
	setLvl(1);
	setGold(0);
	setActiveJob("None");
}
float Player::calculatePrec(int stamina, int& fatigue, int MaxStamina, int dex, int intel) {
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
void Player::AutocreateStats() {
	srand(time(NULL));
	int atkRoll = rand() % 6 + 1;
	int defRoll = rand() % 6 + 1;
	int spdRoll = rand() % 6 + 1;
	int dodgeRoll = rand() % 6 + 1;
	int dexRoll = rand() % 6 + 1;
	int intelRoll = rand() % 6 + 1;
	int endurRoll = rand() % 6 + 1;

	char choice;

	std::cout << "Str: " << atkRoll << "\n";
	std::cout << "Def: " << defRoll << "\n";
	std::cout << "Spd: " << spdRoll << "\n";
	std::cout << "Dodge: " << dodgeRoll << "\n";
	std::cout << "Dexterity: " << dexRoll << "\n";
	std::cout << "Intelligence: " << intelRoll << "\n";
	std::cout << "Endurance: " << endurRoll << "\n";

	std::cout << "\n These are your initial stats. Do you want to keep them?: [Y/N] ";
	std::cin >> choice;
	if (choice == 'Y' || choice == 'y') {
		setStr(atkRoll);
		setDef(defRoll);
		setSpd(spdRoll);
		setDodge(dodgeRoll);
		setDex(dexRoll);
		setInt(intelRoll);
		setEnD(endurRoll);
	}
	else {
		int total = atkRoll + defRoll + spdRoll + dexRoll + intelRoll + endurRoll;
		CustomizecreateStats(total);
	}
}
void Player::CustomizecreateStats(int total) {
	char choice;
	do {
		CustomizeStats(total, 0);
		std::cout << "\n These are your current stats: \n";
		std::cout << "Strength: " << getStr() << "\n";
		std::cout << "Defense: " << getDef() << "\n";
		std::cout << "Speed: " << getSpd() << "\n";
		std::cout << "Dexterity: " << getDex() << "\n";
		std::cout << "Intelligence: " << getInt() << "\n";
		std::cout << "Endurance: " << getEnd() << "\n";
		std::cout << "Keep these stats?[Y/N]";
		std::cin >> choice;
	} while (choice != 'Y' && choice != 'y');
}
void Player::CustomizeStats(int amt, int loc) {
	if (loc > 5) {
		return;
	}
	int statamt;
	std::vector<std::string>stats = { "Str","Def", "Spd", "Dex", "Intel", "endur" };
	std::cout << "You have a total of: " << amt << " stat points. ";
	std::cout << stats[loc] << " : ";
	std::cin >> statamt;
	if (statamt <= amt) {
		switch (loc) {
		case 0:
			setStr(statamt);
			break;
		case 1:
			setDef(statamt);
			break;
		case 2:
			setSpd(statamt);
			break;
		case 3:
			setDex(statamt);
			break;
		case 4:
			setInt(statamt);
			break;
		case 5:
			setEnD(statamt);
			break;
		default:
			std::cout << "\n Stats no saved\n";
			break;
		}
		int newAmt = amt - statamt;
		CustomizeStats(newAmt, loc + 1);
	}


}

void Player::displayAllStats()const {
	int padding = (45 - 5) / 2; // (total width - title width) / 2

	// Display basic information
	//std::cout << std::string(45, '-') << "\n";
	//std::cout << std::setw(padding) << "" << "STATS\n";
	//std::cout << std::left << std::setw(20) << "Name: " << std::right << std::setw(25) << getName() << "\n";
	//std::cout << std::left << std::setw(20) << "Class: " << std::right << std::setw(25) << getClass() << "\n";
	//std::cout << std::left << std::setw(20) << "SubClass: " << std::right << std::setw(25) << getSubClass() << "\n";
	//std::cout << std::left << std::setw(20) << "Job: " << std::right << std::setw(25) << getActiveJob() << "\n";
	//std::cout << std::left << std::setw(20) << "Lvl: " << std::right << std::setw(25) << getLvl() << "\n";
	//std::cout << std::left << std::setw(20) << "XP: " << std::right << std::setw(25) << getXP() << "\n";
	//std::cout << std::left << std::setw(20) << "Gold: " << std::right << std::setw(25) << getGold() << "\n";
	//std::cout << std::string(45, '-') << "\n";

	//// Display detailed stats
	//std::cout << std::left << std::setw(20) << "HP: " << std::right << std::setw(25) << getHP() << "\n";
	//std::cout << std::left << std::setw(20) << "MP: " << std::right << std::setw(25) << getMP() << "\n";
	//std::cout << std::left << std::setw(20) << "Stamina: " << std::right << std::fixed << std::setprecision(2) << std::setw(15) << getStamina() << "\n";
	//std::cout << std::left << std::setw(20) << "Fatigue: " << std::right << std::setw(25) << getFatigue() << "\n";
	//std::cout << std::left << std::setw(20) << "Precision: " << std::right << std::fixed << std::setprecision(2) << std::setw(15) << getPrec() << "%\n";

	//// Display secondary stats
	//std::cout << std::string(45, '-') << "\n";
	//std::cout << std::left << std::setw(20) << "Str: " << std::right << std::setw(25) << getStr() << "\n";
	//std::cout << std::left << std::setw(20) << "Def: " << std::right << std::setw(25) << getDef() << "\n";
	//std::cout << std::left << std::setw(20) << "Spd: " << std::right << std::setw(25) << getSpd() << "\n";
	//std::cout << std::left << std::setw(20) << "Int: " << std::right << std::setw(25) << getInt() << "\n";
	//std::cout << std::left << std::setw(20) << "Endur: " << std::right << std::setw(25) << getEnd() << "\n";
	//std::cout << std::left << std::setw(20) << "Dex: " << std::right << std::setw(25) << getDex() << "\n";
	//std::cout << std::string(45, '-') << "\n";



	std::cout << std::string(45, '-') << "\n";
	std::cout << std::setw(padding) << "" << "STATS\n";
	std::cout << std::left << std::setw(5) << "Name:" << getName() << std::setw(25-getName().length()-5) << std::right << " "   << "HP:" << getHP() << "\n";
	std::cout << std::left << std::setw(7) << "Class:" << getClass() << std::setw(25-getClass().length()-7) << std::right << " "  <<  "MP:" << getMP() << "\n";
	std::cout << std::left << std::setw(10) << "SubClass:" << getSubClass() << std::setw(25-getSubClass().length() - 10) << std::right << " " << "Stamina:" << std::fixed << std::setprecision(2) << getStamina() << "\n";
	std::cout << std::left << std::setw(5) << "Job:" << getActiveJob() << std::setw(25-getActiveJob().length()-5) << std::right << " "   << "Fatigue:" << getFatigue() << "\n";
	std::cout << std::left << std::setw(5) << "Lvl:" << getLvl() << std::setw(25-(std::to_string(getLvl())).size()-5) << std::right << " " << "Precision:" << std::fixed << std::setprecision(2) << getPrec() << "%\n";
	std::cout << std::left << std::setw(4) << "XP:" << getXP()<<std::setw(25-(std::to_string(getXP())).size()-4) << std::right << " "  << "Gold:" << getGold() << "\n";
	std::cout << std::string(45, '-') << "\n";

	// Display detailed stats 

	// Display secondary stats
	std::cout << std::string(45, '-') << "\n";
	std::cout << std::left << std::setw(20) << "Str: " << std::right << std::setw(25) << getStr() << "\n";
	std::cout << std::left << std::setw(20) << "Def: " << std::right << std::setw(25) << getDef() << "\n";
	std::cout << std::left << std::setw(20) << "Spd: " << std::right << std::setw(25) << getSpd() << "\n";
	std::cout << std::left << std::setw(20) << "Int: " << std::right << std::setw(25) << getInt() << "\n";
	std::cout << std::left << std::setw(20) << "Endur: " << std::right << std::setw(25) << getEnd() << "\n";
	std::cout << std::left << std::setw(20) << "Dex: " << std::right << std::setw(25) << getDex() << "\n";
	std::cout << std::string(45, '-') << "\n";

	//Display skills
	std::cout << std::setw(padding) << "" << "ATTACKS\n";
	auto displaySkill = [&](const Skills& skill, int index) {
		if (auto* magePtr = dynamic_cast<Mage*>(Class.get())) {
			if (magePtr->getSubClassName() == "Gambler") {
				std::cout << std::left << index << ".)" << std::setw(25) << skill.getSkillName()  // Adjusted setw values
					<< std::right << "Attack Amt" << " : "<< magePtr->getGambleatk(skill.getSkillName()) << "\n";
			}
			else {
				std::cout << std::left << index << ".)" << std::setw(25) << skill.getSkillName()  // Adjusted setw values
					<< std::right << "Attack Amt" << " : " << skill.getatkAmt() << "\n";
			}
		}
		else {
			std::cout << std::left << index << ".)" << std::setw(25) << skill.getSkillName()  // Adjusted setw values
				<< std::right << "Attack Amt" << " : " << skill.getatkAmt() << "\n";
		}
	};

	for (int i = 0; i < listofSkills.size(); ++i) {
		displaySkill(listofSkills[i], i + 1);
	}
	std::cout << std::string(45, '-') << "\n";

	//Display skills
	std::cout << std::setw(padding) << "" << "SKILLS\n";
	auto displayClassSkill = [&](const Skills& skill, int index) {
		std::cout << std::left << index << ".)" << std::setw(15) << skill.getSkillName()  // Adjusted setw values
		<< std::right << "Type" << " : " << skill.getSkillType() <<" Desc: "<<skill.getSkillEffect() <<"\n";
	};
	
	for (int i = 0; i < Class->listofClassSpecificSkills.size(); ++i) {
		displayClassSkill(Class->listofClassSpecificSkills[i], i + 1);
	}

	std::cout << std::string(45, '-') << "\n";
}

void Player::displayPrimaryStats()const {
	int padding = (45 - 5) / 2; // (total width - title width) / 2
	std::cout << std::string(45, '-') << "\n";
	std::cout << std::setw(padding) << "" << "STATS\n";
	std::cout << std::left << std::setw(20) << "HP: " << std::right << std::setw(25) << getHP() << "\n";
	std::cout << std::left << std::setw(20) << "MP: " << std::right << std::setw(25) << getMP() << "\n";
	std::cout << std::left << std::setw(20) << "Stamina: " << std::right << std::fixed << std::setprecision(2) << std::setw(15) << getStamina() << "\n";
	std::cout << std::left << std::setw(20) << "Fatigue: " << std::right << std::setw(25) << getFatigue() << "\n";
	std::cout << std::left << std::setw(20) << "Precision: " << std::right << std::fixed << std::setprecision(2) << std::setw(15) << getPrec() << "%\n";
	
	std::cout << std::string(45, '-') << "\n";
	
	std::cout << std::left << std::setw(20) << "Str: " << std::right << std::setw(25) << getStr() << "\n";
	std::cout << std::left << std::setw(20) << "Def: " << std::right << std::setw(25) << getDef() << "\n";
	std::cout << std::left << std::setw(20) << "Spd: " << std::right << std::setw(25) << getSpd() << "\n";
	std::cout << std::left << std::setw(20) << "Int: " << std::right << std::setw(25) << getInt() << "\n";
	std::cout << std::left << std::setw(20) << "Endur: " << std::right << std::setw(25) << getEnd() << "\n";
	std::cout << std::left << std::setw(20) << "Dex: " << std::right << std::setw(25) << getDex() << "\n";
	
	std::cout << std::string(45, '-') << "\n";
}


void Player::displayJobs() {
	if (Job.size() == 0) {
		std::cout << "\n You have no jobs available to equip\n";
		return;
	}
	char choice;
	for (int i = 0; i < Job.size(); ++i) {
		std::cout << i + 1 << ".) " << Job[i] << "\n";
	}
	std::cout << "Any jobs you want to equip?[Y/N]: ";
	std::cin >> choice;
	if (choice != 'Y' && choice != 'y') {
		return;
	}
	int jobChoice = 0;
	std::cout << "\n Choose the job number you want: ";
	if (std::cin >> jobChoice) {
		if (jobChoice <= Job.size()) {
			std::string jobName = Job[jobChoice - 1];
			setActiveJob(jobName);
			std::cout << "\n" << jobName << " currently active!";
			return;
		}
		else {
			std::cout << "\nYou've chosen an invalid number";
			return;
		}
	}
	else {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nYour input wasnt valid";
		return;
	}

}


void Player::classSelection() {
	int classChoice;
	bool tryAgain = true;
	std::vector<std::string> classList = { "Mage", "Warrior", "Assassin" };
	do {
		for (int i = 0; i < classList.size(); ++i) {
			std::cout << i + 1 << ".) " << classList[i] << "\n";
		}
		std::cout << "Which class will you bestow upon your character?: ";
		if (std::cin >> classChoice) {
			if (((classChoice - 1) <= classList.size()) && (classChoice > 0)) {
				std::string bestowedClass = classList[classChoice - 1];
				if (bestowedClass == "Mage") {
					Class= std::make_unique<Mage>();
					Class->distributeClassSpecific(level);
					Class->subClassSelection();
					tryAgain = false;
					break;
				}
				else if (bestowedClass == "Warrior") {
					Class = std::make_unique<Warrior>();
					Class->distributeClassSpecific(level);
					Class->subClassSelection();
					tryAgain = false;
					break;
				}
				else if (bestowedClass == "Assassin") {
					Class = std::make_unique<Assassin>();
					Class->distributeClassSpecific(level);
					Class->subClassSelection();
					tryAgain = false;
					break;
				}
				else {
					std::cout << "Error occurred\n";
				}
				
			}

		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nThe choice wasn't valid. Try again.\n";

	} while (true);

}


void Player::setLvl(int l) {
	level = l;
}
void Player::setXP(int x) {
	XP = x;
}
void Player::addJob(std::string j) {
	Job.push_back(j);
}
void Player::setGold(int g) {
	Gold = g;
}
void Player::setActiveJob(std::string aj) {
	ActiveJob = aj;
}
void Player::setClass(std::string c) {
	Class->setClassName(c);
}
void Player::setSubClass(std::string sc) {
	Class->setSubClassName(sc);
}


int Player::getXP()const {
	return XP;
}
int Player::getLvl()const {
	return level;
}

std::string Player::getActiveJob()const {
	return ActiveJob;
}
int Player::getGold()const {
	return Gold;
}
std::string Player::getClass()const {
	return Class->getClassName();
}
std::string Player::getSubClass()const {
	return Class->getSubClassName();
}


void Player::loadAtks() {
	if (!chooseAtk) {
		for (auto& atk : Class->listofAllSkills) {
			if ((atk.getSkillType() == "Attack")||(atk.getSkillType()=="Toggle")) {
				//listofSkills.push_back(std::move(atk));
				addSkill(std::move(atk));
			}
		}
		return;
	}
	int choice;
	std::cout << "Choose the attacks you want to load: ";
	//display attacks
	if (std::cin >> choice) {
		if ((choice <= Class->listofAllSkills.size())&& (choice >0)) {
			//good to go
		}
		else {
			//maqybe a throw here
		}
	}
	else {
		//didnt give good input
	}
}
/*
		CLASS STATS DATA
*/


void playerClass::setClassName(std::string cN) {
	className = cN;
}


std::string playerClass::getClassName()const {
	return className;
}
void playerClass::addClassSkill(Skills&& o) {
	std::cout << "\nNew skill " << o.getSkillName() << " added!\n";
	listofClassSpecificSkills.push_back(std::move(o));
	
 }

void playerClass::setSubClassName(std::string a) {
	subClassName = a;
}
void playerClass::addSkill(Skills&& o) {
	std::cout << "\nNew skill: " << o.getSkillName() << " added!\n";
	listofAllSkills.push_back(std::move(o));
}

std::string playerClass::getSubClassName()const {
	return subClassName;
}





void Mage::distributeClassSpecific(int lvl) {
	// Add Mage-specific information
	Skills primMageSkill1("Magic Resistance", "", 0, "Passive",
		"player takes less dmg from magic attks", 0.40f, 0, "resist");
	Skills primMageSkill2("Birthright", "", 0, "Passive",
		"Magic based attks consume less mana", 0.30f, 0, "enhance");

	//lvl 1 free atk
	Skills MagicSkill1("Water bullets", "MP", (rand() % 15 + 1), "Attack",
		": magic imbued water droplets-strong enough to pierce armor, fire at enemy "
		, 0, (rand() % 6 + 1), "Specific ");

	if (lvl == 1) {
		addClassSkill(std::move(primMageSkill1));
		addClassSkill(std::move(primMageSkill2));
		addSkill(std::move(MagicSkill1));
	}
	setClassName("Mage");
}
void Mage::subClassSelection() {
	int choice;
	std::cout << "\n";
	do {
		for (int i = 0; i < subClasses.size(); ++i) {
			std::cout << i + 1 << ".) " << subClasses[i] << "\n";
		}
		std::cout << "Which subclass do you want?: ";
		if (std::cin >> choice) {
			if ((choice <= subClasses.size()) && (choice > 0)) {
				std::string name = subClasses[choice-1];
				setSubClassName(name);
				break;
			}
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nInvalid choice. Try again";
	} while (true);
	std::vector<Skills>Skilldisplayer;
	std::string subclassN = getSubClassName();
	int _maxSkills = 0;

	int padding = (30 - 5) / 2; // (total width - title width) / 2

	std::cout << std::string(45, '-') << "\n";
	std::cout << std::setw(padding) << "" << "SKILLS\n";
	std::cout << std::string(45, '-') << "\n";
	std::cout << std::left << std::setw(20) << "Name:" << std::right << std::setw(20) << "Requirement" << "\n";
	auto displaySkill = [&](const Skills& skill, int index) {
		std::cout << std::left << index << ".)" << std::setw(30) << skill.getSkillName()  // Adjusted setw values
			<< std::right << skill.getRequirementType() << " : " << skill.getRequirementPayment() << "\n";
	};
	if (subclassN == "Wizard") {
	
		_maxSkills = 5;
		 std::vector<Skills>wizardSkills= {
			 {"Fireball", "MP", 4, "Attack","summons a ball of fire", 0, (rand() % 6 + 1), " "},
			 {"Water Whip", "MP", 4, "Attack",": water comes to your call and forms a weapon", 0, (rand() % 6 + 1), " "},
			 {"Gale", "MP", 10, "Attack","calls forth wind spirits", 0, (rand() % 6 + 1), " "},
			 {"Magic Missle", "MP", 12, "Attack",": bolts of energy shoot out towards your enemy", 0, 5, " "},
			 {"Call to chaos", "MP", 15, "Attack",": a call from oblivion, fate chooses your spell", 0, (rand() % 8 + 1), " "},
			 {"Magic Mark", "MP", 8, "Attack",": a curse is bestowed upon your enemy", 0, (rand() % 6 + 1), " "},
			 {"Guiding Bolt", "MP", 10, "Attack",": from the heavens comes a gift of pain for your enemy", 0, (rand() % 6 + 1), " "},
			 {"Magic Mark", "MP", 8, "Attack",": a curse is bestowed upon your enemy", 0, (rand() % 6 + 1), " "}
		};

		for (int i = 0; i < wizardSkills.size(); ++i) {
			displaySkill(wizardSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(wizardSkills[i]));
		}
	
	}
	else if (subclassN == "Paladin") {
		_maxSkills = 5;
		std::vector<Skills> paladinSkills = {
			{"Holy Water shot", "MP", 5, "Attack", "holy water rains down", 0, (rand() % 6 + 1), "AOE "},
			{"Blade of light slash", "MP", 10, "Attack", "a blade of pure light is conjured temporarily", 0, (rand() % 6 + 1), "Specific"},
			{"Arrows of judgement", "MP", 30, "Attack", "light arrows descend down", 0, ((rand() % 6 + 1) * (rand() % 6 + 1)), "AOE"},
			{"Magic Missle", "MP", 12, "Attack", ": bolts of energy shoot out towards your enemy", 0, 5, " "},
			{"Cutting Laser", "MP", 15, "Attack", ": a single beam of focussed light ", 0, (rand() % 8 + 1), "Specific"},
			{"Flash Bang", "MP", 3, "Attack", ":a bomb of light ", 0, (rand() % 6 + 1), " "},
			{"Dawn Strike", "Stamina", 2, "Attack", "a destructive melee attack", 0, (rand() % 6 + 1), "Specific "},
			{"Touch of light", "stamina", 5, "Attack", ": damage any evil that is grazed by your touch", 0, (rand() % 6 + 1), "Specific"}
		};
		
		for (int i = 0; i < paladinSkills.size(); ++i) {
			displaySkill(paladinSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(paladinSkills[i]));
		}
	}
	else if (subclassN == "Gambler") {
		_maxSkills = 5;
		
		std::vector<Skills>gamblerSkills = {
			{"Lucky Punch", "MP",1.00f*(rand() % 31), "Attack", "A skill that allows the player to punch the enemy, dealing a random amt of damage at a random cost",0,(rand() % 31), "Specific"},
			{"Cleave","MP", 1.00f * (rand() % 20), "Attack", "A possible mighty attack, that can slice space time...or a costly dud of an attack", 0,(rand() % 20),"AOE" },
			{"Lacerate", "MP", 1.00f * (rand() % 10),"Attack", "A stolen slashing technique from the Warrior class. Unknown damage...",0,(rand() % 10), "AOE"},
			{"Rend","MP", 1.00f * (rand() % 5), "Attack","A ball of energy that eviscerates the enemy..or is a harmless gust of wind..", 0,(rand() % 5),"AOE"},
			{"Slot Machine", "MP", 30,"Attack", "Throw mana away in the hope that equivalent damage is dealt to enemy", 0,(rand() % 31),"AOE"},
			{"Deconstruct", "MP",1.00f * (rand()%40), "Attack","An ultimate technique that can tear the enemy asunder", 0, (rand()%40), "AOE"},
			{"Phishing for flames", "MP",1.00f * (rand()%5),"Attack", "A light weight tactic, used in hopes of scoring heavy damage",0 ,  (rand()%5), "Specific"},
			{"Jackpot", "MP",1.00f * (rand()%8),"Attack","A sweet deal. Mana flows at random for cheap for quick damage", 0,(rand()%8+1),"AOE"}
		};

		gamblersDelight.insert({ "Lucky Punch", 31 });
		gamblersDelight.insert({ "Cleave", 20 });
		gamblersDelight.insert({ "Lacerate", 10 });
		gamblersDelight.insert({ "Rend", 5 });
		gamblersDelight.insert({ "Slot Machine", 30 });
		gamblersDelight.insert({ "Deconstruct", 40 });
		gamblersDelight.insert({ "Phishing for flames", 5 });
		gamblersDelight.insert({ "Jackpot", 8 });
		

		for (int i = 0; i < gamblerSkills.size(); ++i) {
			displaySkill(gamblerSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(gamblerSkills[i]));

		}
	}
	else {
		std::cout << "\nError";
	}
	
	std::cout << std::string(45, '-') << "\n";



	int skillchoice;
	std::string done;
	int subclassSkillsize = Skilldisplayer.size();
	std::cout << "You can choose up to"<<_maxSkills  <<" skills you want added:[type 'done' if you no longer want to add skills]\n";

	while (listofAllSkills.size() < _maxSkills) {
		std::string input;
		std::cin >> input;

		if (input == "done") {
			std::cout << "\nYou have a total of: " << listofAllSkills.size() << " skills to use.\n";
			break;
		}

		try {
			skillchoice = std::stoi(input); // Convert input to integer
			if (skillchoice <= 0 || skillchoice > subclassSkillsize) {
				throw std::invalid_argument("Invalid skill choice");
			}
			std::cout << "\nSkill: " << Skilldisplayer[skillchoice - 1].getSkillName() << " added!\n";
			listofAllSkills.push_back(std::move(Skilldisplayer[skillchoice - 1]));
		}
		catch (const std::invalid_argument& e) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nThat's not a valid choice. Try again.\n";
		}
	}
}
int Mage::getGambleatkmultip(std::string atkname) {
	int atkAmt = 0;
	for (const auto& pair : gamblersDelight) {
		if (pair.first == atkname) {
			atkAmt = pair.second;
		}
	}
	if (atkAmt == 0) {
		//an atk that isnt part of the gamble is randomized
		atkAmt = 6; //this is the default
	}
	return atkAmt;
}

int Mage::getGambleatk(std::string atkname) {
	int num = getGambleatkmultip(atkname);
	return rand() % num;
}

void Warrior::distributeClassSpecific(int lvl) {
	// Add Warrior-specific information
	Skills primWarriorSkill1("Natural Abuser", "", 0, "Passive",
		"all melee attks give extra damage", 0.15f, 0, "enhance");
	Skills primWarriorSkill2("Rage", "stamina", 1, "Toggle",
		"increase players str and spd", 0.20f, 0, "enhance");
	//lvl 1 free atk
	Skills WarriorSkill("Combo Rush", "Stamina", (rand() % 6 + 1), "Attack",
		":open handed attacks backed by pure innate strength ", 0, (rand() % 6 + 1), "AOE");

	if (lvl == 1) {
		addClassSkill(std::move(primWarriorSkill1));
		addClassSkill(std::move(primWarriorSkill2));
		addSkill(std::move(WarriorSkill));
	}
	setClassName("Warrior");
}
void Warrior::subClassSelection() {
	int choice;
	std::cout << "\n";
	do {
		for (int i = 0; i < subClasses.size(); ++i) {
			std::cout << i + 1 << ".) " << subClasses[i] << "\n";
		}
		std::cout << "Which subclass do you want?: ";
		if (std::cin >> choice) {
			if ((choice <= subClasses.size()) && (choice > 0)) {
				std::string name = subClasses[choice - 1];
				setSubClassName(name);
				break;
			}
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nInvalid choice. Try again";
	} while (true);

	std::vector<Skills>Skilldisplayer;
	std::string subclassN = getSubClassName();

	int _maxSkills = 0;

	int padding = (30 - 5) / 2; // (total width - title width) / 2

	std::cout << std::string(45, '-') << "\n";
	std::cout << std::setw(padding) << "" << "SKILLS\n";
	std::cout << std::string(45, '-') << "\n";
	std::cout << std::left << std::setw(20) << "Name:" << std::right << std::setw(20) << "Requirement" << "\n";
	auto displaySkill = [&](const Skills& skill, int index) {
		std::cout << std::left << index << ".)" << std::setw(30) << skill.getSkillName()  // Adjusted setw values
			<< std::right << skill.getRequirementType() << " : " << skill.getRequirementPayment() << "\n";
	};

	if (subclassN == "Swordsmen") {
		_maxSkills = 5;

		std::vector<Skills> swordsmenSkills = {
			{"Bladestorm", "Stamina", 2, "Attack", "your attack turns into a natural disaster of swinging blades", 0, (rand() % 6 + 1), "AOE "},
			{"Piercing Strike", "Stamina", 1, "Attack", ": basic bladed attack", 0, (rand() % 6 + 1), "Specific"},
			{"Barrage", "Stamina", 3, "Attack", "A rush of energy is converted to your blade", 0, (rand() % 6 + 1), " "},
			{"Lacerate", "MP", 12, "Attack", ": magical strength gathers into your blade to add extra dmg", 0, (rand() % 10 + 1), "Specific "},
			{"Slash x8", "Stamina", 15, "Attack", ": multiple slashes to the opponent", 0, (rand() % 8 + 1), " "},
			{"Bone breaker", "Stamina", 2, "Attack", ": extra strength gathers in your arms for a single strike", 0, (rand() % 6 + 1), " "},
			{"First strike", "stamina", 1, "Attack", ": way of the bladeless strike", 0, (rand() % 6 + 1), "Specific"},
			{"Wingchun: Bui Sau", "Stamina", 1, "Attack", ": a 3 finger piercing technique", 0, (rand() % 6 + 1), " "}
		};

		for (int i = 0; i < swordsmenSkills.size(); ++i) {
			displaySkill(swordsmenSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(swordsmenSkills[i]));
		}
	}
	else if (subclassN == "Hunter") {
		std::cout << "\n A unique subclass has been chosen!\n";
		std::cout << "While Hunters do not have an initial multiple skills to choose from,";
		std::cout << " they have the ability to use any weapon equipped as well as absorb skills from humanoid opponents beaten\n";

		std::vector<Skills> hunterSkills = {
			{"Bone break", "stamina", 2, "Attack", "using brute strength to put down your opponent", 0, (rand() % 6 + 1), "Specific"}
		};

		for (int i = 0; i < hunterSkills.size(); ++i) {
			displaySkill(hunterSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(hunterSkills[i]));
		}
		
	}
	else if (subclassN == "Tank") {
		_maxSkills = 4;

		std::vector<Skills>tankSkills = {
			{"Telekenetic Grab","MP", 5, "Attack", "invisible forces snatch up your enemy, slamming them to the ground", 0, (rand() % 6 + 1), "AOE"},
			{"Power smash", "Stamina",1, "Attack", "Brute strength used to drive into your enemy", 0, (rand() % 6 + 1),"Specific"},
			{"Shockwave","MP", 2,"Attack","Mana flows into the muscles as you slam your fists into the ground", 0, (rand() % 6 + 1), "AOE"},
			{"Reverse Suplex","Stamina", 2, "Attack", "Youre provided strength to hoist your enemy in the air before slamming them down", 0, (rand() % 6 + 1), "AOE"},
			{"Cobra Clutch backbreaker", "Stamina", 4, "Attack", "no matter the size, all backs will be broken", 0, (rand()%6+1), "Specific"},
			{"Firemans Carry", "Stamina", 1.00f * (rand()%6+1), "Attack", "the enemy is aquanted with the ground",0,(rand() % 6 + 1),"AOE"}

		};
		for (int i = 0; i < tankSkills.size(); ++i) {
			displaySkill(tankSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(tankSkills[i]));
		}

	}
	else {
		std::cout << "\nError";
	}

	std::cout << std::string(45, '-') << "\n";



	int skillchoice;
	std::string done;
	int subclassSkillsize = Skilldisplayer.size();
	std::cout << "You can choose up to"<<_maxSkills  <<" skills you want added:[type 'done' if you no longer want to add skills]\n";

	while (listofAllSkills.size() < _maxSkills) {
		std::string input;
		std::cin >> input;

		if (input == "done") {
			std::cout << "\nYou have a total of: " << listofAllSkills.size() << " skills to use.\n";
			break;
		}

		try {
			skillchoice = std::stoi(input); // Convert input to integer
			if (skillchoice <= 0 || skillchoice > subclassSkillsize) {
				throw std::invalid_argument("Invalid skill choice");
			}
			std::cout << "\nSkill: " << Skilldisplayer[skillchoice - 1].getSkillName() << " added!\n";
			listofAllSkills.push_back(std::move(Skilldisplayer[skillchoice - 1]));
		}
		catch (const std::invalid_argument& e) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nThat's not a valid choice. Try again.\n";
		}
	}
}



void Assassin::distributeClassSpecific(int lvl) {
	// Add Assassin-specific information
	Skills primAssassinSkill1("blade apprentice", "", 0, "Passive",
		"all bladed weapons deal extra damage", 0.10f, 0, "enhance");
	Skills primAssassinSkill2("Stealth", "MP", 3, "Toggle",
		"the player is given an extra dodge", (rand() % 6 + 1), 0, "resist");
	Skills assassinSkill("Hunt", "Stamina", 2, "Attack",
		"", 0, (rand() % 6 + 1), "Specific");

	if(lvl==1){
		addClassSkill(std::move(primAssassinSkill1));
		addClassSkill(std::move(primAssassinSkill2));
		addSkill(std::move(assassinSkill));
	}
	setClassName("Assassin");
}
void Assassin::subClassSelection() {
	int choice;
	std::cout << "\n";
	do {
		for (int i = 0; i < subClasses.size(); ++i) {
			std::cout << i + 1 << ".) " << subClasses[i] << "\n";
		}
		std::cout << "Which subclass do you want?: ";
		if (std::cin >> choice) {
			if ((choice <= subClasses.size()) && (choice > 0)) {
				std::string name = subClasses[choice - 1];
				setSubClassName(name);
				break;
			}
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nInvalid choice. Try again";
	} while (true);
	
	std::vector<Skills>Skilldisplayer;
	std::string subclassN = getSubClassName();

	int _maxSkills = 0;

	int padding = (30 - 5) / 2; // (total width - title width) / 2

	std::cout << std::string(45, '-') << "\n";
	std::cout << std::setw(padding) << "" << "SKILLS\n";
	std::cout << std::string(45, '-') << "\n";
	std::cout << std::left << std::setw(20) << "Name:" << std::right << std::setw(20) << "Requirement" << "\n";
	auto displaySkill = [&](const Skills& skill, int index) {
		std::cout << std::left << index << ".)" << std::setw(30) << skill.getSkillName()  // Adjusted setw values
			<< std::right << skill.getRequirementType() << " : " << skill.getRequirementPayment() << "\n";
	};

	if (subclassN == "Rogue") {
		_maxSkills = 4;

		std::vector<Skills>rogueSkills = {
			{"Dagger rush", "Stamina", 1,"Attack", "A chained attack that pierces the target", 0, 5,"Specific"},
			{"Bloodlust", "MP", 1.00f*(rand()%20+1), "Toggle", "if HP is less than 15 set HP to 0",0,0," "},
			{"Shadow hunter", "Stamina", 2, "Attack", "lurk in the shadows to strike the opponent", 0, (rand()%6+1), "Specific"},
			{"precise kill", "Stamina", 1.00f * (rand()%6+1), "Attack", "locate pressure points to incapacitate", 0, (rand()%6+1), "Specific"},
			{"Dance with poison", "MP", 3, "Toggle", "if HP is less than 50 set HP to -3",0,0," "},
			{"Shadow Dash", "Stamina", 1.00f*(rand()%6+1), "Attack", "quick attack from darkness", 0, (rand()%6+1),"Specific"},
			{"Cobra strike", "Stamina", 2, "Attack", "lethal attack", 0, 5, "Specific"}
		};
		for (int i = 0; i < rogueSkills.size(); ++i) {
			displaySkill(rogueSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(rogueSkills[i]));
		}
		
	}
	else if (subclassN == "ShapeShifter") {
		_maxSkills = 5;

		std::vector<Skills>ShapeShifterSkills = {
			{}
		};
		for (int i = 0; i < ShapeShifterSkills.size(); ++i) {
			displaySkill(ShapeShifterSkills[i], i + 1);
			Skilldisplayer.push_back(std::move(ShapeShifterSkills[i]));
		}
	}
	else if (subclassN == "Reaper") {

	}
	else {
		std::cout << "\nError";
	}
	std::cout << std::string(45, '-') << "\n";

	int skillchoice;
	std::string done;
	int subclassSkillsize = Skilldisplayer.size();
	std::cout << "You can choose up to" << _maxSkills << " skills you want added:[type 'done' if you no longer want to add skills]\n";

	while (listofAllSkills.size() < _maxSkills) {
		std::string input;
		std::cin >> input;

		if (input == "done") {
			std::cout << "\nYou have a total of: " << listofAllSkills.size() << " skills to use.\n";
			break;
		}

		try {
			skillchoice = std::stoi(input); // Convert input to integer
			if (skillchoice <= 0 || skillchoice > subclassSkillsize) {
				throw std::invalid_argument("Invalid skill choice");
			}
			std::cout << "\nSkill: " << Skilldisplayer[skillchoice - 1].getSkillName() << " added!\n";
			listofAllSkills.push_back(std::move(Skilldisplayer[skillchoice - 1]));
		}
		catch (const std::invalid_argument& e) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nThat's not a valid choice. Try again.\n";
		}
	}
}