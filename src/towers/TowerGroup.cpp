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

 // Constructor
TowerGroup::TowerGroup(int& playerGold, Map* map, DetailAttributeDisplay& detailDisplay)
    : playerGold(playerGold), map(map), detailDisplay(detailDisplay) {
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
void TowerGroup::update(float deltaTime, std::vector<Critter>& critters) {
	for (int i = 0; i < towers.size(); i++) {
		// Find the target critter for the current tower
		Critter* targettedCritter = towers[i]->findCritter(critters);

		// Have the tower shoot a projectile at the targeted critter
		towers[i]->shootProjectile(targettedCritter);

		if (critters.size()) {
			// Now, check the projectiles fired by this tower for collisions
			for (auto* projectile : towers[i]->getProjectiles()) {
				for (auto critter : critters) {
					if (projectile->checkCollision(critter)) {
						critter.takeDamage();
						critter.notify();
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

void TowerGroup::handleEvent(SDL_Event& e) {
	// resets tower buy selection
	bool buttonClick = false;

	bool correctCell = false;

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect currentRenderRect = map->getCurrentRenderRect();

	// Calculate scaling factor
	float mapWidth = map->cellCountX * map->PIXELS_PER_CELL;
	float mapHeight = map->cellCountY * map->PIXELS_PER_CELL;
	float scaleX = currentRenderRect.w / mapWidth;
	float scaleY = currentRenderRect.h / mapHeight;
	float scale = std::min(scaleX, scaleY);

	// Convert mouse position
	Vector2D mousePosition(
		(mouseX - currentRenderRect.x) / scale / map->PIXELS_PER_CELL,
		(mouseY - currentRenderRect.y) / scale / map->PIXELS_PER_CELL);

	// Convert mouse position to cell coordinates (integers)
	int cellX = static_cast<int>(mousePosition.x);
	int cellY = static_cast<int>(mousePosition.y);

	float cellSize = map->PIXELS_PER_CELL;
	float targetX = (cellX * cellSize + currentRenderRect.x);
	float targetY = (cellY * cellSize + currentRenderRect.y);

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
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[7]))->isClicked())
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

				map->wallCellDict[targetCell] = false;

				return;
			}
		}

		// checking if selling tower
		if ((dynamic_cast<DetailButton*>(detailDisplay.getTowerComponents()[8]))->isClicked())
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
		std::cout << "[DEBUG] Left mouse button clicked." << std::endl;
		bool towerClick = false;

		// If clicking on the current tower
		for (int i = 0; i < towers.size(); i++)
		{
			if (towers[i]->isClicked(1.5f))
			{
				std::cout << "[DEBUG] Clicked on tower index: " << i << std::endl;
				detailDisplay.selectTower(towers[i]);
				towers[i]->notify();
				towerClick = true;
			}
		}

		// If not clicking on a tower or critter, proceed with placing a tower
		if (!towerClick)
		{
			std::cout << "[DEBUG] No tower clicked. Attempting to place a new tower..." << std::endl;

			// Check if placing down a tower on a valid, unoccupied wall cell
			if (correctCell)
			{
				std::cout << "[DEBUG] Correct cell selected." << std::endl;
			}
			else
			{
				std::cout << "[DEBUG] Invalid cell selected." << std::endl;
			}

			if (towerBuySelect >= 0)
			{
				std::cout << "[DEBUG] Tower type selected for purchase: " << towerBuySelect << std::endl;
			}
			else
			{
				std::cout << "[DEBUG] No tower type selected." << std::endl;
			}

			if (map->wallCellDict.find(targetCell) != map->wallCellDict.end())
			{
				std::cout << "[DEBUG] Target cell exists in wallCellDict." << std::endl;
			}
			else
			{
				std::cout << "[DEBUG] Target cell does NOT exist in wallCellDict." << std::endl;
			}

			if (map->wallCellDict[targetCell])
			{
				std::cout << "[DEBUG] Target cell is already occupied." << std::endl;
			}
			else
			{
				std::cout << "[DEBUG] Target cell is NOT occupied." << std::endl;
			}

			if (correctCell && towerBuySelect >= 0 && map->wallCellDict.find(targetCell) != map->wallCellDict.end() && !map->wallCellDict[targetCell])
			{
				Tower* newTower = nullptr;

				// Checks for currently selected tower
				switch (towerBuySelect)
				{
				case 0: // Buy standard tower
					std::cout << "[DEBUG] Attempting to buy Standard Tower. Player gold: " << playerGold << std::endl;
					if (playerGold >= STANDARD_TOWER_COST)
					{
						playerGold -= STANDARD_TOWER_COST;
						newTower = new StandardTower(targetX, targetY, STANDARD_TOWER_COST);
						std::cout << "[DEBUG] Standard Tower purchased successfully." << std::endl;
					}
					else
					{
						std::cout << "[DEBUG] Not enough gold for Standard Tower." << std::endl;
					}
					towerBuySelect = -1;
					break;

				case 1: // Buy rapid fire tower
					std::cout << "[DEBUG] Attempting to buy Rapid Fire Tower. Player gold: " << playerGold << std::endl;
					if (playerGold >= RAPID_FIRE_TOWER_COST)
					{
						playerGold -= RAPID_FIRE_TOWER_COST;
						newTower = new RapidFireTower(targetX, targetY, RAPID_FIRE_TOWER_COST);
						std::cout << "[DEBUG] Rapid Fire Tower purchased successfully." << std::endl;
					}
					else
					{
						std::cout << "[DEBUG] Not enough gold for Rapid Fire Tower." << std::endl;
					}
					towerBuySelect = -1;
					break;

				case 2: // Buy cannon tower
					std::cout << "[DEBUG] Attempting to buy Cannon Tower. Player gold: " << playerGold << std::endl;
					if (playerGold >= CANNON_TOWER_COST)
					{
						playerGold -= CANNON_TOWER_COST;
						newTower = new CannonTower(targetX, targetY, CANNON_TOWER_COST);
						std::cout << "[DEBUG] Cannon Tower purchased successfully." << std::endl;
					}
					else
					{
						std::cout << "[DEBUG] Not enough gold for Cannon Tower." << std::endl;
					}
					towerBuySelect = -1;
					break;
				}

				// If a new tower was successfully created, place it in the towers list
				if (newTower != nullptr)
				{
					std::cout << "[DEBUG] New tower created and added to tower list." << std::endl;
					towers.push_back(newTower);
					detailDisplay.selectTower(newTower);

					float scaleFactor = 1.5f;
					float newSize = map->PIXELS_PER_CELL * scaleFactor;
					float offset = (newSize - map->PIXELS_PER_CELL) / 2.0f;
					newTower->setCurrentRenderedRect({ targetX - offset, targetY - offset, newSize, newSize });

					newTower->attach(detailDisplay.getTowerObserver());
					newTower->notify();

					// Mark the wall cell as occupied
					std::cout << targetCell.x << " and " << targetCell.y << std::endl;
					map->wallCellDict[targetCell] = true;
					std::cout << "[DEBUG] Target cell marked as occupied." << std::endl;
				}
				else
				{
					std::cout << "[DEBUG] Tower creation failed (possibly due to insufficient funds)." << std::endl;
				}
			}
			else
			{
				std::cout << "[DEBUG] Failed to place tower: Invalid conditions." << std::endl;
				detailDisplay.selectTower(nullptr);
			}

			return;
		}
	}
}