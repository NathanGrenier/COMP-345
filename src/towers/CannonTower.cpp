/**
 * @class CannonTower
 * @brief Implementation of CannonTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/CannonTower.h>
#include <towers/Projectile.h>

/**
 * @brief Default Constructor
 */
CannonTower::CannonTower() : Tower()
{
    towerTexture.loadFromFile("assets/tower/CannonTower.png");
    upgradeValues.rangeIncrease = 50;
    upgradeValues.powerIncrease = 5;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Constructor with position and buying cost 
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying CannonTower
 * @details Constructor for CannonTower with x, y position and buying cost
 * Uses default range, power, and rate of fire for CannonTower
 * Uses default refund value ratio in Tower class 
 */
CannonTower::CannonTower(float x, float y, int buyingCost)
    : Tower(x, y, buyingCost, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/CannonTower.png");
    upgradeValues.rangeIncrease = 50;
    upgradeValues.powerIncrease = 5;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Constructor with position, buying cost, and refund value
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying CannonTower
 * @param refundValue Amount of coins refunded when selling a CannonTower
 * @details Constructor for CannonTower with x, y position, buying cost, and refund value
 * Uses default range, power, and rate of fire for CannonTower
 */
CannonTower::CannonTower(float x, float y, int buyingCost, int refundValue)
    : Tower(x, y, buyingCost, refundValue, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/CannonTower.png");
    upgradeValues.rangeIncrease = 50;
    upgradeValues.powerIncrease = 5;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Accessor for maximum level
 *
 * @return The maximum level for CannonTower upgrades
 */
int CannonTower::getMaxLevel()
{
    return CANNON_MAX_LEVEL;
}

/**
 * @brief Fires and moves projectiles for the CannonTower
 * 
 * @param critter Critter targetted by the CannonTower
 * @details Checks if Critter exists and can still be targeted by CannonTower
 * Calculates horizontal and vertical distance (giving direction) for projectile to move 
 * Times the projectiles to be shot out of the CannonTower, CannonTower has a slow rate of fire
 * Applies damage to targetted Critter if the Projectile is in collision
 * Gets rid of Projectiles when needed, either if out of bounds, Critter is already dead, or when collision is made with Critter
 */
void CannonTower::shootProjectile(Critter* critter)
{
    // check if critter still exists
    if (!critter)
    {
        // get rid of currently shot projectiles if no target
        projectiles.clear();
        return;
    }

    // tower position with offset
    float posX = x + currentRenderedRect.w / 2;
    float posY = y + currentRenderedRect.w / 2;

    // critter position with offset
    float critterPosX = critter->getPosition().x;
    float critterPosY = critter->getPosition().y;

    // differences in position from tower to cannon
    float differenceX = posX - critterPosX;
    float differenceY = posY - critterPosY;

    float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

    // distance for projectile as a unit vector
    float speedX = (critterPosX - posX) / distance;
    float speedY = (critterPosY - posY) / distance;

    // checks if it is time to shoot
    if (shootingTimer <= 0)
    {
        // fires a big sized projectile, resets shooting timer
        projectiles.push_back(new Projectile(posX, posY, power, false, 6));
        shootingTimer = MAX_SHOOTING_TIMER;
    }
    else // decreases shooting timer
    {
        shootingTimer -= rateOfFire;
    }

    // moves projectile at a slow speed
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(3 * speedX, 3 * speedY);

        // check if projectile hits critter
        if (projectiles[i]->checkCollision(critter))
        {
            critter->takeDamage(power);
            critter->notify();
            projectiles.erase(projectiles.begin() + i);

            // clear projectiles if critter has no hp, no target
            if (critter->getHitPoints() <= 0) {
                projectiles.clear();
            }
        }
        // check if projectile is outside of map
        else if (projectiles[i]->isOutside())
        {
            projectiles.erase(projectiles.begin() + i);
        }
    }
}