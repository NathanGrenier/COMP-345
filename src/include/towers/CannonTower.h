/**
 * @file CannonTower.h
 * @brief Tower class for a tower that shoots sparingly but with longer range and more damage
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class of CannonTower
 * Shoots projectiles at a slower pace than StandardTower
 * However, shoots much more powerful projectiles
 * Much more expensive variation of StandardTower
 * Implements shooting projectiles at critters at a slow pace
 * Upgrades are for range, rate of fire, and power
 */

#pragma once
#include <critter/Critter.h>
#include "Tower.h"

class CannonTower : public Tower {
public:
	// Initial DPS: 10
	// Max DPS: 150
	static const int TOWER_COST = 100;
	static const int RANGE = 75;      /** @brief default range for CannonTower */
	static const int POWER = 10;       /** @brief default projectile damage for CannonTower */
	static const int RATE_OF_FIRE = 1; /** @brief default rate of fire for CannonTower */
	static const int MAX_LEVEL = 3;    /** @brief default maximum level for CannonTower */

	static constexpr float TURN_SPEED_FACTOR = 0.8f;

	// Range: 75 - 95 - 115
	static const int rangeIncreasePerLevel = 20;
	static const int powerIncreasePerLevel = 20;
	static const int rateOfFireIncreasePerLevel = 1;
	static const int upgradeCosts[MAX_LEVEL - 1];

	CannonTower();
	CannonTower(float x, float y, float width, int buyingCost);

	int getMaxLevel();
	void shootProjectile(Critter* targettedCritter);
	void loadTextureForLevel() override;
	std::string towerShotFileName = "tower/CannonTower/towerShot.wav";
	static Mix_Chunk* towerShot;
};
