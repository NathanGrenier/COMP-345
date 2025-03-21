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
    getTowerTexture().loadFromFile("assets/tower/CannonTower.png");
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
CannonTower::CannonTower(float x, float y, float width, int buyingCost)
    : Tower(x, y, width, buyingCost, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{
    getTowerTexture().loadFromFile("assets/tower/CannonTower.png");
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
CannonTower::CannonTower(float x, float y, float width, int buyingCost, int refundValue)
    : Tower(x, y, width, buyingCost, refundValue, CANNON_RANGE, CANNON_POWER, CANNON_RATE_OF_FIRE)
{
    getTowerTexture().loadFromFile("assets/tower/CannonTower.png");
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
void CannonTower::shootProjectile(Critter* targettedCritter)
{
    // Ensure we're using the center of the tower
    float towerCenterX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2.0f;
    float towerCenterY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2.0f;
    float deltaAngle = 0;

    // Target the center of the critter
    if (targettedCritter != nullptr) {
        Vector2D dirToTarget;
            dirToTarget.x = (targettedCritter->getPosition().x + targettedCritter->getPosition().w / 2.0f) - towerCenterX;
            dirToTarget.y = (targettedCritter->getPosition().y + targettedCritter->getPosition().h / 2.0f) - towerCenterY;

        // Calculate the raw angle
        float angleRad = atan2(dirToTarget.y, dirToTarget.x);
        float angleDeg = angleRad * (180.0f / PI_CONSTANT);

        // Adjust for sprite orientation (assuming "top" is default forward)
        angleDeg += 90.0f;

        deltaAngle = angleDeg - getRotation();

        // Normalize delta to [-180, 180] for shortest path
        while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        while (deltaAngle < -180.0f) deltaAngle += 360.0f;

        // Calculate max rotation step this frame
        float maxRotationStep = 180.0f * 0.016f;

        // Clamp rotation delta to avoid sudden jumps
        if (deltaAngle > maxRotationStep) deltaAngle = maxRotationStep;
        if (deltaAngle < -maxRotationStep) deltaAngle = -maxRotationStep;

        // Apply smooth rotation
        setRotation(getRotation() + deltaAngle);
    }

    std::vector<Projectile*>& projectiles = getProjectiles();

    // checks if it is time to shoot
    if (getShootingTimer() <= 0 && fabs(deltaAngle) < 2.0f)
    {
        if (targettedCritter != nullptr)
        {
            
            // tower position with offset
            float posX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2;
            float posY = getCurrentRenderRect().y + getCurrentRenderRect().w / 2;

        float currentCellSize = Global::currentMap->getPixelPerCell();

            // critter position with offset
            float critterPosX = targettedCritter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize / 2;
            float critterPosY = targettedCritter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize / 2;

            // differences in position from tower to cannon
            float differenceX = posX - critterPosX;
            float differenceY = posY - critterPosY;

        float distance = (float)sqrt(pow(differenceX, 2) + pow(differenceY, 2));

            // distance for projectile as a unit vector
            float speedX = (critterPosX - posX) / distance;
            float speedY = (critterPosY - posY) / distance;

            // fires a big sized projectile, resets shooting timer
            projectiles.push_back(new Projectile(posX, posY, getPower(), false, 6, getRotationAngle(), speedX, speedY, "assets/tower/CannonProjectile.png"));
            shootingTimer(MAX_SHOOTING_TIMER);
        }
    }
    else // decreases shooting timer
    {
        setShootingTimer(getShootingTimer() - getRateOfFire());
    }

    // moves projectile at a slow speed
    moveProjectiles(5, targettedCritter);
}