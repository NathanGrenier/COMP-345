/**
 * @class StandardTower
 * @brief Implementation of StandardTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/StandardTower.h>
#include <towers/Projectile.h>

/**
 * @brief Default Constructor
 */
StandardTower::StandardTower() : Tower()
{
    towerTexture.loadFromFile("assets/tower/StandardTower.png");
    upgradeValues.rangeIncrease = 20;
    upgradeValues.powerIncrease = 1;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Constructor with position and buying cost 
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying StandardTower
 * @details Constructor for StandardTower with x, y position and buying cost
 * Uses default range, power, and rate of fire for StandardTower
 * Uses default refund value ratio in Tower class 
 */
StandardTower::StandardTower(float x, float y, int buyingCost)
    : Tower(x, y, buyingCost, STANDARD_RANGE, STANDARD_POWER, STANDARD_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/StandardTower.png");
    upgradeValues.rangeIncrease = 20;
    upgradeValues.powerIncrease = 1;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Constructor with position, buying cost, and refund value
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param buyingCost Cost of buying StandardTower
 * @param refundValue Amount of coins refunded when selling a StandardTower
 * @details Constructor for StandardTower with x, y position, buying cost, and refund value
 * Uses default range, power, and rate of fire for StandardTower
 */
StandardTower::StandardTower(float x, float y, int buyingCost, int refundValue)
    : Tower(x, y, buyingCost, refundValue, STANDARD_RANGE, STANDARD_POWER, STANDARD_RATE_OF_FIRE)
{
    towerTexture.loadFromFile("assets/tower/StandardTower.png");
    upgradeValues.rangeIncrease = 20;
    upgradeValues.powerIncrease = 1;
    upgradeValues.rateOfFireIncrease = 1;
}

/**
 * @brief Accessor for maximum level
 *
 * @return The maximum level for StandardTower upgrades
 */
int StandardTower::getMaxLevel()
{
    return STANDARD_MAX_LEVEL;
}

/**
 * @brief Fires and moves projectiles for the StandardTower
 * 
 * @param critter Critter targetted by the StandardTower
 * @details Checks if Critter exists and can still be targeted by StandardTower
 * Calculates horizontal and vertical distance (giving direction) for projectile to move 
 * Times the projectiles to be shot out of the StandardTower, StandardTower has a moderate rate of fire
 * Applies damage to targetted Critter if the Projectile is in collision
 * Gets rid of Projectiles when needed, either if out of bounds, Critter is already dead, or when collision is made with Critter
 */
void StandardTower::shootProjectile(Critter* critter)
{
    // Ensure we're using the center of the tower
    float towerCenterX = x + currentRenderRect.w / 2.0f;
    float towerCenterY = y + currentRenderRect.h / 2.0f;

    // Target the center of the critter
    Vector2D dirToTarget;
    if (critter != nullptr) {
        dirToTarget.x = (critter->getPosition().x + critter->getPosition().w / 2.0f) - towerCenterX;
        dirToTarget.y = (critter->getPosition().y + critter->getPosition().h / 2.0f) - towerCenterY;
    }

    // Calculate the raw angle
    float angleRad = atan2(dirToTarget.y, dirToTarget.x);
    float angleDeg = angleRad * (180.0f / M_PI);

    // Adjust for sprite orientation (assuming "top" is default forward)
    angleDeg += 90.0f;

    float deltaAngle = angleDeg - rotationAngle;

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

     // checks if it is time to shoot
    if (critter != nullptr && shootingTimer <= 0 && fabs(deltaAngle) < 2.0f)
    {
        // tower position with offset
        float posX = currentRenderRect.x + currentRenderRect.w / 2;
        float posY = currentRenderRect.y + currentRenderRect.w / 2;

        SDL_FRect currentCellSize = Global::currentMap->getPixelPerCell();

        // critter position with offset
        float critterPosX = critter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize.w / 2;
        float critterPosY = critter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize.h / 2;

        // differences in position from tower to cannon
        float differenceX = posX - critterPosX;
        float differenceY = posY - critterPosY;

        float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

        // distance for projectile as a unit vector
        float speedX = (critterPosX - posX) / distance;
        float speedY = (critterPosY - posY) / distance;

        // fires a projectile with the default size, resets shooting timer
        projectiles.push_back(new Projectile(posX, posY, power, false, rotationAngle, speedX, speedY, "assets/tower/StandardProjectile.png"));
        shootingTimer = MAX_SHOOTING_TIMER;
    }
    else // decreases shooting timer
    {
        shootingTimer -= rateOfFire;
    }

    // moves projectiles at a fast speed
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(10);

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