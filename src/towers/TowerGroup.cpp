/**
 * @class TowerGroup
 * @brief Manages a collection of towers, handling their placement, upgrades, and interactions.
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

#include <towers/powerups/Powerup.h>
#include <towers/powerups/FirePowerup.h>
#include <towers/powerups/IcePowerup.h>

/**
 * @brief Constructs a TowerGroup object.
 *
 * Initializes dummy towers for display and attaches them to the observer pattern.
 * Initializes TowerStrategy objects for Towers to use.
 *
 * @param playerGold Reference to player's gold amount.
 * @param map Pointer to the game map.
 * @param detailDisplay Reference to the UI detail display.
 */
TowerGroup::TowerGroup(int &playerGold, Map *map, DetailAttributeDisplay* detailDisplay)
	: playerGold(playerGold), map(map), detailDisplay(detailDisplay)
{
	// creating dummy Towers
	dummyStandardTower = new StandardTower(0, 0, 0, STANDARD_TOWER_COST);
	dummyRapidFireTower = new RapidFireTower(0, 0, 0, RAPID_FIRE_TOWER_COST);
	dummyCannonTower = new CannonTower(0, 0, 0, CANNON_TOWER_COST);

	TowerObserver *towerObserver = detailDisplay->getTowerObserver();

	// adding dummy Towers to display values before buying Towers
	towerObserver->addToBuyTowers(dummyStandardTower);
	towerObserver->addToBuyTowers(dummyRapidFireTower);
	towerObserver->addToBuyTowers(dummyCannonTower);

	// attaching dummy Towers to Observer
	dummyStandardTower->attach(towerObserver);
	dummyRapidFireTower->attach(towerObserver);
	dummyCannonTower->attach(towerObserver);

	// initializing TowerStrategy objects
	strategies = new TowerStrategy *[TowerObserver::STRATEGY_COUNT];
	strategies[0] = new TargetNearExit;
	strategies[1] = new TargetNearTower;
	strategies[2] = new TargetStrongest;
	strategies[3] = new TargetWeakest;
}

/**
 * @brief Destructor for TowerGroup.
 *
 * Cleans up dynamically allocated towers and power-ups.
 */
TowerGroup::~TowerGroup()
{
	// Clean up dynamically allocated towers
	for (Tower *tower : towers)
	{
		delete tower;
	}
	towers.clear();

	for (Powerup *powerup : activePowerups)
	{
		delete powerup;
	}

	activePowerups.clear();
}

/**
 * @brief Updates all towers and active powerups.
 *
 * Handles projectile generation, targeting, shooting, and powerup spawning.
 *
 * @param deltaTime The time elapsed since the last update.
 * @param critters List of critters in the game.
 */
void TowerGroup::update(float deltaTime, std::vector<Critter *> critters)
{
	for (int i = 0; i < towers.size(); i++)
	{
		towers[i]->generateAllProjectiles();

		// Find the target critter for the current tower
		Critter *targettedCritter = towers[i]->findCritter(critters);

		// Have the tower shoot a projectile at the targeted critter
		towers[i]->shootProjectile(targettedCritter);

		if (critters.size())
		{
			// Now, check the projectiles fired by this tower for collisions
			for (auto *projectile : towers[i]->getProjectiles())
			{
				for (auto critter : critters)
				{
					if (projectile->checkCollision(critter))
					{
						critter->takeDamage(static_cast<float>(projectile->getDamage()));
						critter->notify();
						projectile->destroy();

						// Check if critter is dead
						if (!critter->isAlive())
						{
							float spawnChance = 0.02f; // 2% chance to spawn a powerup
							if (rand() % 100 < spawnChance * 100)
							{
								Powerup *powerup = nullptr;
								int powerupType = rand() % 2; // Randomly choose between fire or ice
								if (powerupType == 0)
								{
									powerup = new FirePowerup(critter->getCurrentRenderRect());
								}
								else
								{
									powerup = new IcePowerup(critter->getCurrentRenderRect());
								}
								activePowerups.push_back(powerup);
							}
						}
					}
				}
			}
		}
	}

	for (auto it = activePowerups.begin(); it != activePowerups.end(); ) {
		Powerup* powerup = *it;
		powerup->update(0.0008f);

		if (powerup->markForDespawn) {
			delete powerup;  // Free memory
			it = activePowerups.erase(it);  // Remove from list and move iterator
		}
		else {
			++it;
		}
	}
}

/**
 * @brief Renders all towers and active powerups.
 */
void TowerGroup::render()
{
	for (auto &tower : towers)
	{
		tower->render();
	}

	for (Powerup *powerup : activePowerups)
	{
		powerup->render();
	}
}

/**
 * @brief Retrieves the list of towers.
 * @return A reference to the vector containing all towers.
 */
std::vector<Tower *> &TowerGroup::getTowers()
{
	return towers;
}

/**
 * @brief Finds and returns a tower at a given position.
 *
 * @param x The x-coordinate to check.
 * @param y The y-coordinate to check.
 * @param scaleFactor The scaling factor for grid positioning.
 * @return A pointer to the tower at the specified position, or nullptr if none exists.
 */
Tower *TowerGroup::getTowerAtPosition(float x, float y, float scaleFactor)
{
	return nullptr;
}

/**
 * @brief Upgrades a given tower.
 *
 * @param tower The tower to upgrade.
 */
void TowerGroup::upgradeTower(Tower *tower) {}

/**
 * @brief Converts the TowerStrategy of a Tower to match the index in the strategies array
 */
int TowerGroup::getStrategyIndex(Tower *tower)
{
	TowerStrategy *critterTargettingStrategy = tower->getCritterTargettingStrategy();
	if (dynamic_cast<TargetNearExit *>(critterTargettingStrategy))
	{
		return 0;
	}
	else if (dynamic_cast<TargetNearTower *>(critterTargettingStrategy))
	{
		return 1;
	}
	else if (dynamic_cast<TargetStrongest *>(critterTargettingStrategy))
	{
		return 2;
	}
	else if (dynamic_cast<TargetWeakest *>(critterTargettingStrategy))
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

/**
 * @brief Handles player interactions with towers and powerups.
 *
 * Processes clicks for tower placement, upgrades, selling, and powerup interactions.
 *
 * @param e The SDL event structure representing user input.
 */
void TowerGroup::handleEvent(SDL_Event &e)
{
	// resets tower buy selection
	bool buttonClick = false;
	bool correctCell = false;

	static Powerup *draggedPowerup = nullptr;
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

	if (dragging && draggedPowerup)
	{
		if (!SDL_HasRectIntersectionFloat(&draggedPowerup->position, &Global::mapViewRect))
		{
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
		for (Powerup *powerup : activePowerups)
		{
			if (powerup->isClicked(mouseX, mouseY))
			{
				draggedPowerup = powerup; // Start dragging powerup
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
		std::vector<DetailDisplayComponent *> components = detailDisplay->getComponents();

		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<DetailButton *>(components[i]) != nullptr)
			{
				if (dynamic_cast<DetailButton *>(components[i])->isClicked())
				{
					towerBuySelect = i - 1;
					buttonClick = true;

					// displays different values from dummy Towers
					switch (i)
					{
					case 1:
						detailDisplay->selectTower(dummyStandardTower);
						dummyStandardTower->notify();
						return;
					case 2:
						detailDisplay->selectTower(dummyRapidFireTower);
						dummyRapidFireTower->notify();
						return;
					case 3:
						detailDisplay->selectTower(dummyCannonTower);
						dummyCannonTower->notify();
						return;
					}
				}
			}
		}

		// checking if upgrading tower
		if (detailDisplay->getTowerObserver()->getCurrentTower() != nullptr && (dynamic_cast<DetailButton *>(detailDisplay->getTowerComponents()[7]))->isClicked())
		{
			buttonClick = true;
			int upgradeCost = detailDisplay->getTowerObserver()->getCurrentTower()->getUpgradeCost();

			// checks if enough coins for upgrade
			if (playerGold >= upgradeCost)
			{
				// checks if tower is already max level
				if (detailDisplay->getTowerObserver()->getCurrentTower()->upgrade())
				{
					detailDisplay->getTowerObserver()->getCurrentTower()->notify();
					playerGold -= upgradeCost;
				}

				return;
			}
		}

		// checking if changing TowerStrategy
		int startingIndex = TowerObserver::TOWER_COMPONENT_COUNT - TowerObserver::STRATEGY_COUNT + 1;
		Tower *currentTower = detailDisplay->getTowerObserver()->getCurrentTower();
		if (currentTower != nullptr && (dynamic_cast<DetailButton *>(detailDisplay->getTowerComponents()[10]))->isClicked())
		{
			buttonClick = true;

			int strategyIndex = getStrategyIndex(currentTower);
			++strategyIndex %= TowerObserver::STRATEGY_COUNT;
			currentTower->setCritterTargettingStrategy(strategies[strategyIndex]);
			currentTower->notify();

			return;
		}

		// checking if selling tower
		if (detailDisplay->getTowerObserver()->getCurrentTower() != nullptr && (dynamic_cast<DetailButton *>(detailDisplay->getTowerComponents()[8]))->isClicked())
		{
			buttonClick = true;
			playerGold += detailDisplay->getTowerObserver()->getCurrentTower()->getRefundValue();

			// find tower to erase
			for (int i = 0; i < towers.size(); i++)
			{
				if (towers[i] == detailDisplay->getTowerObserver()->getCurrentTower())
				{
					towers.erase(towers.begin() + i);

					map->wallCellDict[targetCell] = false;
				}
			}
			detailDisplay->selectTower(nullptr);

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
				detailDisplay->selectTower(towers[i]);
				towers[i]->notify();
				towerClick = true;
			}
		}

		// If not clicking on a tower or critter, proceed with placing a tower
		if (!towerClick)
		{
			if (correctCell && towerBuySelect >= 0 && map->wallCellDict.find(targetCell) != map->wallCellDict.end() && !map->wallCellDict[targetCell])
			{
				Tower *newTower = nullptr;

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
					detailDisplay->selectTower(newTower);

					float scaleFactor = 1.5f;
					float newSize = map->getPixelPerCell() * scaleFactor;
					newTower->setCurrentRenderRect(targetX, targetY, newSize, newSize);

					newTower->attach(detailDisplay->getTowerObserver());
					newTower->notify();

					map->wallCellDict[targetCell] = true;
				}
			}
			else
			{
				towerBuySelect = -1;
				detailDisplay->selectTower(nullptr);
			}

			return;
		}
	}

	if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
	{
		if (dragging && draggedPowerup)
		{

			bool towerClicked = false;
			for (size_t i = 0; i < towers.size(); i++)
			{
				if (towers[i]->isClicked(1))
				{
					// Unwrap the tower if it is already wrapped with a powerup
					Tower *baseTower = towers[i];

					// Unwrap all decorators (powerups) applied to the tower
					while (auto *decorated = dynamic_cast<TowerDecorator *>(baseTower))
					{
						baseTower = decorated->getWrappedTower(); // Get the base tower without the decorator
																  // Do not delete decorated here; let the new decorator handle it
					}

					// Apply the new powerup to the base tower (without previous decorators)
					Tower *upgradedTower = draggedPowerup->applyPowerupToTower(baseTower, towers[i]->getCurrentRenderRect());

					// Ensure the position of the tower is retained
					SDL_FRect towerRect = towers[i]->getCurrentRenderRect();
					upgradedTower->setCurrentRenderRect(towerRect.x, towerRect.y, towerRect.w, towerRect.h);

					// Replace the old tower with the new upgraded tower
					towers[i] = upgradedTower; // Replace the tower reference in the array

					// reattaches the tower observer
					baseTower->detach(detailDisplay->getTowerObserver());
					upgradedTower->attach(detailDisplay->getTowerObserver());
					detailDisplay->getTowerObserver()->setCurrentTower(upgradedTower);

					// Remove the powerup from active powerups and clean up
					activePowerups.erase(std::remove(activePowerups.begin(), activePowerups.end(), draggedPowerup), activePowerups.end());

					// Only delete draggedPowerup if it's no longer needed
					delete draggedPowerup;
					draggedPowerup = nullptr; // Set draggedPowerup to null after deletion

					// Reset dragging state
					dragging = false;
					towerClicked = true;
					break;
				}
			}
			if (!towerClicked)
			{
				draggedPowerup->isDragged = false;
				draggedPowerup = nullptr;
				dragging = false;
			}
		}
	}
}