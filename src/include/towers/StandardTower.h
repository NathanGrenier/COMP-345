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
	static const int STANDARD_RANGE = 125; /**< @brief default range for StandardTower */
	static const int STANDARD_POWER = 3; /**< @brief default projectile damage for StandardTower */
	static const int STANDARD_RATE_OF_FIRE = 4; /**< @brief default rate of fire for StandardTower */
	static const int STANDARD_MAX_LEVEL = 5; /**< @brief default max level for StandardTower */

	static const int rangeIncreasePerLevel = 20;
	static const int powerIncreasePerLevel = 1;
	static const int rateOfFireIncreasePerLevel = 1;

	StandardTower();
	StandardTower(float x, float y, float width, int buyingCost);
	StandardTower(float x, float y, float width, int buyingCost, int refundValue);

	int getMaxLevel();
	void shootProjectile(Critter* targettedCritter) override;
};
