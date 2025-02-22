/**
 * @class Tower
 * @brief Implementation of Tower Parent class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/Tower.h>
#include <towers/DummyCritter.h>
#include <towers/Projectile.h>
#include <iostream>

/**
 * @brief Default Constructor, setting all values to 0
 */
Tower::Tower() 
    : x(0), y(0), buyingCost(0), refundValue(0), range(0), power(0), rateOfFire(0), level(0), shootingTimer(0)
{

}

/**
 * @brief Constructor with position, buying cost, range, power, and rate of fire 
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying Tower
 * @param range maximum distance for Tower to target a Critter
 * @param power damage passed on to Projectiles to remove from a Critter's health
 * @param rateOfFire value to indicate how fast a Tower shoots projectile: the greater the value, the faster it shoots
 * @details Constructor for Tower with x, y position, buying cost, range, power, and rate of fire 
 * Sets Tower level to 1 and shootingTimer to 0 to immediately start firing once placed
 * Uses default refund value ratio in Tower class 
 */
Tower::Tower(float x, float y, int buyingCost, int range, int power, int rateOfFire)
    : x(x), y(y), buyingCost(buyingCost), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0)
{
    refundValue = static_cast<int>(REFUND_RATIO * buyingCost);
}

/**
 * @brief Constructor with position, buying cost, refund value, range, power, and rate of fire 
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying Tower
 * @param refundValue Amount of coins refunded when selling a StandardTower
 * @param range maximum distance for Tower to target a Critter
 * @param power damage passed on to Projectiles to remove from a Critter's health
 * @param rateOfFire value to indicate how fast a Tower shoots projectile: the greater the value, the faster it shoots
 * @details Constructor for Tower with x, y position, buying cost, refund value, range, power, and rate of fire 
 * Sets Tower level to 1 and shootingTimer to 0 to immediately start firing once placed
 */
Tower::Tower(float x, float y, int buyingCost, int refundValue, int range, int power, int rateOfFire)
    : x(x), y(y), buyingCost(buyingCost), refundValue(refundValue), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0)
{
    
}

/**
 * @brief Finds a DummyCritter to target
 * 
 * @param critters Vector of currently existing DummyCritter within the map
 * @details Iterates through all DummyCritter in the map
 * Checks if each DummyCritter is in range of the Tower
 * Stops after the first DummyCritter that is in range of the Tower is found
 * @return DummyCritter pointer for first DummyCritter that is in range of the Tower
 * @return nullptr if no DummyCritter is in range
 */
DummyCritter * Tower::findCritter(std::vector<DummyCritter*> critters)
{
    // checks all critters in map
    for (int i = 0; i < critters.size(); i++)
    {
        if (isCritterInRange(*critters[i])) 
        {
            return critters[i];
        }
    }

    // no critter can be targeted by Tower
    return nullptr;
}

/**
 * @brief Clears projectiles previously fired from the Tower
 * 
 * @details Used when targeted DummyCritter is invalid, usually when DummyCritter dies and target changes
 */
void Tower::clearProjectiles() 
{
    projectiles.clear();
}

/**
 * @brief Shoots projectile from Tower, to be overridden by sub classes 
 * 
 * @param critter The DummyCritter targeted by the Tower
 */
void Tower::shootProjectile(DummyCritter* critter) 
{

}

/**
 * @brief Generates Tower
 * 
 * @details Represents a Tower with a black square
 * Draws the square using SDL 
 */
void Tower::generateTower()
{
    SDL_FRect fillRect = {x, y, TOWER_SIZE, TOWER_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

/**
 * @brief Generates Projectiles fired by a Tower
 * 
 * @details Iterates through the projectiles of a Tower to generate them, simply calling its respective function
 */
void Tower::generateAllProjectiles() 
{
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->generateProjectile();
    }
}

/**
 * @brief Upgrades Tower, to be overridden by sub classes. Cannot be used for Tower class 
 * 
 * @return false if Tower cannot be upgraded
 */
bool Tower::upgrade()
{
    return false;
}

/**
 * @brief Calculates the refund value of a Tower with levels in consideration
 * 
 * @details Sums the initial refund value along with a refund amount per each level upgraded
 * Default refund amount per level is 50 coins
 * @return a refund value of a Tower while considering levels
 */
int Tower::getRefundValue()
{
    return refundValue + (level - 1) * REFUND_PER_UPGRADE;
}

/**
 * @brief Calculates the upgrade cost of a Tower with current level in consideration
 * 
 * @details Adds 100 on top of an additional amount of coins as the cost of upgrading a Tower
 * Default additional amount per level is 50 coins
 * @return a refund value of a Tower while considering levels
 */
int Tower::getUpgradeCost()
{
    return 100 + level * 50;
}

/**
 * @brief Checks if a Tower has been clicked
 * 
 * @details If click event happens, checks if cursor is on the same position as the Tower's square
 * @return true if Tower has been clicked
 * @return false if Tower has not been clicked
 */
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

/**
 * @brief Checks if a DummyCritter is in range of the Tower
 * 
 * @param critter DummyCritter to check if Tower can damage
 * @details Compares the range of the Tower with the distance between the Tower and the DummyCritter
 * @return true if the DummyCritter is in range of the Tower and can be fired a Projectile at 
 * @return false if the DummyCritter is out of range of the Tower and cannot be damaged 
 */
bool Tower::isCritterInRange(DummyCritter critter) 
{
    return range >= calcDistance(critter);
}

/**
 * @brief Calculates the distance between the DummyCritter and Tower
 * 
 * @param critter DummyCritter to check distance from Tower
 * @details Calculates the horizontal and vertical distance between the Tower and the DummyCritter 
 * Then uses both distances to calculate the direct distance 
 * Uses hypotenuse of a triangle/distance formula
 * Takes Tower and DummyCritter size in account for distance
 * @return the absolute distance between the Tower and the DummyCritter
 */
float Tower::calcDistance(DummyCritter critter) 
{
    // considers Tower size
    float posX = x + static_cast<float>(TOWER_SIZE) / 2;
    float posY = y + static_cast<float>(TOWER_SIZE) / 2;

    // considers Critter size
    float critterPosX = critter.getX() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;
    float critterPosY = critter.getY() + static_cast<float>(DummyCritter::CRITTER_SIZE) / 2;

    // distance in each direction
    float differenceX = posX - critterPosX;
    float differenceY = posY - critterPosY;

    // distance formula
    return sqrt(pow(differenceX, 2) + pow(differenceY, 2));
}