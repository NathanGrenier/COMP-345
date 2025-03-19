/**
 * @class RapidFireTower
 * @brief Implementation of RapidFireTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/RapidFireTower.h>
#include <towers/Projectile.h>

/**
 * @brief Default Constructor
 */
RapidFireTower::RapidFireTower() : Tower(), fireBreak(0), fireBreakRate(0), burstSize(0), burstCount(0)
{
    towerTexture.loadFromFile("assets/tower/RapidFireTower.png");
    upgradeValues.rangeIncrease = 10;
    upgradeValues.rateOfFireIncrease = 3;
}

/**
 * @brief Constructor with position and buying cost 
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying RapidFireTower
 * @details Constructor for RapidFireTower with x, y position and buying cost
 * Uses default range, power, and rate of fire for RapidFireTower
 * Uses default refund value ratio in Tower class 
 * Sets the fireBreak to 0 to start shooting immediately and fireBreakRate to 5
 * Sets the burstSize to 50 for the interval for shooting, and the burstCount to 0
 */
RapidFireTower::RapidFireTower(float x, float y, int buyingCost)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, buyingCost, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/RapidFireTower.png");
    upgradeValues.rangeIncrease = 10;
    upgradeValues.rateOfFireIncrease = 3;
}

/**
 * @brief Constructor with position buying cost, and refund value
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying RapidFireTower
 * @param refundValue Amount of coins refunded when selling a RapidFireTower
 * @details Constructor for RapidFireTower with x, y position, buying cost, and refund value
 * Uses default range, power, and rate of fire for RapidFireTower
 * Uses default refund value ratio in Tower class 
 * Sets the fireBreak to 0 to start shooting immediately and fireBreakRate to 5
 * Sets the burstSize to 50 for the interval for shooting, and the burstCount to 0
 */
RapidFireTower::RapidFireTower(float x, float y, int buyingCost, int refundValue)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, buyingCost, refundValue, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/RapidFireTower.png");
    upgradeValues.rangeIncrease = 10;
    upgradeValues.rateOfFireIncrease = 3;
}

/**
 * @brief Upgrades a RapidFireTower
 * 
 * @details Slightly increases range
 * Moderately increaes the rate of fire
 * Moderately increases the burst size, allowing for more Projectiles to be fired in a firing interval
 * Performs check for level, if not already max level
 * @return true if RapidFireTower is upgraded successfully
 * @return false if RapidFireTower could not be upgraded
 * @return false if RapidFireTower could not be upgraded because it has already reached maximum level
 */
bool RapidFireTower::upgrade()
{
    // check if not yet max level
    if (Tower::upgrade())
    {
        burstSize += 20;
        return true;
    }

    return false;
}

/**
 * @brief Accessor for maximum level
 *
 * @return The maximum level for RapidFireTower upgrades
 */
int RapidFireTower::getMaxLevel()
{
    return RAPID_MAX_LEVEL;
}

/**
 * @brief Fires and moves projectiles for the RapidFireTower
 * 
 * @param critter Critter targetted by the RapidFireTower
 * @details Checks if Critter exists and can still be targeted by RapidFireTower
 * Calculates horizontal and vertical distance (giving direction) for projectile to move 
 * Times the projectiles to be shot out of the RapidFireTower, RapidFireTower has a fast rate of fire
 * Times the break interals between the rapid fire shooting intervals 
 * Applies damage to targetted Critter if the Projectile is in collision
 * Gets rid of Projectiles when needed, either if out of bounds, Critter is already dead, or when collision is made with Critter
 */
void RapidFireTower::shootProjectile(Critter* targettedCritter)
{
    // tower position with offset
    float posX = x + currentRenderedRect.w / 2;
    float posY = y + currentRenderedRect.w / 2;

    SDL_FRect currentCellSize = Global::currentMap->getPixelPerCell();


    // todo: add ratio multiplier













    // checks if it is a shooting interval
    if (fireBreak <= 0)
    {
        // checks if it is time to shoot within the interval
        if (shootingTimer <= 0)
        {
            if (targettedCritter != nullptr)
            {
                // fires a projectile, resets shooting timer
                projectiles.push_back(new Projectile(posX, posY, power, false, 5, 3, targettedCritter));
                shootingTimer = MAX_SHOOTING_TIMER;
            }
        }
        else
        {
            shootingTimer -= rateOfFire;
        }
        
        // if maximum interval time is reached
        if (burstCount == burstSize)
        {
            fireBreak = MAX_RAPID_FIRE_BREAK;
        }

        burstCount++;
    }
    else // break from firing interval, no Projectiles to be fired
    {
        burstCount = 0;
        fireBreak -= fireBreakRate;
    }
    
    // moves projectile at a moderate speed
    moveProjectiles();
}