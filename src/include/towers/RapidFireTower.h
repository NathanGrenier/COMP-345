#pragma once
#include "Tower.h"

const int RAPID_RANGE = 75;
const int RAPID_POWER = 1;
const int RAPID_RATE_OF_FIRE = 10;
static const int RAPID_MAX_LEVEL = 3;

const int MAX_RAPID_FIRE_BREAK = 200;

class RapidFireTower : public Tower
{
public:
    RapidFireTower();
    RapidFireTower(float x, float y, int buyingCost);
    RapidFireTower(float x, float y, int buyingCost, int refundValue);
    
    bool upgrade();
    void shootProjectile(DummyCritter* critter);
    void generateTower();

private:
    int fireBreak;
    int fireBreakRate;
    int burstSize;
    int burstCount;
};
