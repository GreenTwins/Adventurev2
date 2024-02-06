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
  
   
    std::vector<int>dodge;
    std::map<std::string, float>upperBody;
    std::map<std::string, float>lowerBody;
    int Str;
    int Def;
    int Spd;
    int Int;
    int dex;
    int enD;
    //bodyPart specials
    bool hasWings;
    bool hasLegs;
public:
    Character();
    Character(std::string, std::string);
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
    int getFatigue() const;
    int getMaxFatigue() const;
    float getPrec() const;
    float getMaxPrec() const;
    std::vector<int> getDodge() const;
    int getStr() const;
    int getDef() const;
    int getSpd() const;
    int getEnd()const;
    int getInt()const;
    int getDex()const;
  


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
    void setDef(int);
    void setSpd(int);
    void setInt(int);
    void setDex(int);
    void setEnD(int);
    
    //attacks
    
  


    struct bodyPart {
        float HP;
        int def;
        bool hasArmor;
        float armorDef;
        //or is it holding a weapon
        bool hasWeapon;
    };
    bodyPart Head, Torso, LeftArm, LeftLeg, RightArm, RightLeg, Trunk, Tail, Thorax, Abdomen;

    //Take dmg
    void takeUpperBodyDmg(int);
    void takeLowerBodyDmg();
    void takePreciseDmg();
};
#endif