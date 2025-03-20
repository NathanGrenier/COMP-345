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
    static constexpr float PI_CONSTANT = 3.14159265358979323846f;

    Tower();
    Tower(float x, float y, float width, int buyingCost, int range, int power, int rateOfFire);
    Tower(float x, float y, float width, int buyingCost, int refundValue, int range, int power, int rateOfFire);
    
    virtual std::vector<Projectile*>& getProjectiles() { return projectiles; }

    virtual Critter* findCritter(std::vector<Critter*> critters);

    struct UpgradeValues {
        int rangeIncrease;
        int powerIncrease;
        int rateOfFireIncrease;
    };

    UpgradeValues upgradeValues;

    virtual void generateAllProjectiles();
    
    virtual int getRange() const;

    virtual int getPower() const;

    virtual int getRateOfFire() const;

    virtual int getLevel() const;

    virtual int getRefundValue();

    virtual int getUpgradeCost();

    virtual int getMaxLevel() = 0;

    virtual bool upgrade();

    virtual void shootProjectile(Critter* critter) = 0;

    virtual void clearProjectiles();

    virtual bool isClicked(float scaleFactor) const;

    virtual bool isCritterInRange(Critter* critter);

    virtual UpgradeValues getUpgradeValues() const;

    virtual void render();

    virtual void setRotation(float angle);

    virtual float getRotation() const { return rotationAngle; }

    virtual void setCurrentRenderRect(float originalX, float originalY, float w, float h);

    virtual SDL_FRect getCurrentRenderRect() const;

    virtual LTexture& getTowerTexture() { return towerTexture; };

    virtual int& getShootingTimer() { return shootingTimer; };

    virtual void setShootingTimer(int newShootingTimer) { shootingTimer = newShootingTimer; };

private:
    SDL_FRect currentRenderRect;
    int buyingCost;
    int upgradeCost;
    int refundValue;
    float rotationAngle = 0.0f;
    int range;
    int power;
    int rateOfFire; /** @brief higher rate of fire indicates faster shooting */
    int level;
    int shootingTimer; /** @brief decremented with rate of fire for shooting */
    float calcDistance(Critter* critter) const;
    std::vector<Projectile *> projectiles; /** @brief vector of all projectile */
    LTexture towerTexture;
};
