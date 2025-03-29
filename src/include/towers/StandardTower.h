/**
 * @file StandardTower
 * @brief Tower class for a standard tower
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class of StandardTower
 * Tower is used as a cheap way to defeat critters at lower levels
 * It is the standard for other towers to compare to
 * Implements shooting projectiles at critters at a regular pace, as well as upgrading range, power, rate of fire
 */

#pragma once
#include "Tower.h"

class StandardTower : public Tower {
public:
	// Initial DPS: 10
	// Max DPS: 54
	static const int TOWER_COST = 50;
	static const int RANGE = 100; /**< @brief default range for StandardTower */
	static const int POWER = 5; /**< @brief default projectile damage for StandardTower */
	static const int RATE_OF_FIRE = 2; /**< @brief default rate of fire for StandardTower */
	static const int MAX_LEVEL = 5; /**< @brief default max level for StandardTower */

	static constexpr float TURN_SPEED_FACTOR = 1.0f;

	// Range: 100 - 110 - 120 - 130 - 140
	static const int rangeIncreasePerLevel = 10;
	static const int powerIncreasePerLevel = 1;
	static const int rateOfFireIncreasePerLevel = 1;
	static const int upgradeCosts[MAX_LEVEL - 1];

	StandardTower();
	StandardTower(float x, float y, float width, int buyingCost);

	int getMaxLevel();
	void shootProjectile(Critter* targettedCritter) override;
};
