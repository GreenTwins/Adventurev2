#include <iostream>
#include <string>
#include "Player.h"
//#include "Game.h"

int main() {

	//Game& newGame = Game::getinstance();
	//std::string inputName;
	//std::cout << "What do you want to name your character?: ";
	//std::getline(std::cin, inputName);
	//newGame.createPlayer(inputName);
	//
	//std::map<std::string, int>playeratk = newGame.playerN.attack();

	//for (auto& atkData : playeratk) {
	//	std::string atkName = atkData.first;
	//	int atkDmg = atkData.second;
	//	if (atkName == " ") {
	////check if we can redo it but if not then its a whiff
	//	std::cout << "You've made an attempt to attack but lack the ability to complete\n";

	//	}
	//	else if (atkName == "Effect") {
	////////		//use function to read said effect

	//	}
	//	else {
	//		int totaldmg = atkDmg + newGame.playerN.getStr();
	//		std::cout << newGame.playerN.getName() << " uses " << atkName;
	//		std::cout << "\n Total attack power: " << totaldmg << "\n";
	//	}
	//}

	//newGame.playerN.displayAllStats();


	Player p("Blue");
	p.displayAllStats();
	
	return 0;
}