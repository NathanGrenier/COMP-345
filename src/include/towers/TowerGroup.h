#pragma once

#include <vector>
#include <towers/Tower.h>
#include <ui/DetailAttributeDisplay.h>
#include <map/Map.h>
#include <towers/powerups/Powerup.h>
#include <SDL3_mixer/SDL_mixer.h>

/**
 * @file TowerGroup.h
 * @brief The header file for the TowerGroup class, managing all towers on the map.
 * @author
 * @date 2025-03-15
 *
 * @details Handles creation, rendering, and upgrading of towers.
 */

class TowerGroup {
public:
	// Constructor with parameters
	TowerGroup(int& playerGold, Map* map, DetailAttributeDisplay* detailDisplay);
	~TowerGroup();

	void update(float deltaTime, std::vector<Critter*> critters); // Update towers to attack critters
	void render();												   // Render all towers

	std::vector<Tower*>& getTowers();               // Access all towers (modifiable)
	int getTotalTowersPlaced();                     // Access number of towers
	Tower* getTowerAtPosition(float x, float y, float scaleFactor); // Get a tower at a specific position (for selecting/upgrading)

	void upgradeTower(Tower* tower); // Upgrade a selected tower, if possible
	void handleEvent(SDL_Event& e);

	static int getStrategyIndex(Tower* tower); /** @brief index for the strategy of a Tower */
	static Mix_Chunk* towerPurchase;
	static Mix_Chunk* towerUpgrade;
	static Mix_Chunk* towerSell;
private:
	int& playerGold;                               // Reference to player's gold for buying/upgrading towers
	Map* map;                                      // Pointer to the map for tower placement validation
	DetailAttributeDisplay* detailDisplay;         // UI detail display for selected tower information
	std::vector<Tower*> towers;                    // Vector containing all tower instances
	std::vector<Powerup*> activePowerups;
	int towerBuySelect = -1;
	int totalTowersPlaced = 0;

	Tower* dummyStandardTower;	/**< @brief dummy StandardTower for details before buying Tower */
	Tower* dummyRapidFireTower; /**< @brief dummy RapidFireTower for details before buying Tower */
	Tower* dummyCannonTower;	/**< @brief dummy CannonTower for details before buying Tower */
	TowerStrategy** strategies; /**< @brief Tower Strategies for targetting Critters */

	const float POWERUP_SPAWN_CHANCE = 0.01;

	/** @brief targetted cell when clicking */
	Cell targetCell;

	static Powerup* draggedPowerup;
	static bool dragging;
};
