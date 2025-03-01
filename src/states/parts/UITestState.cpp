/**
 * @class UITestState
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

#include <states/parts/UITestState.h>
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
#include <ui/DetailDisplay.h>
#include <ui/DetailAttributeDisplay.h>

using namespace std;

/**
 * @brief UITestState singleton
 */
UITestState UITestState::sUITestState;

vector<Tower *> towers; /** @brief vector containing towers */
vector<DummyCritter *> critters; /** @brief vector containing testing critters */

const int STANDARD_TOWER_COST = 25; /** @brief gold cost for standard tower */
const int CANNON_TOWER_COST = 100; /** @brief gold cost for cannon tower */
const int RAPID_FIRE_TOWER_COST = 50; /** @brief gold cost for rapid fire tower */

const int DUMMY_CRITTER_COUNT = 4; /** @brief number of testing critters */

int coins = 700; /** @brief coins for testing buying, upgrading, and selling */









DetailAttributeDisplay detailDisplay;









/**
 * @brief Get the singleton instance of Part2State
 * @return Pointer to the Part2State singleton
 */
UITestState* UITestState::get() {
	return &sUITestState;
}

/**
 * @brief Initialize the state
 *
 * @details Creates all 5 buttons for towers: 3 tower classes, upgrade, sell
 * Creates 4 DummyCritter objects for testing towers
 *
 * @return true if initialization was successful
 */
bool UITestState::enter() {
	bool success = true;

	// prints starting amount of coins
	cout << "Starting coins: " << coins << "\n" << endl;

	SDL_Color textColor { 0x00, 0x00, 0x00, 0xFF };

	// creates critters 
	critters.push_back(new DummyCritter(100, 100, 40));
	critters.push_back(new DummyCritter(300, 100, 40));
	critters.push_back(new DummyCritter(100, 300, 40));
	critters.push_back(new DummyCritter(300, 300, 40));

	// Creates display
	detailDisplay = DetailAttributeDisplay::DetailAttributeDisplay();
	detailDisplay.initializeComponents();

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
bool UITestState::exit() {
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
void UITestState::handleEvent(SDL_Event& e) 
{

	std::vector<DetailDisplayComponent*> components = detailDisplay.getComponents();

	for (int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<DetailButton*>(components[i]) != NULL)
		{
			if (dynamic_cast<DetailButton*>(components[i])->isClicked() && 
				e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
				e.button.button == SDL_BUTTON_LEFT)
			{
				cout << i << endl;
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
void UITestState::update()
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
void UITestState::render()
{
	detailDisplay.render();
	
	// generates towers and projectiles associated with towers
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->generateTower();
		towers[i]->generateAllProjectiles();
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
UITestState::UITestState() {

}