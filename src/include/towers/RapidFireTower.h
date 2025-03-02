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

class RapidFireTower : public Tower
{
public:
    static const int RAPID_RANGE = 75; /** @brief default range for RapidFireTower */
    static const int RAPID_POWER = 1; /** @brief default projectile damage for RapidFireTower */
    static const int RAPID_RATE_OF_FIRE = 10; /** @brief default rate of fire for RapidFireTower */
    static const int RAPID_MAX_LEVEL = 3; /** @brief default max level for RapidFireTower */
    static const int MAX_RAPID_FIRE_BREAK = 200; /** @brief break maximum for no shooting intervals */

    RapidFireTower();
    RapidFireTower(float x, float y, int buyingCost);
    RapidFireTower(float x, float y, int buyingCost, int refundValue);
    
    bool upgrade();
    int getMaxLevel();
    void shootProjectile(DummyCritter* critter);
    void generateTower();

private:
    int fireBreak; /** @brief decremented for no shooting interval */
    int fireBreakRate; /** @brief amount to decrement fireBreak by */
    int burstSize; /** @brief time for projectiles to be fired in an interval */
    int burstCount; /** @brief counts time for firing projectiles in interval */
};
