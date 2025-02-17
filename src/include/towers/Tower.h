#pragma once
#include "DummyCritter.h"
#include "Projectile.h"

const double REFUND_RATIO = 0.5;

class Tower
{
public:
    static const int TOWER_SIZE = 30;
    static const int MAX_SHOOTING_TIMER = 100;
    static const int REFUND_PER_UPGRADE = 50;

    Tower();
    Tower(float x, float y, int buyingCost, int range, int power, int rateOfFire);
    Tower(float x, float y, int buyingCost, int refundValue, int range, int power, int rateOfFire);
    
    virtual void generateTower();
    virtual void generateAllProjectiles();
    virtual bool upgrade();
    virtual int getRefundValue();
    virtual int getUpgradeCost();
    virtual void shootProjectile(DummyCritter* critter);

    DummyCritter* findCritter(std::vector<DummyCritter *> critters);
    void clearProjectiles();
    bool isClicked() const;
    bool isCritterInRange(DummyCritter critter);

protected: 
    float x;
    float y;
    int buyingCost;
    int refundValue;
    int range;
    int power;
    int rateOfFire;
    int level;
    int shootingTimer;
    float calcDistance(DummyCritter critter);
    std::vector<Projectile *> projectiles;
};
