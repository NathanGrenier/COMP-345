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

const int STANDARD_RANGE = 100; /** @brief default range for standard towers */
const int STANDARD_POWER = 3; /** @brief default projectile damage for standard towers */
const int STANDARD_RATE_OF_FIRE = 4; /** @brief default rate of fire for standard towers */
static const int STANDARD_MAX_LEVEL = 5; /** @brief default max level for standard towers */

class StandardTower : public Tower
{
public:
    StandardTower();
    StandardTower(float x, float y, int buyingCost);
    StandardTower(float x, float y, int buyingCost, int refundValue);

    bool upgrade(); /** upgrades range, power, and rate of fire */
    void shootProjectile(DummyCritter* critter);

    void generateTower();
};
