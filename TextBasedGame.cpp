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
#include <cstdlib>

using namespace std;

void printMap();
void revealMap();
void mapMenu();
void move();
void spaceAction();

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
struct character {

	vector<string> inventory;
	double health;
	mapItem activeWeapon;
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
	vector<string> items;
};


space blocked = { false, true, false, false, false, false, false, {} };
space loot = { true, false, false, false, false, false, false, {} };
space enemy = { false, false, true, false, false, false, false, {} };
space document = { false, false, false, false, false, true, false, {} };
space blank = { false, false, false, false, false, false, false, {} };
space clue = { false, false, false, false, false, false, true , {} };

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
int const COLLECTABLESSIZE = 9;
int const TIMELEFT = 25;
int seenCount = 0;
bool endGame = false;

int main(){
	srand(time(NULL));
	
	
	// setting start position
	RobertSquarePants.x = 5;
	RobertSquarePants.y = 3;
	RobertSquarePants.health = 100;


	// creating map items
	mapItem ZzuchiniZapper = { "ZzucchiniZapper", WEAPON, 25, false };
	mapItem MetalStrainer = { "Titanium Food Strainer", ARMOR, 1, false };
	mapItem SpagettitCannon = { "Spagettit Cannon", WEAPON, 30, false };
	mapItem HealthPotion = { "Health Potion", POTION, 40, false };
	mapItem PepperPoison = { "Potion of Cayenne Pepper Powder", POTION, 25, false }; // poison potion
	mapItem TwistingTwizzler = { "Twisting Twizzler Spear", WEAPON, 20, false };
	mapItem BreechesBaguette = { "Breeches of Baguettes", ARMOR, 2, false };
	mapItem garbage = { "Literal Garbage", WEAPON, 0, false };

	mapItem defaultWeapon = { "Broccolini Blaster", WEAPON, 15, true };
	RobertSquarePants.activeWeapon = defaultWeapon;

	collectables = { ZzuchiniZapper , MetalStrainer, SpagettitCannon, garbage, HealthPotion, PepperPoison, TwistingTwizzler, BreechesBaguette , garbage};

	RobertSquarePants.inventory.push_back("Literal Garbage");
	RobertSquarePants.inventory.push_back("Health Potion");

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
	if (endGame) {

		return;
	}
	string const alphabet = "abcdefghijklmnopqrstuvxyz";
	char userInput = ' ';
	cout << "What would you like to do? E(x)it\n";
	cout << "a. MOVE  b. LOOK FOR CLUES c. INVENTORY d. PICK UP\n";
	cin >> userInput;
		
		userInput = tolower(userInput);

		if (userInput == 'a') {

			move();
			revealMap();
			printMap();
			mapMenu();
		}
		else if (userInput == 'b') {

			if (map[RobertSquarePants.x][RobertSquarePants.y].clue) {
				if (seenCount != MAPCOLSIZE * MAPROWSIZE) {
					cout << seenCount << endl;
					int randomX = 0;
					int randomY = 0;
					do {
						randomX = rand() % MAPROWSIZE;
						randomY = rand() % MAPCOLSIZE;
					} while (map[randomX][randomY].seen);


					cout << "You have found a clue to map space (" << randomX << ", " << randomY << ")\n";
					map[RobertSquarePants.x][RobertSquarePants.y].clue = false;
					map[randomX][randomY].seen = true;
				} else {
					
					cout << "Gods look fondly upon those who are great explorers..\n";
					cout << "You have been fully healed..\nSuddenly your " << RobertSquarePants.activeWeapon.name << " is engulfed in fire. \nThough the flames do not seem to hurt you..\n";
					RobertSquarePants.health = 100;
					RobertSquarePants.activeWeapon.power += 20;
					
				}
				revealMap();
				printMap();
				mapMenu();

			}
			else {

				cout << "You look through the grime and dust, yet find nothing of value here.\n";
				printMap();
				mapMenu();
			}

		}
		else if (userInput == 'c') {
			
			
			if (RobertSquarePants.inventory.size() != 0) {
				for (int i = 0; i < RobertSquarePants.inventory.size(); i++) {

					cout << alphabet[i] << ". " << RobertSquarePants.inventory.at(i) << endl;

				}
				char userInput = ' ';
				cout << "Which would you like to use?\n";
				cin >> userInput;
				userInput = tolower(userInput);

				cout << "What would you like to do with it?\n a. USE b. DROP\n"; 
				char userChoice = ' ';
				cin >> userChoice;
				userChoice = tolower(userChoice);

				if (userChoice == 'a') {
					if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Health Potion") {

						cout << "You gain strength.\n";
						RobertSquarePants.health += 40;
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Potion of Cayenne Pepper Powder") {

						cout << "You have applied poisen to your weapon.\n";
						RobertSquarePants.activeWeapon.power += 25;
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Literal Garbage") {

						cout << "You have eaten a handful of trash.  It has no effect, however, you feel objectively worse.\n";
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else {
						cout << "That is already equipped.\n";

					}
				}
				else if (userChoice == 'b') {

					cout << "You have dropped " << RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) << endl;
					swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
					map[RobertSquarePants.x][RobertSquarePants.y].items.push_back(RobertSquarePants.inventory.back());
					RobertSquarePants.inventory.pop_back();
					
				}
				printMap();
				mapMenu();
			}
			else {

				cout << "\nYour inventory is empty.\n";
				printMap();
				mapMenu();
			}

		}
		else if (userInput == 'd') {

			if (map[RobertSquarePants.x][RobertSquarePants.y].items.size() != 0) {

				for (int i = 0; i < map[RobertSquarePants.x][RobertSquarePants.y].items.size(); i++) {

					cout << alphabet[i] << ". " << map[RobertSquarePants.x][RobertSquarePants.y].items.at(i) << endl;

				}

			
			cout << "Which would you like to pick up?\n";
			cin >> userInput;
			userInput = tolower(userInput);

			RobertSquarePants.inventory.push_back(map[RobertSquarePants.x][RobertSquarePants.y].items.at(alphabet.find_first_of(userInput)));
			swap(map[RobertSquarePants.x][RobertSquarePants.y].items.at(alphabet.find_first_of(userInput)), map[RobertSquarePants.x][RobertSquarePants.y].items.back());
			map[RobertSquarePants.x][RobertSquarePants.y].items.pop_back();

			cout << "The item has been put in your inventory.\n";
			}
			else {

				cout << "There are no items on this space.\n";
			}
			printMap();
			mapMenu();
		}
		else if (userInput != 'x'){

			cout << "INVALID INPUT. . . \n";
			printMap();
			mapMenu();
		} 
}

void move() {
	if (endGame) {

		return;
	}
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
	cout << RobertSquarePants.x << " - " << RobertSquarePants.y << endl;

}

void spaceAction() {
	if (endGame) {

		return;
	}
	if (RobertSquarePants.x == 0 && RobertSquarePants.y == 3) {

		int numDocs = 0;
		for (int i = 0; i < RobertSquarePants.inventory.size(); i++) {

			if (RobertSquarePants.inventory.at(i) == "Corporate Document") {

				numDocs += 1;

			}

		}
		if (numDocs == 4) {

			cout << "You have unraveled the oil company's dasterdly scheme.  Releasing the corporate documents, you have saved your place under the pineapple under the sea.\n";
			endGame = true;
			return;
		}
		else {

			cout << "You still need " << 4 - numDocs << " corporate document(s) in order to take down the company.\n";

		}


	}

	if (map[RobertSquarePants.x][RobertSquarePants.y].document) {

		cout << "\n\nYou have picked up a new corporate document.\n";
		cout << "You are one step closer to upending this corporate villany!!\n";
		RobertSquarePants.inventory.push_back("Corporate Document");
		map[RobertSquarePants.x][RobertSquarePants.y].document = false;
	}
	else if (map[RobertSquarePants.x][RobertSquarePants.y].loot) {
		cout << "\n\nYou come upon a mighty boon that will aid you in your adventure!!\n";
		int ranItem = 0;

			ranItem = rand() % collectables.size();

		cout << collectables.at(ranItem).name << " with a power of " << collectables.at(ranItem).power << " has been added to the inventory.\n";
		if (collectables.at(ranItem).category == WEAPON) {
			if (collectables.at(ranItem).power > RobertSquarePants.activeWeapon.power) {

				RobertSquarePants.activeWeapon = collectables.at(ranItem);
				cout << collectables.at(ranItem).name << " has replaced your main weapon.\n";
			}

		}
		else {

			RobertSquarePants.inventory.push_back(collectables.at(ranItem).name);

		}
		swap(collectables.at(ranItem), collectables.back());
		collectables.pop_back();
		map[RobertSquarePants.x][RobertSquarePants.y].loot = false;
	}
	else if (map[RobertSquarePants.x][RobertSquarePants.y].enemyPresent) {
		cout << "You enter the room and see a mutant strafing from wall to wall, blisters festering on the skin.  \nYou have come across a corporate drone.\n";
		int enemyHealth = 100;
		do {


		
			cout << "Your current stats are as follows:\n";
			cout << "Health: " << RobertSquarePants.health << endl;
			cout << "Inventory: \n";
			for (int i = 0; i << RobertSquarePants.inventory.size(); i++) {

				cout << i + 1 << ":  " << RobertSquarePants.inventory.at(i) << endl;

			}


			cout << "\nWhat would you like to do?\n";
			cout << "a. Attack  b. Use Item\n";
			char userInput;
			cin >> userInput;
			userInput = tolower(userInput);
			if (userInput == 'a') {

				int dodge = rand() % 8;
				if (dodge != 1) {
					int RobertAttack = RobertSquarePants.activeWeapon.power + (rand() % 20 + 5);
					cout << "You have hit the enemy with your " << RobertSquarePants.activeWeapon.name << " with a power of " << RobertAttack << endl;
					enemyHealth -= RobertAttack;
				}
				else {

					cout << "\nThe enemy has evaded your attack.\n";
				}
				int dodgeFactor = 0;
				for (int i = 0; i < RobertSquarePants.inventory.size(); i++) {
					if (RobertSquarePants.inventory.at(i) == "Titanium Food Strainer") {
						dodgeFactor += 1;
					}
					else if (RobertSquarePants.inventory.at(i) == "Breeches of Baguettes") {
						dodgeFactor += 2;
					}
				}
				dodge = rand() % (5 - dodgeFactor);
				if (dodge != 1) {

					int enemyAttack = rand() % 30 + 10;
					cout << "The enemy has hit you with a power of " << enemyAttack << endl;
					RobertSquarePants.health -= enemyAttack;
				}
				else {

					cout << "\nYou have dodged the attack.\n";

				}
				

			}
			else if (userInput == 'b') {
				string alphabet = "abcdefghijklmnopqrstuvxyz";
				if (RobertSquarePants.inventory.size() != 0) {
					
					for (int i = 0; i < RobertSquarePants.inventory.size(); i++) {

						cout << alphabet[i] << ". " << RobertSquarePants.inventory.at(i) << endl;

					}
					char userInput = ' ';
					cout << "Which would you like to use?\n";
					cin >> userInput;
					userInput = tolower(userInput);

					if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Health Potion") {

						cout << "You gain strength.\n";
						RobertSquarePants.health += 40;
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Potion of Cayenne Pepper Powder") {

						cout << "You have applied poisen to your weapon.\n";
						RobertSquarePants.activeWeapon.power += 25;
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else if (RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)) == "Literal Garbage") {

						cout << "You have eaten a handful of trash.  It has no effect, however, you feel objectively worse.\n";
						swap(RobertSquarePants.inventory.at(alphabet.find_first_of(userInput)), RobertSquarePants.inventory.back());
						RobertSquarePants.inventory.pop_back();

					}
					else {
						cout << "That is already equipped.\n";

					}
				}
				else {

					cout << "\nYour inventory is empty.\n";

				}
			}


		} while (enemyHealth > 0 && RobertSquarePants.health > 0);
		if (RobertSquarePants.health <= 0) {

			cout << "You have perished. . . \nYour precious pineapple will soon become obliterated by drilling machines.\n";
			endGame = true;
			return;
		}
		else {

			cout << "You have succeeded in ridding this world of another monster.\nYou are able to find a health potion in the room.\n";
			RobertSquarePants.inventory.push_back("Health Potion");

		}
		map[RobertSquarePants.x][RobertSquarePants.y].enemyPresent = false;
	}


}
void revealMap() {

	if (endGame) {

		return;
	}
	map[RobertSquarePants.x][RobertSquarePants.y].seen = true;
	if (RobertSquarePants.x < MAPROWSIZE - 1) {
		map[RobertSquarePants.x + 1][RobertSquarePants.y].seen = true;
	}
	if (RobertSquarePants.y < MAPCOLSIZE - 1) {

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
	if (RobertSquarePants.y < MAPCOLSIZE - 1 && RobertSquarePants.x > 0) {

		map[RobertSquarePants.x - 1][RobertSquarePants.y + 1].seen = true;

	}
	if (RobertSquarePants.y > 0 && RobertSquarePants.x < MAPROWSIZE - 1) {

		map[RobertSquarePants.x + 1][RobertSquarePants.y - 1].seen = true;

	}
	if (RobertSquarePants.y < MAPCOLSIZE - 1 && RobertSquarePants.x < MAPROWSIZE - 1) {

		map[RobertSquarePants.x + 1][RobertSquarePants.y + 1].seen = true;

	}
}

void printMap() {
	if (endGame) {

		return;
	}
	std::cout << "\n\nCurrent Map:\n";
	seenCount = 1;
	for (int i = 0; i < MAPROWSIZE; i ++) {
		for (int j = 0; j < MAPCOLSIZE; j++) {


			if (i == RobertSquarePants.x && j == RobertSquarePants.y) {

				cout << " [P] ";

			}
			else {
				
				if (map[i][j].seen) {
					seenCount += 1;
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

