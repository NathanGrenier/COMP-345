/**
 * @class UITestState
 * @brief Driver file for testing assignment 2, part 1
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details Class that shows Tower behaviors
 * Select to buy a Tower, upgrade a Tower, or sell a tower using the buttons on the left.
 * After selecting a Tower class, left clicking will place a single Tower and reduce gold
 * After selecting a Tower, upgrading a Tower will upgrade it by 1 level, using gold
 * After selecting a Tower, selling a Tower will delete it, refunding a set amount of gold
 * Tower attributes are shown on the right
 */

#include <states/parts/UITestState.h>
#include <Global.h>
#include <ui/LButton.h>
#include <vector>
#include <vector>

#include "../towers/Tower.cpp"
#include "../towers/CannonTower.cpp"
#include "../towers/RapidFireTower.cpp"
#include "../towers/StandardTower.cpp"
#include "../towers/Projectile.cpp"
#include "../ui/DetailButton.cpp"
#include <iostream>
#include <ui/DetailDisplay.h>
#include <ui/DetailAttributeDisplay.h>

using namespace std;

/**
 * @brief UITestState singleton
 */
UITestState UITestState::sUITestState;

vector<Tower*> towers; /** @brief vector containing towers */
vector<Critter*> critters; /** @brief vector containing testing critters */

const int STANDARD_TOWER_COST = 25; /** @brief gold cost for standard tower */
const int CANNON_TOWER_COST = 100; /** @brief gold cost for cannon tower */
const int RAPID_FIRE_TOWER_COST = 50; /** @brief gold cost for rapid fire tower */

const int DUMMY_CRITTER_COUNT = 4; /** @brief number of testing critters */

int coins = 7000; /** @brief coins for testing buying, upgrading, and selling */

DetailAttributeDisplay detailDisplay; /** @brief ui display for details */
int towerBuySelect; /** @brief selected Tower as index */

Tower* dummyStandardTower; /** @brief dummy StandardTower for details before buying Tower */
Tower* dummyRapidFireTower; /** @brief dummy RapidFireTower for details before buying Tower */
Tower* dummyCannonTower; /** @brief dummy CannonTower for details before buying Tower */


/**
 * @brief Get the singleton instance of UITestState
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
	// prints starting amount of coins
	cout << "Starting coins: " << coins << "\n" << endl;
	towerBuySelect = -1;

	// Creates display
	detailDisplay = DetailAttributeDisplay::DetailAttributeDisplay();
	bool success = detailDisplay.initializeComponents();

	// creating dummy Towers
	dummyStandardTower = new StandardTower(0, 0, 0, 12);
	dummyRapidFireTower = new RapidFireTower(0, 0, 0, 25);
	dummyCannonTower = new CannonTower(0, 0, 50);

	TowerObserver* towerObserver = detailDisplay.getTowerObserver();

	// adding dummy Towers to display values before buying Towers
	towerObserver->addToBuyTowers(dummyStandardTower);
	towerObserver->addToBuyTowers(dummyRapidFireTower);
	towerObserver->addToBuyTowers(dummyCannonTower);

	// attaching dummy Towers to Observer
	dummyStandardTower->attach(towerObserver);
	dummyRapidFireTower->attach(towerObserver);
	dummyCannonTower->attach(towerObserver);

	return success;
}

/**
 * @brief Cleans up UITestState
 *
 * @details Deallocates DefaultCritters and Towers
 *
 * @return true towers and critters were deallocated
 * @return false if could not deallocate either towers or critters
 */
bool UITestState::exit() {
	// deallocates Tower objects
	for (int i = 0; i < towers.size(); i++)
	{
		delete towers[i];
	}
	towers.clear();

	// deallocates DummyCritter objects
	for (Critter* critter : critters) 
	{
		delete critter;
	}
	critters.clear();

	return true;
}

/**
 * @brief Handles events to process in UITestState
 *
 * @details If buy Tower button is clicked on, updates the selection
 * If left clicking on scenery, places selected tower if previously pressed on a Tower selection
 * If left clicking on tower, shows Tower details, allowing upgrading and selling of that specific Tower
 *
 * @param e the SDL event
 */
void UITestState::handleEvent(SDL_Event& e)
{
	// resets tower buy selection
	bool buttonClick = false;

	// handles hovering, clicking of buttons
	detailDisplay.handleButtonEvents(&e);

	// if click happens
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		// checking if buying tower
		std::vector<DetailDisplayComponent*> components = detailDisplay.getComponents();
		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<DetailButton*>(components[i]) != NULL)
			{
				if (dynamic_cast<DetailButton*>(components[i])->isClicked())
				{
					towerBuySelect = i - 1;
					buttonClick = true;
					
					// displays different values from dummy Towers
					switch (i)
					{
					case 1:
						detailDisplay.selectTower(dummyStandardTower);
						dummyStandardTower->notify();
						break;
					case 2:
						detailDisplay.selectTower(dummyRapidFireTower);
						dummyRapidFireTower->notify();
						break;
					case 3:
						detailDisplay.selectTower(dummyCannonTower);
						dummyCannonTower->notify();
						break;
					}
				}
			}
		}

		// checking if upgrading tower
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[7]))->isClicked())
		{
			buttonClick = true;
			int upgradeCost = detailDisplay.getTowerObserver()->getCurrentTower()->getUpgradeCost();

			// checks if enough coins for upgrade
			if (coins >= upgradeCost) {

				// checks if tower is already max level
				if (detailDisplay.getTowerObserver()->getCurrentTower()->upgrade())
				{
					coins -= upgradeCost;
				}
			}

			cout << "Current coins: " << coins << "\n" << endl;

		}

		// checking if selling tower
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[8]))->isClicked())
		{
			buttonClick = true;
			coins += detailDisplay.getTowerObserver()->getCurrentTower()->getRefundValue();

			// find tower to erase 
			for (int i = 0; i < towers.size(); i++)
			{
				if (towers[i] == detailDisplay.getTowerObserver()->getCurrentTower())
				{
					towers.erase(towers.begin() + i);
				}
			}
			
			detailDisplay.selectTower(nullptr);

			cout << "Current coins: " << coins << "\n" << endl;
		}
	}

	// check if clicking
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		bool towerClick = false;
		
		// if clicking on current tower
		for (int i = 0; i < towers.size(); i++) {
			//towers[i]->handleEvent(&e);
			if (towers[i]->isClicked())
			{
				detailDisplay.selectTower(towers[i]);
				towers[i]->notify();
				towerClick = true;
			}
		}

		// if not clicking on a tower
		if (!towerClick)
		{
			// if placing down tower
			if (towerBuySelect >= 0) {
				float x = -1.f, y = -1.f;
				SDL_GetMouseState(&x, &y);

				Tower* newTower = nullptr;

				// checks for currently selected tower
				switch (towerBuySelect)
				{
				case 0: // buy standard tower
					if (coins >= STANDARD_TOWER_COST) {
						coins -= STANDARD_TOWER_COST;
						newTower = new StandardTower(x, y, STANDARD_TOWER_COST);
					}

					cout << "Current coins: " << coins << "\n" << endl;
					towerBuySelect = -1;
					break;
				case 1: // buy rapid fire tower
					if (coins >= RAPID_FIRE_TOWER_COST) {
						coins -= RAPID_FIRE_TOWER_COST;
						newTower = new RapidFireTower(x, y, RAPID_FIRE_TOWER_COST);
					}

					cout << "Current coins: " << coins << "\n" << endl;
					towerBuySelect = -1;
					break;
				case 2: // buy cannon tower
					if (coins >= CANNON_TOWER_COST) {
						coins -= CANNON_TOWER_COST;
						newTower = new CannonTower(x, y, CANNON_TOWER_COST);
					}

					cout << "Current coins: " << coins << "\n" << endl;
					towerBuySelect = -1;
					break;
				}

				if (newTower != nullptr)
				{
					towers.push_back(newTower);
					detailDisplay.selectTower(newTower);
					newTower->attach(detailDisplay.getTowerObserver());
					newTower->notify();
				}
			}
			else
			{
				detailDisplay.selectTower(nullptr);
			}
		}
	}
}

/**
 * @brief Handles updates to UITestState
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
			Critter* targettedCritter = nullptr;

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
		if (critters[i]->getHitPoints() <= 0)
		{
			critters.erase(critters.begin() + i);
		}
	}
}

/**
 * @brief Renders objects in UITestState
 *
 * @details Generates towers that are placed down, along with their projectiles
 * Generates display for Tower details
 * Generates DummyCritters
 */
void UITestState::render()
{
	// renders display
	detailDisplay.render();

	// generates towers and projectiles associated with towers
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->generateTower();
		towers[i]->generateAllProjectiles();
	}
}
/**
 * @brief UITestState default constructor
 */
UITestState::UITestState() {

}