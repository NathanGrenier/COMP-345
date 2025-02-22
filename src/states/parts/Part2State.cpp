/**
 * @file Part2State.cpp
 * @brief Driver file for assignment 1, part 2
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class that shows tower behaviors
 * Select to buy a tower, upgrade a tower, or sell a tower using the buttons on the left. 
 * After selecting a tower, left clicking will place it and reduce gold
 * After selecting upgrade, left clicking a tower will upgrade it by 1 level, using gold 
 * After selecting sell, left clicking a tower will delete it, refunding a set amount of gold
 * Gold is shown through console as display is shown in Part3
 */

#include <states/parts/Part2State.h>
#include <Global.h>
#include <ui/TowerButton.h>
#include <vector>

//#include <towers/Tower.h>
//#include <towers/CannonTower.h>
//#include <towers/RapidFireTower.h>
//#include <towers/StandardTower.h>
//#include <towers/Projectile.h>
//#include <towers/DummyCritter.h>

#include "../towers/Tower.cpp"
#include "../towers/CannonTower.cpp"
#include "../towers/RapidFireTower.cpp"
#include "../towers/StandardTower.cpp"
#include "../towers/DummyCritter.cpp"
#include "../towers/Projectile.cpp"
#include <iostream>

using namespace std;

/**
 * @brief Part2State singleton
 */
Part2State Part2State::sPart2State;

vector<Tower *> towers; /** @brief vector containing towers */
vector<DummyCritter *> critters; /** @brief vector containing testing critters */

const int TOWER_BUTTON_COUNT = 5; /** @brief number of buttons for testing tower behaviors */
const int X_TOWER_BUTTON_SPACING = 50; /** @brief horizontal spacing for buttons */
const int Y_TOWER_BUTTON_SPACING = 10; /** @brief vertical spacing for buttons */

const int STANDARD_TOWER_COST = 25; /** @brief gold cost for standard tower */
const int CANNON_TOWER_COST = 100; /** @brief gold cost for cannon tower */
const int RAPID_FIRE_TOWER_COST = 50; /** @brief gold cost for rapid fire tower */

const int DUMMY_CRITTER_COUNT = 4; /** @brief number of testing critters */

TowerButton towerButtons[TOWER_BUTTON_COUNT]; /** @brief array for buttons */
int currentTowerSelection = -1; /** @brief class selection for buying towers */
bool sellTower = false; /** @brief if selling tower */
bool upgradeTower = false; /** @brief if upgrading tower */

int coins = 700; /** @brief coins for testing buying, upgrading, and selling */

/**
 * @brief Get the singleton instance of Part2State
 * @return Pointer to the Part2State singleton
 */
Part2State* Part2State::get() {
	return &sPart2State;
}

/**
 * @brief Initialize the state
 *
 * @details Creates all 5 buttons for towers: 3 tower classes, upgrade, sell
 * Creates 4 DummyCritter objects for testing towers
 *
 * @return true if initialization was successful
 */
bool Part2State::enter() {
	bool success = true;

	// prints starting amount of coins
	cout << "Starting coins: " << coins << "\n" << endl;

	SDL_Color textColor { 0x00, 0x00, 0x00, 0xFF };

	// labels for buttons
	const char* buttonLabels[TOWER_BUTTON_COUNT] = {
		"regular",
		"rapid fire",
		"cannon",
		"sell",
		"upgrade"
	};

	// creates buttons
	for (int i = 0; i < TOWER_BUTTON_COUNT; ++i)
	{
		if (!towerButtons[i].setText(buttonLabels[i], textColor))
		{
			printf("Failed to set button text: %s\n", buttonLabels[i]);
			success = false;
		}
	}

	// creates critters 
	critters.push_back(new DummyCritter(100, 100, 40));
	critters.push_back(new DummyCritter(300, 100, 40));
	critters.push_back(new DummyCritter(100, 300, 40));
	critters.push_back(new DummyCritter(300, 300, 40));

	return success;
}

/**
 * @brief Cleans up Part2State
 *
 * @details Deallocates DefaultCritters and Towers
 *
 * @return true towers and critters were deallocated
 * @return false if could not deallocate either towers or critters
 */
bool Part2State::exit() {
	// todo: check critter and tower vectors
	return true;
}

/**
 * @brief Handles events to process in Part2State
 * 
 * @details If button is clicked on, updates the selection 
 * If left clicking on scenery, places selected tower if applicable
 * If left clicking on tower, either upgrades or sells tower based on selection
 * 
 * @param e the SDL event 
 */
void Part2State::handleEvent(SDL_Event& e) {
	bool buttonClick = false;

	// updates the selection of the button
	for (int i = 0; i < TOWER_BUTTON_COUNT; i++)
	{
		towerButtons[i].handleEvent(&e);

		// Check if any of the buttons were clicked and set the next state accordingly
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if (towerButtons[i].isClicked())
			{
				// resets all options
				currentTowerSelection = -1; // no tower selected
				sellTower = false;
				upgradeTower = false;
				buttonClick = true;
				
				switch (i)
				{
				case 0: // buy type 0 tower
					currentTowerSelection = 0;
					break;
				
				case 1: // buy type 1 tower
					currentTowerSelection = 1;
					break;
				
				case 2: // buy type 2 tower
					currentTowerSelection = 2;
					break;
				
				case 3: // sell towers
					sellTower = true;
					break;
				
				case 4: // upgrade towers
					upgradeTower = true;
					break;
				}
			}
		}
	}

	// if clicking on existing tower, attempting to upgrade a tower
	for (int i = 0; i < towers.size(); i++) {
		//towers[i]->handleEvent(&e);

		// Check if any of the buttons were clicked and set the next state accordingly
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if (towers[i]->isClicked())
			{
				// sells tower
				if (sellTower)
				{
					coins += towers[i]->getRefundValue();
					towers.erase(towers.begin() + i);
					cout << "Current coins: " << coins << "\n" << endl;
				}
				
				// upgrades tower
				if (upgradeTower)
				{
					int upgradeCost = towers[i]->getUpgradeCost();
					
					// checks if enough coins for upgrade
					if (coins >= upgradeCost) { 
						
						// checks if tower is already max level
						if (towers[i]->upgrade())
						{
							cout << "Upgrading tower\n";
							coins -= upgradeCost;
						}
						else
						{
							cout << "Could not upgrade Tower\n";
						}
					}
					else
						cout << "Not enough coins for Tower upgrade\n";
					cout << "Current coins: " << coins << "\n" << endl;
				}
			}
		}
	}

	// if clicking on scenery, attempting to buy and place a tower
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		// if tower is currently selected
		if (currentTowerSelection >= 0) {
			float x = -1.f, y = -1.f;
			SDL_GetMouseState(&x, &y);

			x -= Tower::TOWER_SIZE / 2;
			y -= Tower::TOWER_SIZE / 2;

			// checks for currently selected tower
			switch (currentTowerSelection)
			{
			case 0: // buy standard tower
				if (coins >= STANDARD_TOWER_COST) {
					coins -= STANDARD_TOWER_COST;
					cout << "Placing down StandardTower\n";
					towers.push_back(new StandardTower(x, y, STANDARD_TOWER_COST));
				}
				else
					cout << "Not enough coins for StandardTower\n";
				cout << "Current coins: " << coins << "\n" << endl;
				break;
			case 1: // buy rapid fire tower
				if (coins >= RAPID_FIRE_TOWER_COST) {
					coins -= RAPID_FIRE_TOWER_COST;
					cout << "Placing down RapidFireTower\n";
					towers.push_back(new RapidFireTower(x, y, RAPID_FIRE_TOWER_COST));
				}
				else
					cout << "Not enough coins for RapidFireTower\n";
				cout << "Current coins: " << coins << "\n" << endl;
				break;
			case 2: // buy cannon tower
				if (coins >= CANNON_TOWER_COST) {
					coins -= CANNON_TOWER_COST;
					cout << "Placing down CannonTower\n";
					towers.push_back(new CannonTower(x, y, CANNON_TOWER_COST));
				}
				else
					cout << "Not enough coins for CannonTower\n";
				cout << "Current coins: " << coins << "\n" << endl;
				break;
			}
		}
	}
}

/**
 * @brief Handles updates to Part2State
 * 
 * @details Allows towers to find critters in range and fire projectiles at those DummyCritters
 * Also deletes DummyCritters if their health reaches 0
 */
void Part2State::update() 
{
	// checks tower ranges
	for (int i = 0; i < towers.size(); i++) 
	{
		// checks if critters exist
		if (critters.size()) 
		{
			// finds a target critter
			DummyCritter* targettedCritter = towers[i]->findCritter(critters);

			// shoot if there is a critter in tower range
			if (targettedCritter)
			{
				towers[i]->shootProjectile(targettedCritter);
			}
			// deletes already fired projectiles if there is no critter in range
			else 
			{
				towers[i]->clearProjectiles();
			}
		}

		// deletes already fired projectiles if no critters
		else
		{
			towers[i]->clearProjectiles();
		}
	}

	// checks critter health
	for (int i = 0; i < critters.size(); i++) {

		// deletes critter if health is depleted
		if (critters[i]->getHealth() <= 0) 
		{
			critters.erase(critters.begin() + i);
		}
	}
}

/**
 * @brief Renders objects in Part2State
 *
 * @details Generates towers that are placed down
 * Generates buttons for selecting, selling, and upgrading towers
 */
void Part2State::render() 
{
	// generates towers and projectiles associated with towers
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->generateTower();
		towers[i]->generateAllProjectiles();
	}

	// generates buttons 
	for (int i = 0; i < TOWER_BUTTON_COUNT; i++) {
		towerButtons[i].setPosition(Global::kScreenWidth - TowerButton::kButtonWidth - X_TOWER_BUTTON_SPACING,
			i * (LButton::kButtonHeight + Y_TOWER_BUTTON_SPACING));
	}
	// Show the message
	mMessageTexture.render((Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f, 20);

	// Render buttons
	for (int i = 0; i < TOWER_BUTTON_COUNT; i++)
	{
		towerButtons[i].render();
	}

	// generates critters
	for (int i = 0; i < critters.size(); i++)
	{
		critters[i]->generateCritter();
	}
}
/**
 * @brief Part2State default constructor
 */
Part2State::Part2State() {

}