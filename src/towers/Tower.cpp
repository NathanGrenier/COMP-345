#include <Global.h>
#include <towers/Tower.h>
#include <towers/DummyCritter.h>
#include <towers/Projectile.h>
#include <iostream>

Tower::Tower() 
    : x(0), y(0), buyingCost(0), refundValue(0), range(0), power(0), rateOfFire(0), level(0), shootingTimer(0)
{

}

Tower::Tower(float x, float y, int buyingCost, int range, int power, int rateOfFire)
    : x(x), y(y), buyingCost(buyingCost), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0)
{
    refundValue = static_cast<int>(REFUND_RATIO * buyingCost);
}

Tower::Tower(float x, float y, int buyingCost, int refundValue, int range, int power, int rateOfFire)
    : x(x), y(y), buyingCost(buyingCost), refundValue(refundValue), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0)
{
    
}

DummyCritter * Tower::findCritter(std::vector<DummyCritter*> critters)
{
    for (int i = 0; i < critters.size(); i++)
    {
        if (isCritterInRange(*critters[i])) 
        {
            return critters[i];
        }
    }
    return nullptr;
}

void Tower::clearProjectiles() 
{
    projectiles.clear();
}

void Tower::shootProjectile(DummyCritter* critter) 
{

}

void Tower::generateTower()
{
    SDL_FRect fillRect = {x, y, TOWER_SIZE, TOWER_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

void Tower::generateAllProjectiles() 
{
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->generateProjectile();
    }
}

bool Tower::upgrade()
{
    return false;
}

int Tower::getRefundValue()
{
    return refundValue + (level - 1) * REFUND_PER_UPGRADE;
}

int Tower::getUpgradeCost()
{
    return 100 + level * 50;
}

bool Tower::isClicked() const 
{
    // Get the current mouse position
    float mouseXPos, mouseYPos;
    SDL_GetMouseState(&mouseXPos, &mouseYPos);

    // Check if the mouse position is inside the button's area
    if (mouseXPos >= x && mouseXPos <= x + Tower::TOWER_SIZE &&
        mouseYPos >= y && mouseYPos <= y + Tower::TOWER_SIZE)
    {
        return true;
    }
    return false;
}

bool Tower::isCritterInRange(DummyCritter critter) 
{
    return range >= calcDistance(critter);
}

float Tower::calcDistance(DummyCritter critter) 
{
    float posX = x + static_cast<float>(TOWER_SIZE) / 2;
    float posY = y + static_cast<float>(TOWER_SIZE) / 2;

    float critterPosX = critter.getX() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;
    float critterPosY = critter.getY() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;

    float differenceX = posX - critterPosX;
    float differenceY = posY - critterPosY;

    return sqrt(pow(differenceX, 2) + pow(differenceY, 2));
}