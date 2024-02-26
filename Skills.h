#pragma once

#include<string>

#ifndef _SKILLS_H_
#define _SKILLS_H_

class Skills {
	std::string skillName;
	std::string requirementType;
	float requirementPayment;
	std::string SkillType;
	std::string skillEffect; //if its enhancer, inhibitor, passive active skill
	float skillEffectAmt;
	int atkAmt;
	std::string applicationType;
	int skillID; //only recv through SQL
	int skillLvl; //default is 1
	std::string bodyReq;
	std::string atkType;
public:
	Skills();
	Skills(const std::string&, const std::string&, float,
		const std::string&, const std::string&, float, int, const std::string&, int, const std::string&, const std::string&);
	Skills(const Skills&);
	Skills(Skills&&)noexcept;
	Skills& operator=(const Skills&);
	Skills& operator=(Skills&&) noexcept;
	bool operator==(const Skills& o)const;
	~Skills();

	//available setters
	void updateSkillName(std::string);
	void updateSkillEffect(std::string);
	void updateSkillType(std::string);
	void updateSkillEffectAmt(int);
	void updaterequirementPayment(int);
	void updateatkAmt(int);
	void updateReqType(std::string);
	void setapplicationType(std::string);
	void setSkillID(int);
	void setBodyReq(std::string);
	void setSkillLvl(int);
	void setatkType(std::string);
	//getters
	std::string getSkillName()const;
	std::string getRequirementType()const;
	std::string getAppType()const;
	int getRequirementPayment()const;
	std::string getSkillType()const;
	std::string getSkillEffect()const;
	int getSkillEffectAmt()const;
	int getatkAmt()const;
	int getSkillID()const;
	std::string getBodyReq()const;
	int getSkillLvl()const;
	std::string getAtkType()const;

};

#endif