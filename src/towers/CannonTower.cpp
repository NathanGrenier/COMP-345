/**
 * @class CannonTower
 * @brief Implementation of CannonTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/CannonTower.h>
#include <towers/Projectile.h>
#include <util/AudioManager.h>

Mix_Chunk* CannonTower::towerShot = nullptr;

 // Initial Cost: 100
 // Max Cost: 600
const int CannonTower::upgradeCosts[] = { 200, 300 };

/**
 * @brief Default Constructor
 */
CannonTower::CannonTower() : Tower() {
	loadTextureForLevel();
	getTowerTexture().loadFromFile("tower/CannonTower/CannonTower1.png");
	upgradeValues.rangeIncrease = CannonTower::rangeIncreasePerLevel;
	upgradeValues.powerIncrease = CannonTower::powerIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = CannonTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
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
	: Tower(x, y, width, buyingCost, RANGE, POWER, RATE_OF_FIRE) {
	loadTextureForLevel();
	getTowerTexture().loadFromFile("tower/CannonTower/CannonTower1.png");
	upgradeValues.rangeIncrease = CannonTower::rangeIncreasePerLevel;
	upgradeValues.powerIncrease = CannonTower::powerIncreasePerLevel;
	upgradeValues.rateOfFireIncrease = CannonTower::rateOfFireIncreasePerLevel;
	upgradeValues.upgradeCosts = std::vector<int>(upgradeCosts, upgradeCosts + MAX_LEVEL - 1);
}

/**
 * @brief Accessor for maximum level
 *
 * @return The maximum level for CannonTower upgrades
 */
int CannonTower::getMaxLevel() {
	return MAX_LEVEL;
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
void CannonTower::shootProjectile(Critter* targettedCritter) {
	// Center of the tower
	float towerCenterX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2.0f;
	float towerCenterY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2.0f;
	float deltaAngle = 0;

	// Track target direction
	if (targettedCritter != nullptr) {
		Vector2D dirToTarget;
		dirToTarget.x = (targettedCritter->getPosition().x + targettedCritter->getPosition().w / 2.0f) - towerCenterX;
		dirToTarget.y = (targettedCritter->getPosition().y + targettedCritter->getPosition().h / 2.0f) - towerCenterY;

		// Calculate angle toward critter
		float angleRad = atan2(dirToTarget.y, dirToTarget.x);
		float angleDeg = angleRad * (180.0f / PI_CONSTANT);
		angleDeg += 90.0f; // adjust for sprite orientation

		deltaAngle = angleDeg - getRotation();

		while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
		while (deltaAngle < -180.0f) deltaAngle += 360.0f;

		float maxRotationStep = DEFAULT_TURN_SPEED * TURN_SPEED_FACTOR;
		deltaAngle = (deltaAngle > maxRotationStep) ? maxRotationStep :
			(deltaAngle < -maxRotationStep) ? -maxRotationStep : deltaAngle;

		setRotation(getRotation() + deltaAngle);
	}

	std::vector<Projectile*>& projectiles = getProjectiles();
	int shootingTimer = getShootingTimer();

	// Start animation if ready to fire
	if (!getIsAnimating() && shootingTimer <= 0 && fabs(deltaAngle) < 2.0f) {
		if (targettedCritter != nullptr) {
			setIsAnimating(true);
		}
	}
	else if (shootingTimer > 0) {
		setShootingTimer(shootingTimer - getRateOfFire());
	}

	// Handle shooting animation and projectile logic
	if (getIsAnimating()) {
		updateAnimation(0.08f);

		int triggerFrame = getFrameCount() / 2 - (getFrameCount() > 8 ? 3 : 2);

		if (getCurrentFrame() == triggerFrame && targettedCritter != nullptr) {
			float posX = getCurrentRenderRect().x + getCurrentRenderRect().w / 2;
			float posY = getCurrentRenderRect().y + getCurrentRenderRect().h / 2;

			float currentCellSize = Global::currentMap->getPixelPerCell();
			float critterPosX = targettedCritter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize / 2;
			float critterPosY = targettedCritter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize / 2;

			float diffX = critterPosX - posX;
			float diffY = critterPosY - posY;
			float distance = static_cast<float>(sqrt(diffX * diffX + diffY * diffY));

			float speedX = diffX / distance;
			float speedY = diffY / distance;

			projectiles.push_back(new Projectile(
				posX, posY, getPower(), false, 10, getRotation(),
				speedX, speedY, "tower/CannonTower/CannonProjectile.png"
			));

			Mix_PlayChannel(AudioManager::eEffectChannelTowerShot, towerShot, 0);

			setShootingTimer(MAX_SHOOTING_TIMER);
		}

		if (getCurrentFrame() == getFrameCount() - 1) {
			setIsAnimating(false);
		}
	}

	moveProjectiles(5, targettedCritter);
}


/**
 * @brief Loads the texture based on the CannonTower's level.
 */
void CannonTower::loadTextureForLevel() {
	std::string textureFileName;

	// Change texture based on the tower's level
	switch (getLevel()) {
	case 1:
		textureFileName = "tower/CannonTower/CannonTower1.png";
		setFrameCount(11);
		break;
	case 2:
		textureFileName = "tower/CannonTower/CannonTower2.png";
		setFrameCount(11);
		break;
	case 3:
		textureFileName = "tower/CannonTower/CannonTower3.png";
		setFrameCount(11);
		break;
	default:
		textureFileName = "tower/CannonTower/CannonTower1.png";
		setFrameCount(11);
		break;
	}

	// Load the appropriate texture for this level
	getTowerTexture().loadFromFile(textureFileName, true);
}
