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
#include <towers/Projectile.h>
#include <util/Observable.h>
#include <critter/Critter.h>
#include <ui/LTexture.h>

 /** @brief ratio between original tower cost and refund value */
const double REFUND_RATIO = 0.5;

class Tower : public Observable
{
public:
    static const int MAX_SHOOTING_TIMER = 100; /** @brief shooting timer to be decremented */
    static const int REFUND_PER_UPGRADE = 50; /** @brief additional gold refunded per level */

    Tower();
    Tower(float x, float y, int buyingCost, int range, int power, int rateOfFire);
    Tower(float x, float y, int buyingCost, int refundValue, int range, int power, int rateOfFire);
    
    virtual void generateTower();
    virtual void generateAllProjectiles();
    
    int getRange();
    int getPower();
    int getRateOfFire();
    int getLevel();

    virtual int getRefundValue();
    virtual int getUpgradeCost();
    virtual int getMaxLevel() = 0;

    virtual bool upgrade();
    virtual void shootProjectile(Critter* critter) = 0;

    Critter* findCritter(std::vector<Critter>& critters); /** finds critter in range of tower */
    void clearProjectiles();
    bool isClicked(float scaleFactor) const;
    bool isCritterInRange(Critter critter);

    struct UpgradeValues {
        int rangeIncrease;
        int powerIncrease;
        int rateOfFireIncrease;
    };

    UpgradeValues getUpgradeValues();

    void render();
    void setRotation(float angle);
    float getRotation() const { return rotationAngle; }
    void setCurrentRenderedRect(SDL_FRect targetRect);
    SDL_FRect getCurrentRenderedRect();

    float x; /** @brief x position for projectile */
    float y; /** @brief y position for projectile */

protected:
    SDL_FRect currentRenderedRect;
    int buyingCost;
    int refundValue;
    float rotationAngle = 0.0f;
    int range;
    int power;
    int rateOfFire; /** @brief higher rate of fire indicates faster shooting */
    int level;
    int shootingTimer; /** @brief decremented with rate of fire for shooting */
    float calcDistance(Critter critter);
    std::vector<Projectile *> projectiles; /** @brief vector of all projectile */

    UpgradeValues upgradeValues;

    LTexture towerTexture;
};
