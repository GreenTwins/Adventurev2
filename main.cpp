#include <iostream>
#include <string>
#include "Player.h"

int main() {



	std::string inputName;
	std::cout << "Give your character a name: ";
	std::getline(std::cin, inputName);

	Player newPlayer(inputName);

	newPlayer.displayAllStats();
	

	//std::map<std::string, int> playeratk=newPlayer.attack();

	//for (auto& atkData : playeratk) {
	//	std::string atkName = atkData.first;
	//	int atkDmg = atkData.second;
	//	if (atkName == " ") {
	//	//check if we can redo it but if not then its a whiff
	//	std::cout << "You've made an attempt to attack but lack the ability to complete\n";

	////	}
	////	else if (atkName == "Effect") {
	////		//use function to read said effect

	//	}
	//	else {
	//		int totaldmg = atkDmg + newPlayer.getStr();
	//		std::cout << newPlayer.getName() << " uses " << atkName;
	//		std::cout << "\n Total attack power: " << totaldmg;
	//	}
	//}
	////newPlayer.displayAllStats();
	return 0;
}