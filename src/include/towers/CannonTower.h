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
 *
 */

#pragma once
#include <critter/Critter.h>
#include "Tower.h"

class CannonTower : public Tower
{
public:
    static const int CANNON_RANGE = 125; /** @brief default range for CannonTower */
    static const int CANNON_POWER = 10; /** @brief default projectile damage for CannonTower */
    static const int CANNON_RATE_OF_FIRE = 1; /** @brief default rate of fire for CannonTower */
    static const int CANNON_MAX_LEVEL = 3; /** @brief default maxmimum level for CannonTower */

    CannonTower();
    CannonTower(float x, float y, float width, int buyingCost);
    CannonTower(float x, float y, float width, int buyingCost, int refundValue);

    int getMaxLevel();
    void shootProjectile(Critter* targettedCritter);
};
