#include <Global.h>
#include <towers/RapidFireTower.h>
#include <towers/DummyCritter.h>
#include <towers/Projectile.h>

RapidFireTower::RapidFireTower() : Tower()
{

}

RapidFireTower::RapidFireTower(float x, float y, int buyingCost)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, buyingCost, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
{
    
}

RapidFireTower::RapidFireTower(float x, float y, int buyingCost, int refundValue)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, buyingCost, refundValue, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
{

}

bool RapidFireTower::upgrade()
{
    if (level < RAPID_MAX_LEVEL)
    {
        range += 10;
        rateOfFire += 3;
        burstSize += 20;

        level++;

        return true;
    }

    return false;
}

void RapidFireTower::shootProjectile(DummyCritter* critter)
{
    if (!critter)
    {
        projectiles.clear();
        return;
    }

    float posX = x + static_cast<float>(TOWER_SIZE) / 2;
    float posY = y + static_cast<float>(TOWER_SIZE) / 2;

    float critterPosX = critter->getX() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;
    float critterPosY = critter->getY() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;

    float differenceX = posX - critterPosX;
    float differenceY = posY - critterPosY;

    float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

    float speedX = (critterPosX - posX) / distance;
    float speedY = (critterPosY - posY) / distance;

    if (fireBreak <= 0)
    {
        if (shootingTimer <= 0)
        {
            projectiles.push_back(new Projectile(posX, posY, power, false, 3));
            shootingTimer = MAX_SHOOTING_TIMER;
        }
        else
        {
            shootingTimer -= rateOfFire;
        }
        
        if (burstCount == burstSize)
        {
            fireBreak = MAX_RAPID_FIRE_BREAK;
        }

        burstCount++;
    }
    else 
    {
        burstCount = 0;
        fireBreak -= fireBreakRate;
    }
    

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(5 * speedX, 5 * speedY);

        if (projectiles[i]->checkCollision(critterPosX, critterPosY))
        {
            critter->damageCritter(power);

            projectiles.erase(projectiles.begin() + i);
            
            if (critter->getHealth() <= 0) {
                projectiles.clear();
            }

        }
        else if (projectiles[i]->isOutside())
        {
            projectiles.erase(projectiles.begin() + i);
        }
    }
}

void RapidFireTower::generateTower()
{
    SDL_FRect fillRect = { x, y, Tower::TOWER_SIZE, Tower::TOWER_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}