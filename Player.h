#include <string>
#include <vector>
#include "Character.h"
#include <memory>
#include <unordered_map>
#include "Skills.h"
#pragma once
//you can also do inner classes and have playerClass inside of Player in order to share data

/*
Making playerClass an inner class of Player:

Pros:

Encapsulation: Inner classes have access to the private members of their enclosing class. This can simplify access to Player's private members from within playerClass.
Logical Grouping: Placing playerClass within Player may improve code organization and readability by explicitly indicating their relationship and dependencies.
Access Control: Inner classes can have different access levels (e.g., private, protected, public) with respect to their enclosing class, allowing you to control the visibility of playerClass members.
Cons:

Tight Coupling: Inner classes are tightly coupled with their enclosing class. This can make it more difficult to reuse playerClass outside of the context of Player.
Increased Complexity: Adding inner classes can increase the complexity of the enclosing class, potentially making it harder to understand and maintain.
Compilation Time: Inner classes are typically recompiled whenever their enclosing class is modified, which may increase compilation time for large projects.
Keeping playerClass as a standalone class:

Pros:

Separation of Concerns: Keeping playerClass separate from Player promotes separation of concerns and modularity, making it easier to understand and maintain each class independently.
Reusability: Standalone classes can be reused more easily in other parts of the codebase or in different projects.
Reduced Compilation Time: Changes to playerClass do not require recompilation of Player, potentially reducing compilation time.
Cons:

Access Control: Standalone classes do not have direct access to the private members of Player, which may require additional getters and setters or friend declarations for access.
Relationship Clarity: The relationship between Player and playerClass may be less explicit, potentially leading to confusion about their association.
In summary, whether to make playerClass an inner class of Player or keep it as a standalone class depends on factors such as code organization, encapsulation requirements, and the level of coupling desired between the two classes. Evaluate these factors based on your project's needs to determine the most suitable approach.




*/



#ifndef _LIFECLASS_H_
#define _LIFECLASS_H_

class playerClass {
protected:
	std::string className;
	std::string subClassName;
public:
	virtual ~playerClass() = default;
	virtual void distributeClassSpecific(int) = 0;
	virtual void subClassSelection() = 0;
	std::vector<Skills>listofAllSkills; //what we use to atk or debuff/toggle
	std::vector<Skills>listofClassSpecificSkills; //enhance, debuff, class passive/active
	// Setters
	void setClassName(std::string);
	void setSubClassName(std::string);
	void addSkill(Skills&&);
	void addClassSkill(Skills&&);



	// Getters
	std::string getClassName() const;
	std::string getSubClassName()const;
};

class Mage : public playerClass {
public:
	std::vector<std::string>subClasses = { "Wizard","Paladin" ,"Gambler" };
	void distributeClassSpecific(int) override;
	void subClassSelection()override;

	//gambler specific
	std::unordered_multimap<std::string, float>gamblersDelight;
	/*virtual int getGambleatkmultip(std::string) = 0;
	virtual int getGambleatk(std::string) = 0;*/
	int getGambleatkmultip(std::string);
	int getGambleatk(std::string);
	//std::vector<Skills>wizardSkills;
};

class Warrior : public playerClass {
public:
	std::vector<std::string>subClasses = { "Swordsmen","Hunter" ,"Tank" };
	void distributeClassSpecific(int) override;
	void subClassSelection()override;

	//hunter speciic
	bool stealSkill(float);
	bool gluttonyActive(float, float);
};

class Assassin : public playerClass {
public:
	std::vector<std::string>subClasses = { "Rogue","ShapeShifter" ,"Reaper" };
	void distributeClassSpecific(int)  override;
	void subClassSelection()override;

};

#endif

#ifndef _NEWPLAYER_H_
#define _NEWPLAYER_H_



class Player : public Character {
	int maindodge;
	int secondarydodge;
	float dodgingSkill;
	int level;
	int XP;
	int Gold;
	std::unique_ptr<playerClass>Class;
	std::string subClass;
	std::vector<std::string> Job;
	std::string ActiveJob;
	const double ptWorth = 8.33;
	bool chooseAtk;

	size_t numofAtks;
	int location;
	int ID;

public:
	Player();
	Player(std::string);
	Player(const Player&);
	Player(Player&&);
	~Player();

	bool loaded = false;
	void init();
	//direct access
	void setLvl(int);
	void setXP(int);
	void setClass(std::string);
	void setSubClass(std::string);
	void addJob(std::string);
	void setActiveJob(std::string);
	void setGold(int);
	void setNumAtks();
	void setLocation(int);
	void setID(int);
	std::vector<Skills>listofSkills;
	std::vector<Skills>ClassSkills;

	//getter
	int getID()const;
	int getLvl()const;
	int getXP()const;
	int getGold()const;
	std::string getClass()const;
	std::string getSubClass()const;
	std::string getActiveJob()const;
	//do functions
	void AutocreateStats();
	void CustomizecreateStats(int);
	void CustomizeStats(int, int);
	void compileAllStats();
	//float calculatePrec(int, int&, int, int, int);
	//float calculateStamin(int, int);
	int getLocation()const;


	void classSelection();
	void loadClassSkills();
	

	//displays
	void displayAllStats()const;
	void displayJobs();
	void displayPrimaryStats()const;

	void loadAtks();
	void removeAtks();
	Skills& permRemoveAtk();
	void addSkill(Skills&&);
	void addClassSkill(Skills&&);
	void preLoadAllSkills();
	size_t getNumofAtks()const;
	std::map<std::string, int>attack();

	//overloads
	//Player& operator=(const Player&);
	Player& operator=(Player&& other) noexcept;
	
};


#endif

