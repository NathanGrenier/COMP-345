#pragma once
#include "Tower.h"

const int CANNON_RANGE = 125;
const int CANNON_POWER = 10;
const int CANNON_RATE_OF_FIRE = 1;
static const int CANNON_MAX_LEVEL = 3;

class CannonTower : public Tower
{
public:
    CannonTower();
    CannonTower(float x, float y, int buyingCost);
    CannonTower(float x, float y, int buyingCost, int refundValue);

    bool upgrade();
    void shootProjectile(DummyCritter* critter);

    void generateTower();
};
