/**
 * @file RapidFireTower.h
 * @brief Tower class for a tower that shoots faster relative to a standard tower
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class of RapidFireTower
 * Shoots projectiles at a quicker pace than StandardTower
 * However, has a pause in shooting, fires projectiles in small intervals
 * Slightly more expensive variation of StandardTower
 * Implements shooting projectiles at critters at a fast pace but broken into intervals
 * Upgrades are for range, rate of fire, and in the number of projectiles in an interval
 */

#pragma once
#include "Tower.h"

class RapidFireTower : public Tower {
public:
	// Initial DPS: 24
	// Max DPS: 42
	static const int TOWER_COST = 75;
	static const int RANGE = 50; /**< @brief default range for RapidFireTower */
	static const int POWER = 3; /**< @brief default projectile damage for RapidFireTower */
	static const int RATE_OF_FIRE = 8; /**< @brief default rate of fire for RapidFireTower */
	static const int MAX_LEVEL = 3; /**< @brief default max level for RapidFireTower */
	static const int MAX_BREAK = 200; /**< @brief break maximum for no shooting intervals */

	static constexpr float TURN_SPEED_FACTOR = 1.2f;

	// Range: 50 - 60 - 70
	static const int rangeIncreasePerLevel = 10;
	static const int rateOfFireIncreasePerLevel = 3;
	static const int upgradeCosts[MAX_LEVEL - 1];

	RapidFireTower();
	RapidFireTower(float x, float y, float width, int buyingCost);

	bool upgrade();
	int getMaxLevel();
	void shootProjectile(Critter* targettedCritter) override;
	void loadTextureForLevel() override;
private:
	int fireBreak; /**< @brief decremented for no shooting interval */
	int fireBreakRate; /**< @brief amount to decrement fireBreak by */
	int burstSize; /**< @brief time for projectiles to be fired in an interval */
	int burstCount; /**< @brief counts time for firing projectiles in interval */
	bool isAnimating;
};
