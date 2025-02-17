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

Part2State Part2State::sPart2State;

vector<Tower *> towers;
vector<DummyCritter *> critters;

const int TOWER_BUTTON_COUNT = 5;
const int X_TOWER_BUTTON_SPACING = 50;
const int Y_TOWER_BUTTON_SPACING = 10;

const int STANDARD_TOWER_COST = 25;
const int CANNON_TOWER_COST = 100;
const int RAPID_FIRE_TOWER_COST = 50;

const int DUMMY_CRITTER_COUNT = 4;

TowerButton towerButtons[TOWER_BUTTON_COUNT];
int currentTowerSelection = -1;
bool sellTower = false;
bool upgradeTower = false;

int coins = 700;

//Hollow exit state
Part2State* Part2State::get() {
	return &sPart2State;
}

bool Part2State::enter() {
	bool success = true;

	cout << "Starting coins: " << coins << "\n" << endl;

	SDL_Color textColor { 0x00, 0x00, 0x00, 0xFF };

	const char* buttonLabels[TOWER_BUTTON_COUNT] = {
		"regular",
		"rapid fire",
		"cannon",
		"sell",
		"upgrade"
	};

	for (int i = 0; i < TOWER_BUTTON_COUNT; ++i)
	{
		if (!towerButtons[i].setText(buttonLabels[i], textColor))
		{
			printf("Failed to set button text: %s\n", buttonLabels[i]);
			success = false;
		}
	}

	critters.push_back(new DummyCritter(100, 100, 40));
	critters.push_back(new DummyCritter(300, 100, 40));
	critters.push_back(new DummyCritter(100, 300, 40));
	critters.push_back(new DummyCritter(300, 300, 40));

	return success;
}

bool Part2State::exit() {
	return true;
}

void Part2State::handleEvent(SDL_Event& e) {
	bool buttonClick = false;

	for (int i = 0; i < TOWER_BUTTON_COUNT; i++)
	{
		towerButtons[i].handleEvent(&e);

		// Check if any of the buttons were clicked and set the next state accordingly
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if (towerButtons[i].isClicked())
			{
				// resets all options
				currentTowerSelection = -1;
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

	// if clicking on existing tower
	for (int i = 0; i < towers.size(); i++) {
		//towers[i]->handleEvent(&e);

		// Check if any of the buttons were clicked and set the next state accordingly
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if (towers[i]->isClicked())
			{
				if (sellTower)
				{
					coins += towers[i]->getRefundValue();
					towers.erase(towers.begin() + i);
					cout << "Current coins: " << coins << "\n" << endl;
				}
				if (upgradeTower)
				{
					int upgradeCost = towers[i]->getUpgradeCost();
					if (coins >= upgradeCost) {
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

	// if clicking on scenery
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		if (currentTowerSelection >= 0) {
			float x = -1.f, y = -1.f;
			SDL_GetMouseState(&x, &y);

			x -= Tower::TOWER_SIZE / 2;
			y -= Tower::TOWER_SIZE / 2;

			switch (currentTowerSelection)
			{
			case 0:
				if (coins >= STANDARD_TOWER_COST) {
					coins -= STANDARD_TOWER_COST;
					cout << "Placing down StandardTower\n";
					towers.push_back(new StandardTower(x, y, STANDARD_TOWER_COST));
				}
				else
					cout << "Not enough coins for StandardTower\n";
				cout << "Current coins: " << coins << "\n" << endl;
				break;
			case 1:
				if (coins >= RAPID_FIRE_TOWER_COST) {
					coins -= RAPID_FIRE_TOWER_COST;
					cout << "Placing down RapidFireTower\n";
					towers.push_back(new RapidFireTower(x, y, RAPID_FIRE_TOWER_COST));
				}
				else
					cout << "Not enough coins for RapidFireTower\n";
				cout << "Current coins: " << coins << "\n" << endl;
				break;
			case 2:
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

void Part2State::update() 
{
	for (int i = 0; i < towers.size(); i++) 
	{
		if (critters.size()) 
		{
			DummyCritter* targettedCritter = towers[i]->findCritter(critters);

			if (targettedCritter)
			{
				towers[i]->shootProjectile(targettedCritter);
			}
			else 
			{
				towers[i]->clearProjectiles();
			}
		}
		else
		{
			towers[i]->clearProjectiles();
		}
	}

	for (int i = 0; i < critters.size(); i++) {
		if (critters[i]->getHealth() <= 0) 
		{
			critters.erase(critters.begin() + i);
		}
	}
}

void Part2State::render() 
{
	// generates towers
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

	for (int i = 0; i < critters.size(); i++)
	{
		critters[i]->generateCritter();
	}
}

Part2State::Part2State() {

}