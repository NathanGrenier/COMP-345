/**
 * @file CannonTower.cpp
 * @brief Implementation of CannonTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/CannonTower.h>
#include <towers/DummyCritter.h>
#include <towers/Projectile.h>

/**
 * @brief Default Constructor
 */
CannonTower::CannonTower() : Tower()
{

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

}

/**
 * @brief Upgrades a TowerCannon
 * 
 * @details Greatly increases the range
 * Slightly increases rate of fire
 * Moderately increaes the power of projectiles
 * Performs check for level, if not already max level
 * @return true if CannonTower is upgraded successfully
 * @return false if CannonTower could not be upgraded because it has already reached maximum level
 */
bool CannonTower::upgrade()
{
    // check if not yet max level
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
void CannonTower::shootProjectile(DummyCritter* critter)
{
    // check if critter still exists
    if (!critter)
    {
        // get rid of currently shot projectiles if no target
        projectiles.clear();
        return;
    }

    // tower position with offset
    float posX = x + static_cast<float>(TOWER_SIZE) / 2;
    float posY = y + static_cast<float>(TOWER_SIZE) / 2;

    // critter position with offset
    float critterPosX = critter->getX() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;
    float critterPosY = critter->getY() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;

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
        if (projectiles[i]->checkCollision(critterPosX, critterPosY))
        {
            critter->damageCritter(power);
            
            projectiles.erase(projectiles.begin() + i);

            // clear projectiles if critter has no hp, no target
            if (critter->getHealth() <= 0) {
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

/**
 * @brief Generates CannonTower
 * 
 * @details Represents a CannonTower with a yellow square
 * Draws the square using SDL 
 */
void CannonTower::generateTower()
{
    SDL_FRect fillRect = { x, y, Tower::TOWER_SIZE, Tower::TOWER_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}