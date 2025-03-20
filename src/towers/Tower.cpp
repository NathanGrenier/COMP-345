/**
 * @class Tower
 * @brief Implementation of Tower Parent class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/Tower.h>
#include <towers/Projectile.h>
#include <iostream>

/**
 * @brief Default Constructor, setting all values to 0
 */
Tower::Tower() 
    : upgradeCost(0), buyingCost(0), refundValue(0), range(0), power(0), rateOfFire(0), level(0), shootingTimer(0), upgradeValues{ 0, 0, 0}
{
    currentRenderRect = { 0, 0, 0, 0 };
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
Tower::Tower(float x, float y, float width, int buyingCost, int range, int power, int rateOfFire)
    : upgradeCost(0), buyingCost(buyingCost), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0), upgradeValues{ 0, 0, 0 }
{
    refundValue = static_cast<int>(REFUND_RATIO * buyingCost);
    currentRenderRect = { x, y, width, width };
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
Tower::Tower(float x, float y, float width, int buyingCost, int refundValue, int range, int power, int rateOfFire)
    : upgradeCost(0), buyingCost(buyingCost), refundValue(refundValue), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0), upgradeValues{ 0, 0, 0 }
{
    currentRenderRect = { x, y, width, width };
}

/**
 * @brief Finds a DummyCritter to target
 * 
 * @param critters Vector of currently existing DummyCritter within the map
 * @details Iterates through all DummyCritter in the map
 * Checks if each Critter is in range of the Tower
 * Stops after the first DummyCritter that is in range of the Tower is found
 * @return DummyCritter pointer for first DummyCritter that is in range of the Tower
 * @return nullptr if no DummyCritter is in range
 */
Critter* Tower::findCritter(std::vector<Critter*> critters) {
    // checks all critters in map
    for (int i = 0; i < critters.size(); i++) {
        if (isCritterInRange(critters[i])) { // Directly use critter[i] as it is an object
            return critters[i]; // Return a pointer to the critter
        }
    }

    // No critter can be targeted by the Tower
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
 * @brief Generates Projectiles fired by a Tower
 * 
 * @details Iterates through the projectiles of a Tower to generate them, simply calling its respective function
 */
void Tower::generateAllProjectiles() 
{
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->updateAnimation(0.016f);
    }
}

/**
 * @brief Accessor for range
 * @return the range for the Tower
 */
int Tower::getRange() const
{
    return range;
}

/**
 * @brief Accessor for power
 * @return the power for the Tower
 */
int Tower::getPower() const
{
    return power;
}

/**
 * @brief Accessor for rate of fire
 * @return the rate of fire for the Tower
 */
int Tower::getRateOfFire() const
{
    return rateOfFire;
}

/**
 * @brief Accessor for level
 * @return the level for the Tower
 */
int Tower::getLevel() const
{
    return level;
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
 * @brief Accessor for the Tower upgrade values
 * @return a struct containing the Tower upgrade values
 */
Tower::UpgradeValues Tower::getUpgradeValues() const
{
    return upgradeValues;
}

/**
 * @brief Upgrades a Tower
 * @details Increases the Tower by 1 level
 * Increases range, power, and rate of fire
 * Performs checks for if the Tower is already maximum level
 * @return true if the Tower has been upgraded
 * @return false if the Tower could not have been upgraded
 */
bool Tower::upgrade()
{
    if (level < getMaxLevel())
    {
        range += upgradeValues.rangeIncrease;
        power += upgradeValues.powerIncrease;
        rateOfFire += upgradeValues.rateOfFireIncrease;

        level++;

        notify();
        return true;
    }

    return false;
}

/**
 * @brief Checks if a Tower has been clicked
 * 
 * @details If click event happens, checks if cursor is on the same position as the Tower's square
 * @return true if Tower has been clicked
 * @return false if Tower has not been clicked
 */
bool Tower::isClicked(float scaleFactor) const
{
    // Get the current mouse position
    float mouseXPos, mouseYPos;
    SDL_GetMouseState(&mouseXPos, &mouseYPos);

    // Adjust width and height based on the scale factor
    float scaledWidth = currentRenderRect.w / scaleFactor;
    float scaledHeight = currentRenderRect.h / scaleFactor;

    // Check if the mouse position is inside the scaled area of the tower
    if (mouseXPos >= currentRenderRect.x && mouseXPos <= currentRenderRect.x + scaledWidth &&
        mouseYPos >= currentRenderRect.y && mouseYPos <= currentRenderRect.y + scaledHeight)
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
bool Tower::isCritterInRange(Critter* critter) 
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
float Tower::calcDistance(Critter* critter) const
{
    // considers Tower size
    float posX = currentRenderRect.x + currentRenderRect.w / 2;
    float posY = currentRenderRect.y + currentRenderRect.w / 2;

    // considers Critter size
    float critterPosX = critter->getPosition().x;
    float critterPosY = critter->getPosition().y;

    // distance in each direction
    float differenceX = posX - critterPosX;
    float differenceY = posY - critterPosY;

    // distance formula
    return static_cast<float>(sqrt(pow(differenceX, 2) + pow(differenceY, 2)));
}

/**
 * @brief Mutator to sets the rectangle to render the Tower
 * @param targetRect new Rect to render the Tower through
 */
void Tower::setCurrentRenderRect(float originalX, float originalY, float w, float h) {
    // Calculate the center of the original position
    float centerX = originalX + currentRenderRect.w / 2.0f;
    float centerY = originalY + currentRenderRect.h / 2.0f;

    // Set the new width and height
    currentRenderRect.w = w;
    currentRenderRect.h = h;

    // Recalculate x and y to keep the same center based on the original X and Y
    currentRenderRect.x = centerX - w / 2.0f;
    currentRenderRect.y = centerY - h / 2.0f;
}


SDL_FRect Tower::getCurrentRenderRect() const {
    return currentRenderRect;
}

/**
 * @brief Mutator for rotation
 * @param angle new angle to set the Tower to 
 */
void Tower::setRotation(float angle) {
    rotationAngle = angle;
}

/**
 * @brief Renders the Towers as an image
 */
void Tower::render() {
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->generateProjectile();
    }

    // Render the tower texture
    towerTexture.render(currentRenderRect.x, currentRenderRect.y, nullptr, currentRenderRect.w, currentRenderRect.h, rotationAngle);
}

