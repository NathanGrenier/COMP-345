/**
 * @class RapidFireTower
 * @brief Implementation of RapidFireTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/RapidFireTower.h>
#include <towers/Projectile.h>

 // Initial Cost: 75
 // Max Cost: 375
const int RapidFireTower::upgradeCosts[] = { 100, 200 };

/**
 * @brief Default Constructor
 */
RapidFireTower::RapidFireTower() : Tower(), fireBreak(0), fireBreakRate(0), burstSize(0), burstCount(0) {
	getTowerTexture().loadFromFile("tower/RapidFireTower.png");
	upgradeValues.rangeIncrease = RapidFireTower::rangeIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = RapidFireTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
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
	: fireBreak(0), fireBreakRate(5), burstSize(50), burstCount(0), Tower(x, y, width, buyingCost, RANGE, POWER, RATE_OF_FIRE) {
	getTowerTexture().loadFromFile("tower/RapidFireTower.png");
	upgradeValues.rangeIncrease = RapidFireTower::rangeIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = RapidFireTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
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
bool RapidFireTower::upgrade() {
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
int RapidFireTower::getMaxLevel() {
	return MAX_LEVEL;
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
void RapidFireTower::shootProjectile(Critter* targettedCritter) {
	// Ensure we're using the center of the tower
	float towerCenterX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2.0f;
	float towerCenterY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2.0f;
	float deltaAngle = 0;

	// Check if there's a valid critter to target
	if (targettedCritter != nullptr)
	{
		// Target the center of the critter
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
		float maxRotationStep = DEFAULT_TURN_SPEED * TURN_SPEED_FACTOR;

		// Clamp rotation delta to avoid sudden jumps
		if (deltaAngle > maxRotationStep) deltaAngle = maxRotationStep;
		if (deltaAngle < -maxRotationStep) deltaAngle = -maxRotationStep;

		// Apply smooth rotation
		setRotation(getRotation() + deltaAngle);
	}

	std::vector<Projectile*>& projectiles = getProjectiles();
	int shootingTimer = getShootingTimer();

	// checks if it is a shooting interval
	if (fireBreak <= 0 && fabs(deltaAngle) < 2.0f)
	{
		// checks if it is time to shoot within the interval
		if (shootingTimer <= 0)
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

				float distance = static_cast<float>(sqrt(pow(differenceX, 2) + pow(differenceY, 2)));

				// distance for projectile as a unit vector
				float speedX = (critterPosX - posX) / distance;
				float speedY = (critterPosY - posY) / distance;

				// fires a projectile, resets shooting timer
				projectiles.push_back(new Projectile(posX, posY, getPower(), false, 6, getRotation(), speedX, speedY, "tower/RapidFireProjectile.png"));
				setShootingTimer(MAX_SHOOTING_TIMER);
			}
		} else
		{
			setShootingTimer(shootingTimer - getRateOfFire());
		}

		// if maximum interval time is reached
		if (burstCount == burstSize)
		{
			fireBreak = MAX_BREAK;
		}

		if (targettedCritter != nullptr)
		{
			burstCount++;
		}
	} else // break from firing interval, no Projectiles to be fired
	{
		burstCount = 0;
		fireBreak -= fireBreakRate;
	}

	// moves projectile at a moderate speed
	moveProjectiles(10, targettedCritter);
}