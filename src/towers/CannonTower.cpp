#include <Global.h>
#include <towers/CannonTower.h>
#include <towers/DummyCritter.h>
#include <towers/Projectile.h>

CannonTower::CannonTower() : Tower()
{

}

CannonTower::CannonTower(float x, float y, int buyingCost)
    : Tower(x, y, buyingCost, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{
    
}

CannonTower::CannonTower(float x, float y, int buyingCost, int refundValue)
    : Tower(x, y, buyingCost, refundValue, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{

}

bool CannonTower::upgrade()
{
    if (level < CANNON_MAX_LEVEL)
    {
        range += 50;
        rateOfFire += 1;
        power += 5;

        level++;

        return true;
    }

    return false;
}

void CannonTower::shootProjectile(DummyCritter* critter)
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

    if (shootingTimer <= 0)
    {
        projectiles.push_back(new Projectile(posX, posY, power, false, 6));
        shootingTimer = MAX_SHOOTING_TIMER;
    }
    else
    {
        shootingTimer -= rateOfFire;
    }

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(3 * speedX, 3 * speedY);

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

void CannonTower::generateTower()
{
    SDL_FRect fillRect = { x, y, Tower::TOWER_SIZE, Tower::TOWER_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}