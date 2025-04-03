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
	loadTextureForLevel();
	getTowerTexture().loadFromFile("tower/RapidFireTower/RapidFireTower1.png");
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
	loadTextureForLevel();
	getTowerTexture().loadFromFile("tower/RapidFireTower/RapidFireTower1.png");
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
	float towerCenterX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2.0f;
	float towerCenterY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2.0f;
	float deltaAngle = 0;

	if (targettedCritter != nullptr) {
		Vector2D dirToTarget;
		dirToTarget.x = (targettedCritter->getPosition().x + targettedCritter->getPosition().w / 2.0f) - towerCenterX;
		dirToTarget.y = (targettedCritter->getPosition().y + targettedCritter->getPosition().h / 2.0f) - towerCenterY;

		float angleRad = atan2(dirToTarget.y, dirToTarget.x);
		float angleDeg = angleRad * (180.0f / PI_CONSTANT);
		angleDeg += 90.0f;

		deltaAngle = angleDeg - getRotation();

		while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
		while (deltaAngle < -180.0f) deltaAngle += 360.0f;

		float maxRotationStep = DEFAULT_TURN_SPEED * TURN_SPEED_FACTOR;

		if (deltaAngle > maxRotationStep) deltaAngle = maxRotationStep;
		if (deltaAngle < -maxRotationStep) deltaAngle = -maxRotationStep;

		setRotation(getRotation() + deltaAngle);
	}

	std::vector<Projectile*>& projectiles = getProjectiles();
	int shootingTimer = getShootingTimer();

	// Start animation when conditions are met
	if (!isAnimating && fireBreak <= 0 && fabs(deltaAngle) < 2.0f) {
		if (shootingTimer <= 0 && targettedCritter != nullptr) {
			isAnimating = true;  // Animation starts
		}
		else {
			setShootingTimer(shootingTimer - getRateOfFire());
		}
	}
	else {
		burstCount = 0;
		fireBreak -= fireBreakRate;
	}

	// Animation should continue uninterrupted once started
	if (isAnimating) {
		updateAnimation(0.16f);

		// Calculate dynamic trigger frame
		int triggerFrame = getFrameCount() / 2 - (getFrameCount() > 8 ? 3 : 2);

		// Fire projectile when reaching the trigger frame
		if (getCurrentFrame() == triggerFrame && targettedCritter != nullptr) {
			float posX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2;
			float posY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2;

			float currentCellSize = Global::currentMap->getPixelPerCell();
			float critterPosX = targettedCritter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize / 2;
			float critterPosY = targettedCritter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize / 2;

			float differenceX = posX - critterPosX;
			float differenceY = posY - critterPosY;
			float distance = static_cast<float>(sqrt(pow(differenceX, 2) + pow(differenceY, 2)));

			float speedX = (critterPosX - posX) / distance;
			float speedY = (critterPosY - posY) / distance;

			projectiles.push_back(new Projectile(posX, posY, getPower(), false, 6, getRotation(), speedX, speedY, "tower/RapidFireTower/RapidFireProjectile.png"));

			setShootingTimer(MAX_SHOOTING_TIMER);
		}

		// Animation resets at last frame
		if (getCurrentFrame() == getFrameCount() - 1) {
			isAnimating = false;
		}

		if (burstCount == burstSize) {
			fireBreak = MAX_BREAK;
		}

		if (targettedCritter != nullptr) {
			burstCount++;
		}
	}

	moveProjectiles(10, targettedCritter);
}


/**
 * @brief Loads the texture based on the RapidFireTower's level.
 */
void RapidFireTower::loadTextureForLevel() {
	std::string textureFileName;

	// Change texture based on the tower's level
	switch (getLevel()) {
	case 1:
		textureFileName = "tower/RapidFireTower/RapidFireTower1.png";
		setFrameCount(8);
		break;
	case 2:
		textureFileName = "tower/RapidFireTower/RapidFireTower2.png";
		setFrameCount(8);
		break;
	case 3:
		textureFileName = "tower/RapidFireTower/RapidFireTower3.png";
		setFrameCount(8);
		break;
	default:
		textureFileName = "tower/RapidFireTower/RapidFireTower1.png";
		setFrameCount(8);
		break;
	}

	// Load the appropriate texture for this level
	getTowerTexture().loadFromFile(textureFileName, true);
}