#pragma once
#include "Tower.h"

const int STANDARD_RANGE = 100;
const int STANDARD_POWER = 3;
const int STANDARD_RATE_OF_FIRE = 4;
static const int STANDARD_MAX_LEVEL = 5;

class StandardTower : public Tower
{
public:
    StandardTower();
    StandardTower(float x, float y, int buyingCost);
    StandardTower(float x, float y, int buyingCost, int refundValue);

    bool upgrade();
    void shootProjectile(DummyCritter* critter);

    void generateTower();
};
