// TextBasedGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <time.h>
#include <algorithm>
using namespace std;

void printMap();
void revealMap();
void mapMenu();
void move();
void spaceAction();

struct character {

	vector<string> inventory;
	double health;
	int x;
	int y;


};

struct space {

	bool loot;
	bool blocked;
	bool enemyPresent;
	bool endSpace;
	bool seen;
	bool document;
	bool clue;
};

enum type {
	ARMOR, WEAPON, POTION
};
struct mapItem
{
	string name;
	type category;
	int power; // out of 100
};

space blocked = { false, true, false, false, false, false, false };
space loot = { true, false, false, false, false, false, false };
space enemy = { false, false, true, false, false, false, false };
space document = { false, false, false, false, false, true, false };
space blank = { false, false, false, false, false, false, false };
space clue = { false, false, false, false, false, false, true };

space map[6][7] = { 

					{blocked, blocked, blocked, blank, blocked, blocked, blocked},
					{ loot, blank, enemy, enemy, enemy, blank, loot },
					{ loot, document, clue, blocked, blank, document, clue },
					{ blocked, clue, blocked, blocked, blocked, blank, blocked },
					{document, enemy, blank, clue, blank, loot, enemy},
					{ loot, blocked, blocked, blank, blocked, blocked, document }

};

vector<mapItem> collectables;
character RobertSquarePants;
int const MAPROWSIZE = 6;
int const MAPCOLSIZE = 7;
int const COLLECTABLESSIZE = 7;

int main(){

	// creating map spaces --> setting properties for each space
	//	could be made to be randomized for a procedural generated map later on
	
	// setting endspaces for print out
	RobertSquarePants.x = 5;
	RobertSquarePants.y = 3;

	mapItem ZzuchiniBlaster = { "ZzucchiniBlaster", WEAPON, 25 };
	mapItem MetalStrainer = { "Titanium Food Strainer", ARMOR, 10 };
	mapItem SpagettitCannon = { "Spagettit Cannon", WEAPON, 30 };
	mapItem HealthPotion = { "Health Potion", POTION, 30 };
	mapItem PepperPoison = { "Potion of Cayenne Pepper Powder", POTION, 15 }; // poison potion
	mapItem TwistingTwizzler = { "Twisting Twizzler Spear", WEAPON, 20 };
	mapItem BreechesBaguette = { "Breeches of Baguettes", ARMOR, 50 };


	collectables = { ZzuchiniBlaster , MetalStrainer, SpagettitCannon, HealthPotion, PepperPoison, TwistingTwizzler, BreechesBaguette };

	map[0][6].endSpace = true;
	map[1][6].endSpace = true;
	map[2][6].endSpace = true;
	map[3][6].endSpace = true;
	map[4][6].endSpace = true;
	map[5][6].endSpace = true;

	revealMap();
	printMap();
	mapMenu();
    return 0;
}


void mapMenu() {
	char userInput = ' ';
	cout << "What would you like to do? E(x)it\n";
	cout << "a. MOVE  b. LOOK FOR CLUES\n";
	cin >> userInput;
		
		userInput = tolower(userInput);

		if (userInput == 'a') {

			move();
			revealMap();
			printMap();
			mapMenu();
		}
		else if (userInput != 'x'){

			cout << "INVALID INPUT. . . \n";
			mapMenu();
		}
}

void move() {
	char userInput = ' ';
	bool west = false, north = false, south = false, east = false;
	cout << "Which direction would you like to move? E(x)it\n";

	if (RobertSquarePants.y > 0 && map[RobertSquarePants.x][RobertSquarePants.y - 1].blocked != true) {

		cout << "a. WEST ";
		west = true;
	}
	if (RobertSquarePants.x > 0 && map[RobertSquarePants.x - 1][RobertSquarePants.y].blocked != true) {

		cout << "w. NORTH ";
		north = true;

	}
	if (RobertSquarePants.x + 1 < MAPROWSIZE && map[RobertSquarePants.x + 1][RobertSquarePants.y].blocked != true) {

		cout << "s. SOUTH ";
		south = true;

	}
	if (RobertSquarePants.y + 1 < MAPCOLSIZE && map[RobertSquarePants.x][RobertSquarePants.y + 1].blocked != true) {

		cout << "d. EAST ";
		east = true;

	}
	cout << "\n";
	cin >> userInput;
	userInput = tolower(userInput);

	if (userInput != 'x') {

		if (userInput == 'a' && west) {

			RobertSquarePants.y -= 1;
			spaceAction();
		}
		else if (userInput == 'w' && north) {

			RobertSquarePants.x -= 1;
			spaceAction();
		}
		else if (userInput == 's' && south) {

			RobertSquarePants.x += 1;
			spaceAction();
		}
		else if (userInput == 'd' && east) {

			RobertSquarePants.y += 1;
			spaceAction();
		}
		else {

			cout << "INVALID INPUT. . .\n";
			move();
		}
	}
	
}
void spaceAction() {
	srand(time(NULL));
	if (map[RobertSquarePants.x][RobertSquarePants.y].document) {

		cout << "You have picked up a new corporate document;";
		cout << "You are one step closer to upending this corporate villany!!\n";
		RobertSquarePants.inventory.push_back("Corporate Document");
		map[RobertSquarePants.x][RobertSquarePants.y].document = false;
	}
	else if (map[RobertSquarePants.x][RobertSquarePants.y].loot) {
		cout << "You come upon a mighty boon that will aid you in your adventure!!\n";
		int ranItem = rand() * COLLECTABLESSIZE;
		cout << collectables[ranItem].name <<" with a power of " << collectables[ranItem].power << " has been added to the inventory.\n";
		collectables.erase(std::remove(collectables.begin(), collectables.end(), collectables[ranItem]), collectables.end());
		for (int i = 0; i < COLLECTABLESSIZE; i++) {

			cout << collectables.at(i).name << endl;

		}
	}


}
void revealMap() {


	map[RobertSquarePants.x][RobertSquarePants.y].seen = true;
	if (RobertSquarePants.x < MAPROWSIZE) {
		map[RobertSquarePants.x + 1][RobertSquarePants.y].seen = true;
	}
	if (RobertSquarePants.y < MAPCOLSIZE) {

		map[RobertSquarePants.x][RobertSquarePants.y + 1].seen = true;
	}
	if (RobertSquarePants.x > 0) {

		map[RobertSquarePants.x - 1][RobertSquarePants.y].seen = true;

	}
	if (RobertSquarePants.y > 0) {

		map[RobertSquarePants.x][RobertSquarePants.y - 1].seen = true;

	}
	if (RobertSquarePants.y > 0 && RobertSquarePants.x > 0) {

		map[RobertSquarePants.x - 1][RobertSquarePants.y - 1].seen = true;

	}
	if (RobertSquarePants.y < MAPCOLSIZE && RobertSquarePants.x < MAPROWSIZE) {

		map[RobertSquarePants.x - 1][RobertSquarePants.y + 1].seen = true;

	}
	if (RobertSquarePants.y > 0 && RobertSquarePants.x < MAPROWSIZE) {

		map[RobertSquarePants.x + 1][RobertSquarePants.y - 1].seen = true;

	}
	if (RobertSquarePants.y < MAPCOLSIZE && RobertSquarePants.x > 0) {

		map[RobertSquarePants.x + 1][RobertSquarePants.y + 1].seen = true;

	}
}

void printMap() {
	std::cout << "\n\n\n\nCurrent Map:\n";

	for (int i = 0; i < MAPROWSIZE; i ++) {
		for (int j = 0; j < MAPCOLSIZE; j++) {


			if (i == RobertSquarePants.x && j == RobertSquarePants.y) {

				cout << " [P] ";

			}
			else {
				if (map[i][j].seen) {
					if (map[i][j].blocked == false) {
						if (map[i][j].loot) {

							cout << " [$] ";
						}
						else if (map[i][j].clue) {

							cout << " [?] ";

						}
						else if (map[i][j].enemyPresent) {

							cout << " [!] ";

						}
						else if (map[i][j].document) {

							cout << " [#] ";

						}
						else {

							cout << " [ ] ";

						}
					}
					else {

						cout << "  X  ";

					}
				}
				else {

					cout << " [-] ";

				}
			}
			if (map[i][j].endSpace) {

				cout << "\n";

			}
		}
	}
}

