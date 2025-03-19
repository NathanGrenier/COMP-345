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

#include <towers/powerups/Powerup.h>
#include <towers/powerups/FirePowerup.h>
#include <towers/powerups/IcePowerup.h>

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
}

// Destructor
TowerGroup::~TowerGroup() {
    // Clean up dynamically allocated towers
    for (Tower* tower : towers) {
        delete tower;
    }
    towers.clear();

	for (Powerup* powerup : activePowerups) {
		delete powerup;
	}

	activePowerups.clear();
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
						// Check if critter is dead
						std::cout << critter->getHitPoints() << std::endl;
						if (critter->getHitPoints() <= 0) {
							float spawnChance = 0.2f; // 20% chance to spawn a powerup
							if (rand() % 100 < spawnChance * 100) {
								Powerup* powerup = nullptr;
								int powerupType = rand() % 2; // Randomly choose between fire or ice
								if (powerupType == 0) {
									powerup = new FirePowerup(critter->getCurrentRenderRect());
								}
								else {
									powerup = new IcePowerup(critter->getCurrentRenderRect());
								}
								activePowerups.push_back(powerup);
							}
						}
						projectile->destroy();
					}
				}
			}
		}

		for (Powerup* powerup : activePowerups) {
			powerup->update(0.0008f);
		}
	}
}



// Render all towers
void TowerGroup::render() {
	for (auto& tower : towers)
	{
		tower->render();
	}

	for (Powerup* powerup : activePowerups) {
		powerup->render();
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

	static Powerup* draggedPowerup = nullptr;
	static bool dragging = false;

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

	if (dragging && draggedPowerup) {
		if (!SDL_HasRectIntersectionFloat(&draggedPowerup->position, &Global::mapViewRect)) {
			activePowerups.erase(std::remove(activePowerups.begin(), activePowerups.end(), draggedPowerup), activePowerups.end());
			delete draggedPowerup;
			draggedPowerup = nullptr;
			dragging = false;
			return;
		}

		draggedPowerup->position.x = mouseX - (draggedPowerup->position.w / 2);
		draggedPowerup->position.y = mouseY - (draggedPowerup->position.h / 2);
	}


	// if click happens
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		for (Powerup* powerup : activePowerups) {
			if (powerup->isClicked(mouseX, mouseY)) {
				draggedPowerup = powerup;  // Start dragging powerup
				dragging = true;
				draggedPowerup->isDragged = true;
				break;
			}
		}

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
					detailDisplay.getTowerObserver()->getCurrentTower()->notify();
					playerGold -= upgradeCost;
				}

				return;
			}
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

	if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT) {
		if (dragging && draggedPowerup) {

			bool towerClicked = false;
			for (size_t i = 0; i < towers.size(); i++) {
				if (towers[i]->isClicked(1)) {
					// Unwrap the tower if it is already wrapped
					Tower* baseTower = towers[i];
					while (auto* decorated = dynamic_cast<TowerDecorator*>(baseTower)) {
						baseTower = decorated->getWrappedTower(); // Assuming getWrappedTower() exists
						delete decorated; // Free old decorator
					}

					// Apply new powerup
					Tower* upgradedTower = draggedPowerup->applyPowerupToTower(baseTower);
					towers[i] = upgradedTower; // Replace old reference

					activePowerups.erase(std::remove(activePowerups.begin(), activePowerups.end(), draggedPowerup), activePowerups.end());
					delete draggedPowerup;
					draggedPowerup->isDragged = false;
					draggedPowerup = nullptr;
					dragging = false;
					towerClicked = true;
					break;
				}
			}
			if (!towerClicked) {
				draggedPowerup->isDragged = false;
				draggedPowerup = nullptr;
				dragging = false;
			}
		}
	}

}