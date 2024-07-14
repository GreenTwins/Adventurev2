
/*
ATTACK CLASS (struct)
*/
#pragma once
#ifndef _ATTACK_H_
#define _ATTACK_H_

struct AttackMod {
	int effectAmt; //aka duration
	int atkAmt; //dmg
	int payment; //determines price
	std::string atkType; //determines  type of price and if it can be countered
	std::string atkName;
};

#endif