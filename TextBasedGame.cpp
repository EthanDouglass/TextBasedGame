//Ethan Douglass, etdouglass@dmacc.edu, 04/18/2019
// At this point in time, the map, movement, fog of war and the basics of the inventory system have been set up
// I am just starting in on the biggest piece of the program.  That being the actions that are taken as the player moves into new spaces
// I will be working on the formatting of the text later on (some of the prompts when items are picked up are hard to notice)

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <time.h>
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
	bool used;
};

space blocked = { false, true, false, false, false, false, false };
space loot = { true, false, false, false, false, false, false };
space enemy = { false, false, true, false, false, false, false };
space document = { false, false, false, false, false, true, false };
space blank = { false, false, false, false, false, false, false };
space clue = { false, false, false, false, false, false, true };

// creating map spaces --> setting properties for each space
	//	could be made to be randomized for a procedural generated map later on
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
	srand(time(NULL));
	
	
	// setting start position
	RobertSquarePants.x = 5;
	RobertSquarePants.y = 3;



	// creating map items
	mapItem ZzuchiniBlaster = { "ZzucchiniBlaster", WEAPON, 25, false };
	mapItem MetalStrainer = { "Titanium Food Strainer", ARMOR, 10, false };
	mapItem SpagettitCannon = { "Spagettit Cannon", WEAPON, 30, false };
	mapItem HealthPotion = { "Health Potion", POTION, 30, false };
	mapItem PepperPoison = { "Potion of Cayenne Pepper Powder", POTION, 15, false }; // poison potion
	mapItem TwistingTwizzler = { "Twisting Twizzler Spear", WEAPON, 20, false };
	mapItem BreechesBaguette = { "Breeches of Baguettes", ARMOR, 50, false };


	collectables = { ZzuchiniBlaster , MetalStrainer, SpagettitCannon, HealthPotion, PepperPoison, TwistingTwizzler, BreechesBaguette };

	// setting endspaces for print out
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
	
	if (map[RobertSquarePants.x][RobertSquarePants.y].document) {

		cout << "\n\nYou have picked up a new corporate document.\n";
		cout << "You are one step closer to upending this corporate villany!!\n";
		RobertSquarePants.inventory.push_back("Corporate Document");
		map[RobertSquarePants.x][RobertSquarePants.y].document = false;
	}
	else if (map[RobertSquarePants.x][RobertSquarePants.y].loot) {
		cout << "\n\nYou come upon a mighty boon that will aid you in your adventure!!\n";
		int ranItem;
		do {
			ranItem = rand() % COLLECTABLESSIZE;
		} while (collectables.at(ranItem).used == true);
		
		collectables.at(ranItem).used = true;
		cout << collectables.at(ranItem).name << " with a power of " << collectables.at(ranItem).power << " has been added to the inventory.\n";

		map[RobertSquarePants.x][RobertSquarePants.y].loot = false;
	}
	else if (map[RobertSquarePants.x][RobertSquarePants.y].enemyPresent) {
		cout << "You enter the room and see a mutant strafing from wall to wall, blisters festering on the skin.  \nYou have come across a corporate drone.\n";
		cout << "Your current stats are as follows:\n";
		cout << "Health: " << RobertSquarePants.health;
		cout << "Inventory: \n";
		for (int i = 0; i << RobertSquarePants.inventory.size(); i++) {

			cout << i + 1 << ":  " << RobertSquarePants.inventory.at(i) << endl;

		}
		cout << "What would you like to do?\n";
		cout << "a. Attack  b. Use Item\n";
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
	std::cout << "\n\nCurrent Map:\n";

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

