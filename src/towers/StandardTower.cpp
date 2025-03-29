/**
 * @class StandardTower
 * @brief Implementation of StandardTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/StandardTower.h>
#include <towers/Projectile.h>

 // Initial Cost: 50
 // Max Cost: 400
const int StandardTower::upgradeCosts[] = { 50, 75, 100, 125 };

/**
 * @brief Default Constructor
 */
StandardTower::StandardTower() : Tower() {
	getTowerTexture().loadFromFile("tower/StandardTower.png");
	upgradeValues.rangeIncrease = StandardTower::rangeIncreasePerLevel;
	upgradeValues.powerIncrease = StandardTower::powerIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = StandardTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
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
StandardTower::StandardTower(float x, float y, float width, int buyingCost)
	: Tower(x, y, width, buyingCost, RANGE, POWER, RATE_OF_FIRE) {
	getTowerTexture().loadFromFile("tower/StandardTower.png");
	upgradeValues.rangeIncrease = StandardTower::rangeIncreasePerLevel;
	upgradeValues.powerIncrease = StandardTower::powerIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = StandardTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
}

/**
 * @brief Accessor for maximum level
 *
 * @return The maximum level for StandardTower upgrades
 */
int StandardTower::getMaxLevel() {
	return MAX_LEVEL;
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
void StandardTower::shootProjectile(Critter* targettedCritter) {
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
		float maxRotationStep = DEFAULT_TURN_SPEED * TURN_SPEED_FACTOR;

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

			// fires a projectile with the default size, resets shooting timer
			projectiles.push_back(new Projectile(posX, posY, getPower(), false, 6, getRotation(), speedX, speedY, "tower/StandardProjectile.png"));
			setShootingTimer(MAX_SHOOTING_TIMER);
		}
	} else // decreases shooting timer
	{
		setShootingTimer(getShootingTimer() - getRateOfFire());
	}

	// moves projectiles at a fast speed
	moveProjectiles(10, targettedCritter);
}