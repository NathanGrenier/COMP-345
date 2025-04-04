/**
 * @class StandardTower
 * @brief Implementation of StandardTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/StandardTower.h>
#include <towers/Projectile.h>
#include <util/AudioManager.h>

Mix_Chunk* StandardTower::towerShot = nullptr;

 // Initial Cost: 50
 // Max Cost: 400
const int StandardTower::upgradeCosts[] = { 50, 75, 100, 125 };

/**
 * @brief Default Constructor
 */
StandardTower::StandardTower() : Tower() {
	loadTextureForLevel();
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
	loadTextureForLevel();
	getTowerTexture().loadFromFile("tower/StandardTower/StandardTower1.png");
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
	static bool projectileShot = false;

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

	// Start animation only when allowed and not currently animating
	if (!getIsAnimating() && fabs(deltaAngle) < 2.0f) {
		if (shootingTimer <= 0 && targettedCritter != nullptr) {
			setIsAnimating(true);
		}
		else {
			setShootingTimer(shootingTimer - getRateOfFire());
		}
	}

	// Animation must continue until it ends
	if (getIsAnimating()) {
		updateAnimation(0.08f); // Progress the animation

		int triggerFrame = (getFrameCount() * 5) / 11;

		if (getCurrentFrame() == triggerFrame && targettedCritter != nullptr && !projectileShot) {
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

			projectiles.push_back(new Projectile(posX, posY, getPower(), false, 6, getRotation(), speedX, speedY, "tower/StandardTower/StandardProjectile.png"));
			Mix_PlayChannel(AudioManager::eEffectChannelTowerShot, towerShot, 0);
			projectileShot = true;

			setShootingTimer(MAX_SHOOTING_TIMER);
		}

		// Reset after the last animation frame
		if (getCurrentFrame() == getFrameCount() - 1) {
			setIsAnimating(false);
			projectileShot = false;
		}
	}

	moveProjectiles(10, targettedCritter);
}


/**
 * @brief Loads the texture based on the StandardTower's level.
 */
void StandardTower::loadTextureForLevel() {
	std::string textureFileName;

	// Change texture based on the tower's level
	switch (getLevel()) {
	case 1:
		textureFileName = "tower/StandardTower/StandardTower1.png";
		setFrameCount(8);
		break;
	case 5:
		textureFileName = "tower/StandardTower/StandardTower2.png";
		setFrameCount(11);
		break;
	default:
		textureFileName = "tower/StandardTower/StandardTower1.png";
		setFrameCount(8);
		break;
	}

	// Load the appropriate texture for this level
	getTowerTexture().loadFromFile(textureFileName, true);
}