#pragma once
#ifndef _CHARACTERIZATION_H_
#define _CHARACTERIZATION_H_

#include <string>
#include <vector>
#include <map>


class Character {
    std::string BodyType;
    std::string Name;
    int HP;
    int maxHP;
    int MP;
    int maxMP;
    float stamina;
    int maxStamina;
    int fatigue;
    int maxFatigue;
    float prec;
    float maxPrec;
  
   
    int dodge;
    int Str;
    int maxStr;
    int Def;
    int maxDef;
    int Spd;
    int maxSpd;
    int Int;
    int maxInt;
    int dex;
    int maxDex;
    int enD;
    int maxEnD;
    //bodyPart specials
    bool hasWings;
    bool hasLegs;
public:
    Character();
    Character(std::string, std::string);
    Character(const Character&);
    Character(Character&&)noexcept;
    ~Character();

    //getters
    std::string getBodyType() const;
    std::string getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getMP() const;
    int getMaxMP() const;
    float getStamina() const;
    int getMaxStamina() const;
    int& guageFatigue();
    int getFatigue()const;
    int getMaxFatigue() const;
    float getPrec() const;
    float getMaxPrec() const;
    int getDodge()const;
    int getStr() const;
    int getmaxStr()const;
    int getDef() const;
    int getmaxDef()const;
    int getSpd() const;
    int getmaxSpd()const;
    int getEnd()const;
    int getmaxEnd()const;
    int getInt()const;
    int getmaxInt()const;
    int getDex()const;
    int getmaxDex()const;
    bool withWings()const;
    bool withLegs()const;
    int getUpperBodysize()const;
    int getLowerBodysize()const;

    //in game calc
    float calculatePrec(int, int&, int, int, int); //handles fatigue
    float calculateStamin(int, int,int);
    void gaugeFatigue();


    // Setters
    void setBodyType(std::string);
    void setName(std::string);
    void setMP(int);
    void setMaxMp(int);
    void setHP(int);
    void setMaxHP(int);
    void setStamina(float);
    void setMaxStamina(int);
    void setFatigue(int);
    void setMaxFatigue(int);
    void setPrec(float);
    void setMaxPrec(float);
    void setDodge(int);
    void setStr(int);
    void setmaxStr(int);
    void setDef(int);
    void setmaxDef(int);
    void setSpd(int);
    void setmaxSpd(int);
    void setInt(int);
    void setmaxInt(int);
    void setDex(int);
    void setmaxDex(int);
    void setEnD(int);
    void setmaxEnD(int);
    void setHasLegs(bool);
    void setHasWings(bool);
  
    //attacks
    
    std::map<std::string, float>upperBody;
    std::map<std::string, float>lowerBody;


    struct bodyPart {
        float HP;
        std::string bodyName;
        int def;
        bool hasArmor;
        int armorDef;
        //or is it holding a weapon
        bool hasWeapon;
    };
    bodyPart Head, Torso, LeftArm, LeftLeg, RightArm, RightLeg, Trunk, Tail, Thorax, Abdomen;
    std::vector<bodyPart>upperHitBox;
    std::vector<bodyPart>lowerHitBox;
    //Take dmg
    void takeUpperBodyDmg(int);
    void takeLowerBodyDmg();
    void takePreciseDmg();

    //skillEffect
    void lessThan(const std::string&, int, int, Character&);
    void greaterThan(const std::string&, int, int, Character&);
    void equalTo(const std::string&, int, int, Character&);
    std::map<std::string, std::is_function<void(const std::string&, int, int, Character&)>>operatorFunctions;
    void parseAndExecute(const std::string&, Character&);


};
#endif