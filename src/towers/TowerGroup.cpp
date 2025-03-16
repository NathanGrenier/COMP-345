/**
 * @file TowerGroup.cpp
 * @brief Implementation file for TowerGroup class managing a group of towers.
 */

#include <towers/TowerGroup.h>
#include <towers/StandardTower.h>
#include <towers/RapidFireTower.h>
#include <towers/CannonTower.h>
#include <map/Map.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
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
	for (int i = 0; i < towers.size(); i++)
	{
		if (critters.size())
		{
			Critter* targettedCritter = towers[i]->findCritter(critters);

			if (targettedCritter)
			{
				// Ensure we're using the center of the tower
				float towerCenterX = towers[i]->x + towers[i]->getCurrentRenderedRect().w / 2.0f;
				float towerCenterY = towers[i]->y + towers[i]->getCurrentRenderedRect().h / 2.0f;

				// Target the center of the critter
				Vector2D dirToTarget;
				dirToTarget.x = (targettedCritter->getPosition().x + targettedCritter->getPosition().w / 2.0f) - towerCenterX;
				dirToTarget.y = (targettedCritter->getPosition().y + targettedCritter->getPosition().h / 2.0f) - towerCenterY;

				// Calculate the raw angle
				float angleRad = atan2(dirToTarget.y, dirToTarget.x);
				float angleDeg = angleRad * (180.0f / M_PI);

				// Adjust for sprite orientation (assuming "top" is default forward)
				angleDeg += 90.0f;

				// Smooth rotation logic
				float currentRotation = towers[i]->getRotation();
				float deltaAngle = angleDeg - currentRotation;

				// Normalize delta to [-180, 180] for shortest path
				while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
				while (deltaAngle < -180.0f) deltaAngle += 360.0f;

				// Calculate max rotation step this frame
				float maxRotationStep = 180.0f * 0.016f;

				// Clamp rotation delta to avoid sudden jumps
				if (deltaAngle > maxRotationStep) deltaAngle = maxRotationStep;
				if (deltaAngle < -maxRotationStep) deltaAngle = -maxRotationStep;

				// Apply smooth rotation
				float newRotation = currentRotation + deltaAngle;
				towers[i]->setRotation(newRotation);

				// Shoot if aligned close enough (optional threshold)
				if (fabs(deltaAngle) < 2.0f) // Optional: only shoot if facing roughly toward target
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
	Cell targetCell;

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

	// if click happens
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
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
			if (playerGold >= upgradeCost)
			{

				// checks if tower is already max level
				if (detailDisplay.getTowerObserver()->getCurrentTower()->upgrade())
				{
					playerGold -= upgradeCost;
				}

				map->wallCellDict[targetCell] = false;
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
					int towerCellX = (towers[i]->x - currentRenderRect.x) / map->getPixelPerCell().w;
					int towerCellY = (towers[i]->y - currentRenderRect.y) / map->getPixelPerCell().h;

					int towerIndex = towerCellX + towerCellY * map->cellCountX;

					towers.erase(towers.begin() + i);

					map->wallCellDict[targetCell] = false;
				}
			}
			detailDisplay.selectTower(nullptr);
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
			// If placing down a tower on a valid, unoccupied wall cell
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
						newTower = new StandardTower(targetX, targetY, STANDARD_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				case 1: // Buy rapid fire tower
					if (playerGold >= RAPID_FIRE_TOWER_COST)
					{
						playerGold -= RAPID_FIRE_TOWER_COST;
						newTower = new RapidFireTower(targetX, targetY, RAPID_FIRE_TOWER_COST);
					}
					towerBuySelect = -1;
					break;
				case 2: // Buy cannon tower
					if (playerGold >= CANNON_TOWER_COST)
					{
						playerGold -= CANNON_TOWER_COST;
						newTower = new CannonTower(targetX, targetY, CANNON_TOWER_COST);
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
					float newSize = map->PIXELS_PER_CELL * scaleFactor;
					float offset = (newSize - map->PIXELS_PER_CELL) / 2.0f;
					newTower->setCurrentRenderedRect({ targetX - offset, targetY - offset, newSize, newSize });

					newTower->attach(detailDisplay.getTowerObserver());
					newTower->notify();

					// Mark the wall cell as occupied
					map->wallCellDict[targetCell] = true;
				}
			}
			else
			{
				detailDisplay.selectTower(nullptr);
			}
		}
	}
}