/**
 * @file TowerGroup.cpp
 * @brief Implementation file for TowerGroup class managing a group of towers.
 */

#include <towers/TowerGroup.h>
#include <towers/StandardTower.h>
#include <towers/RapidFireTower.h>
#include <towers/CannonTower.h>
#include <map/Map.h>

#include <ui/DetailButton.h>
#include <towers/TargetNearExit.h>
#include <towers/TargetWeakest.h>
#include <towers/TargetStrongest.h>
#include <towers/TargetNearTower.h>

 // Constructor
TowerGroup::TowerGroup(int& playerGold, Map* map, DetailAttributeDisplay& detailDisplay)
    : playerGold(playerGold), map(map), detailDisplay(detailDisplay) {
	// creating dummy Towers
	dummyStandardTower = new StandardTower(0, 0, 0, 0, 12);
	dummyRapidFireTower = new RapidFireTower(0, 0, 0, 0, 25);
	dummyCannonTower = new CannonTower(0, 0, 0, 50);

	TowerObserver* towerObserver = detailDisplay.getTowerObserver();

	// adding dummy Towers to display values before buying Towers
	towerObserver->addToBuyTowers(dummyStandardTower);
	towerObserver->addToBuyTowers(dummyRapidFireTower);
	towerObserver->addToBuyTowers(dummyCannonTower);

	// attaching dummy Towers to Observer
	dummyStandardTower->attach(towerObserver);
	dummyRapidFireTower->attach(towerObserver);
	dummyCannonTower->attach(towerObserver);


	// initializing TowerStrategy objects
	strategies = new TowerStrategy * [TowerObserver::STRATEGY_COUNT];
	strategies[0] = new TargetNearExit;
	strategies[1] = new TargetNearTower;
	strategies[2] = new TargetStrongest;
	strategies[3] = new TargetWeakest;
}

// Destructor
TowerGroup::~TowerGroup() {
    // Clean up dynamically allocated towers
    for (Tower* tower : towers) {
        delete tower;
    }
    towers.clear();
}

// Add a tower to the group
void TowerGroup::addTower(Tower* tower) {
}

// Remove a tower from the group
void TowerGroup::removeTower(Tower* tower) {
}

// Update all towers (e.g., shooting critters)
void TowerGroup::update(float deltaTime, std::vector<Critter*> critters) {
	for (int i = 0; i < towers.size(); i++) {
		towers[i]->generateAllProjectiles();

		// Find the target critter for the current tower
		Critter* targettedCritter = towers[i]->findCritter(critters);

		// Have the tower shoot a projectile at the targeted critter
		towers[i]->shootProjectile(targettedCritter);

		if (critters.size()) {
			// Now, check the projectiles fired by this tower for collisions
			for (auto* projectile : towers[i]->getProjectiles()) {
				for (auto critter : critters) {
					if (projectile->checkCollision(critter)) {
						critter->takeDamage(projectile->getDamage());
						critter->notify();
						projectile->destroy(); // Destroy the projectile on collision
					}
				}
			}
		}
	}
}


// Render all towers
void TowerGroup::render() {
	for (auto& tower : towers)
	{
		tower->render();
	}
}

// Access the list of towers
std::vector<Tower*>& TowerGroup::getTowers() {
    return towers;
}

// Find and return the tower at a specific position (for selecting/upgrading)
Tower* TowerGroup::getTowerAtPosition(float x, float y, float scaleFactor) {
    return nullptr;
}

// Upgrade a tower (if possible)
void TowerGroup::upgradeTower(Tower* tower) {
}

/**
 * @brief Converts the TowerStrategy of a Tower to match the index in the strategies array
 */
int TowerGroup::getStrategyIndex(Tower* tower)
{
	TowerStrategy* critterTargettingStrategy = tower->getCritterTargettingStrategy();
	if (dynamic_cast<TargetNearExit*>(critterTargettingStrategy))
	{
		return 0;
	}
	else if (dynamic_cast<TargetNearTower*>(critterTargettingStrategy))
	{
		return 1;
	}
	else if (dynamic_cast<TargetStrongest*>(critterTargettingStrategy))
	{
		return 2;
	}
	else if (dynamic_cast<TargetWeakest*>(critterTargettingStrategy))
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

void TowerGroup::handleEvent(SDL_Event& e) {
	// resets tower buy selection
	bool buttonClick = false;

	bool correctCell = false;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect currentRenderRect = map->getCurrentRenderRect();

	// Calculate scaling factor
	float mapWidth = map->cellCountX * map->getPixelPerCell();
	float mapHeight = map->cellCountY * map->getPixelPerCell();

	// Convert mouse position
	Vector2D mousePosition(
		(mouseX - currentRenderRect.x) / map->getPixelPerCell(),
		(mouseY - currentRenderRect.y) / map->getPixelPerCell());

	// Convert mouse position to cell coordinates (integers)
	int cellX = static_cast<int>(mousePosition.x);
	int cellY = static_cast<int>(mousePosition.y);

	// Snap the target position to the grid cell
	float cellSize = map->getPixelPerCell();
	float targetX = cellX * cellSize + currentRenderRect.x;
	float targetY = cellY * cellSize + currentRenderRect.y;

	// if click happens
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{

		// Ensure valid index range
		if (cellX < 0 || cellX >= map->cellCountX || cellY < 0 || cellY >= map->cellCountY)
		{
			correctCell = false;
		}
		else
		{
			// Compute the index for accessing the cell
			int index = cellX + cellY * map->cellCountX;
			targetCell = map->cells[index];
			correctCell = true;
		}

		// checking if buying tower
		std::vector<DetailDisplayComponent*> components = detailDisplay.getComponents();

		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<DetailButton*>(components[i]) != nullptr)
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
						return;
					case 2:
						detailDisplay.selectTower(dummyRapidFireTower);
						dummyRapidFireTower->notify();
						return;
					case 3:
						detailDisplay.selectTower(dummyCannonTower);
						dummyCannonTower->notify();
						return;
					}
				}
			}
		}

		// checking if upgrading tower
		if (detailDisplay.getTowerObserver()->getCurrentTower() != nullptr && (dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[7]))->isClicked())
		{
			buttonClick = true;
			int upgradeCost = detailDisplay.getTowerObserver()->getCurrentTower()->getUpgradeCost();

			// checks if enough coins for upgrade
			if (playerGold >= upgradeCost)
			{
				// checks if tower is already max level
				if (detailDisplay.getTowerObserver()->getCurrentTower()->upgrade())
				{
					playerGold -= upgradeCost;
				}


				return;
			}
		}

		// checking if changing TowerStrategy
		int startingIndex = TowerObserver::TOWER_COMPONENT_COUNT - TowerObserver::STRATEGY_COUNT + 1; 
		Tower* currentTower = detailDisplay.getTowerObserver()->getCurrentTower();
		if (currentTower != nullptr && (dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[10]))->isClicked())
		{
			int strategyIndex = getStrategyIndex(currentTower);
			++strategyIndex %= TowerObserver::STRATEGY_COUNT;
			currentTower->setCritterTargettingStrategy(strategies[strategyIndex]);

			return;
		}

		// checking if selling tower
		if (detailDisplay.getTowerObserver()->getCurrentTower() != nullptr && (dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[8]))->isClicked())
		{
			buttonClick = true;
			playerGold += detailDisplay.getTowerObserver()->getCurrentTower()->getRefundValue();

			// find tower to erase
			for (int i = 0; i < towers.size(); i++)
			{
				if (towers[i] == detailDisplay.getTowerObserver()->getCurrentTower())
				{
					towers.erase(towers.begin() + i);

					map->wallCellDict[targetCell] = false;
				}
			}
			detailDisplay.selectTower(nullptr);

			return;
		}
	}

	// Check if clicking on towers
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		bool towerClick = false;

		// If clicking on the current tower
		for (int i = 0; i < towers.size(); i++)
		{
			if (towers[i]->isClicked(1.5f))
			{
				detailDisplay.selectTower(towers[i]);
				towers[i]->notify();
				towerClick = true;
			}
		}

		// If not clicking on a tower or critter, proceed with placing a tower
		if (!towerClick)
		{
			if (correctCell && towerBuySelect >= 0 && map->wallCellDict.find(targetCell) != map->wallCellDict.end() && !map->wallCellDict[targetCell])
			{
				Tower* newTower = nullptr;

				// Checks for currently selected tower
				switch (towerBuySelect)
				{
				case 0: // Buy standard tower
					if (playerGold >= STANDARD_TOWER_COST)
					{
						playerGold -= STANDARD_TOWER_COST;
						newTower = new StandardTower(targetX, targetY, map->getPixelPerCell(), STANDARD_TOWER_COST);
					}
					towerBuySelect = -1;
					break;

				case 1: // Buy rapid fire tower
					if (playerGold >= RAPID_FIRE_TOWER_COST)
					{
						playerGold -= RAPID_FIRE_TOWER_COST;
						newTower = new RapidFireTower(targetX, targetY, map->getPixelPerCell(), RAPID_FIRE_TOWER_COST);
					}
					towerBuySelect = -1;
					break;

				case 2: // Buy cannon tower
					if (playerGold >= CANNON_TOWER_COST)
					{
						playerGold -= CANNON_TOWER_COST;
						newTower = new CannonTower(targetX, targetY, map->getPixelPerCell(), CANNON_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				}

				// If a new tower was successfully created, place it in the towers list
				if (newTower != nullptr)
				{
					towers.push_back(newTower);
					detailDisplay.selectTower(newTower);

					float scaleFactor = 1.5f;
					float newSize = map->getPixelPerCell() * scaleFactor;
					newTower->setCurrentRenderRect(targetX, targetY, newSize, newSize);

					newTower->attach(detailDisplay.getTowerObserver());
					newTower->notify();

					// Mark the wall cell as occupied
					std::cout << targetCell.x << " and " << targetCell.y << std::endl;

					map->wallCellDict[targetCell] = true;
				}
			}
			else
			{
				detailDisplay.selectTower(nullptr);
			}

			return;
		}
	}
}