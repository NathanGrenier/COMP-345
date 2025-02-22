/**
 * @file Towers.h
 * @brief Parent class for towers to fire at critters
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Parent class of all Towers to shoot at critters
 * Implements calculating distance between tower and critter to find critters in range
 * Also implements deleting projectiles, for when critters are killed but projectile is fired
 */

#pragma once
#include "DummyCritter.h"
#include "Projectile.h"

 /** @brief ratio between original tower cost and refund value */
const double REFUND_RATIO = 0.5;

class Tower
{
public:
    static const int TOWER_SIZE = 30; /** @brief size of tower in pixels; to change to match with level gen */
    static const int MAX_SHOOTING_TIMER = 100; /** @brief shooting timer to be decremented */
    static const int REFUND_PER_UPGRADE = 50; /** @brief additional gold refunded per level */

    Tower();
    Tower(float x, float y, int buyingCost, int range, int power, int rateOfFire);
    Tower(float x, float y, int buyingCost, int refundValue, int range, int power, int rateOfFire);
    
    virtual void generateTower();
    virtual void generateAllProjectiles();
    virtual bool upgrade();
    virtual int getRefundValue();
    virtual int getUpgradeCost();
    virtual void shootProjectile(DummyCritter* critter); /** should be pure virtual */

    DummyCritter* findCritter(std::vector<DummyCritter *> critters); /** finds critter in range of tower */
    void clearProjectiles();
    bool isClicked() const;
    bool isCritterInRange(DummyCritter critter);

protected: 
    float x; /** @brief x position for projectile */
    float y; /** @brief y position for projectile */
    int buyingCost;
    int refundValue;
    int range;
    int power;
    int rateOfFire; /** @brief higher rate of fire indicates faster shooting */
    int level;
    int shootingTimer; /** @brief decremented with rate of fire for shooting */
    float calcDistance(DummyCritter critter); 
    std::vector<Projectile *> projectiles; /** @brief vector of all projectile */
};
