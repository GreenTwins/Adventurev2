#include "Skills.h"

/*
		SKILLS DATA
*/

Skills::Skills() {

}
Skills::Skills(const std::string& sN, const std::string& rT, float rP, const std::string& sT, const std::string& sE, float sEA, int aA, const std::string& aT, int sL, const std::string& bR) {
	skillName = sN;
	requirementType = rT;
	requirementPayment = rP;
	SkillType = sT;
	skillEffect = sE;
	skillEffectAmt = sEA;
	atkAmt = aA;
	applicationType = aT;
	skillLvl = sL;
	bodyReq = bR;
}
Skills::Skills(const Skills& o) {
	skillName = o.skillName;
	requirementType = o.requirementType;
	requirementPayment = o.requirementPayment;
	SkillType = o.SkillType;
	skillEffect = o.skillEffect;
	skillEffectAmt = o.skillEffectAmt;
	atkAmt = o.atkAmt;
	applicationType = o.applicationType;
}
Skills& Skills::operator=(const Skills& other) {
	if (this != &other) {
		// Copy the state of 'other' to 'this'
		skillName = other.skillName;
		requirementType = other.requirementType;
		requirementPayment = other.requirementPayment;
		SkillType = other.SkillType;
		skillEffect = other.skillEffect;
		skillEffectAmt = other.skillEffectAmt;
		atkAmt = other.atkAmt;
		applicationType = other.applicationType;
	}
	return *this;
}
Skills::Skills(Skills&& o)noexcept {
	skillName = o.skillName;
	requirementType = o.requirementType;
	requirementPayment = o.requirementPayment;
	SkillType = o.SkillType;
	skillEffect = o.skillEffect;
	skillEffectAmt = o.skillEffectAmt;
	atkAmt = o.atkAmt;
	applicationType = o.applicationType;
	bodyReq = o.bodyReq;
	skillLvl = o.skillLvl;
}

Skills & Skills::operator=(Skills && other) noexcept {
	if (this != &other) {
		// Move-assign members from 'other' to 'this'
		skillName = std::move(other.skillName);
		requirementType = std::move(other.requirementType);
		requirementPayment = other.requirementPayment;
		SkillType = std::move(other.SkillType);
		skillEffect = std::move(other.skillEffect);
		skillEffectAmt = other.skillEffectAmt;
		atkAmt = other.atkAmt;
		applicationType = std::move(other.applicationType);
		bodyReq = std::move(other.bodyReq);
		skillLvl = std::move(other.skillLvl);
		// No need to release resources of 'other' because it's a move operation
	}
	return *this;
}
Skills::~Skills() {

}
bool Skills::operator==(const Skills& o)const {
	return (this->skillName == o.skillName);
}
void Skills::setBodyReq(std::string bR) {
	bodyReq = bR;
}
void Skills::setSkillLvl(int sL) {
	skillLvl = sL;
}
void Skills::updateSkillName(std::string sN) {
	skillName = sN;
}
void Skills::updateSkillType(std::string sT) {
	SkillType = sT;
}
void Skills::updateSkillEffectAmt(int sEA) {
	skillEffectAmt = sEA;
}
void Skills::updaterequirementPayment(int rP) {
	requirementPayment = rP;
}
void Skills::updateatkAmt(int aA) {
	atkAmt = aA;
}
void Skills::updateSkillEffect(std::string sE) {
	skillEffect = sE;
}
void Skills::updateReqType(std::string rT) {
	requirementType = rT;
}
void Skills::setapplicationType(std::string aT) {
	applicationType = aT;
}
void Skills::setSkillID(int id) {
	skillID = id;
}
std::string Skills::getSkillName()const {
	return skillName;
}

std::string Skills::getRequirementType()const {
	return requirementType;
}
int Skills::getRequirementPayment()const {
	return requirementPayment;
}
std::string Skills::getSkillType()const {
	return SkillType;
}
std::string Skills::getSkillEffect()const {
	return skillEffect;
}
int Skills::getSkillEffectAmt()const {
	return skillEffectAmt;
}
int Skills::getatkAmt()const {

	return atkAmt;
}
std::string Skills::getAppType()const {
	return applicationType;
}
int Skills::getSkillID()const {
	return skillID;
}
std::string Skills::getBodyReq()const {
	return bodyReq;
}
int Skills::getSkillLvl()const {
	return skillLvl;
}