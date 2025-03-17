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
RapidFireTower::RapidFireTower(float x, float y, float width, int buyingCost)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, width, buyingCost, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
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
RapidFireTower::RapidFireTower(float x, float y, float width, int buyingCost, int refundValue)
    : fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, width, buyingCost, refundValue, RAPID_RANGE, RAPID_POWER, RAPID_RATE_OF_FIRE)
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
void RapidFireTower::shootProjectile(Critter* critter)
{
    // Ensure we're using the center of the tower
    float towerCenterX = currentRenderRect.x + currentRenderRect.w / 2.0f;
    float towerCenterY = currentRenderRect.y + currentRenderRect.h / 2.0f;
    float deltaAngle;

    // Check if there's a valid critter to target
    if (critter != nullptr)
    {
        // Target the center of the critter
        Vector2D dirToTarget;
        dirToTarget.x = (critter->getPosition().x + critter->getPosition().w / 2.0f) - towerCenterX;
        dirToTarget.y = (critter->getPosition().y + critter->getPosition().h / 2.0f) - towerCenterY;

        // Calculate the raw angle
        float angleRad = atan2(dirToTarget.y, dirToTarget.x);
        float angleDeg = angleRad * (180.0f / M_PI);

        // Adjust for sprite orientation (assuming "top" is default forward)
        angleDeg += 90.0f;

        deltaAngle = angleDeg - rotationAngle;

        // Normalize delta to [-180, 180] for shortest path
        while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        while (deltaAngle < -180.0f) deltaAngle += 360.0f;

        // Calculate max rotation step this frame
        float maxRotationStep = 180.0f * 0.016f;

        // Clamp rotation delta to avoid sudden jumps
        if (deltaAngle > maxRotationStep) deltaAngle = maxRotationStep;
        if (deltaAngle < -maxRotationStep) deltaAngle = -maxRotationStep;

        // Apply smooth rotation
        rotationAngle = rotationAngle + deltaAngle;
    }

    // checks if it is a shooting interval
    if (critter != nullptr && fireBreak <= 0 && fabs(deltaAngle) < 2.0f)
    {
        // checks if it is time to shoot within the interval
        if (shootingTimer <= 0)
        {
            // tower position with offset
            float posX = currentRenderRect.x + currentRenderRect.w / 2;
            float posY = currentRenderRect.y + currentRenderRect.w / 2;

            float currentCellSize = Global::currentMap.getPixelPerCell();

            // critter position with offset
            float critterPosX = critter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize / 2;
            float critterPosY = critter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize / 2;

            // differences in position from tower to cannon
            float differenceX = posX - critterPosX;
            float differenceY = posY - critterPosY;

            float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

            // distance for projectile as a unit vector
            float speedX = (critterPosX - posX) / distance;
            float speedY = (critterPosY - posY) / distance;

            // fires a projectile, resets shooting timer
            projectiles.push_back(new Projectile(posX, posY, power, false, 3, rotationAngle, speedX, speedY, "assets/tower/RapidFireProjectile.png"));
            shootingTimer = MAX_SHOOTING_TIMER;
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
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(5);

        // check if projectile hits critter
        if (critter != nullptr && projectiles[i]->checkCollision(critter))
        {
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
