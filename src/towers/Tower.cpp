/**
 * @class Tower
 * @brief Implementation of Tower Parent class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/Tower.h>
#include <towers/Projectile.h>
#include <towers/TargetNearExit.h>
#include <towers/TargetNearTower.h>
#include <towers/TargetStrongest.h>
#include <towers/TargetWeakest.h>
#include <iostream>
#include <string>

 /**
  * @brief Default Constructor, setting all values to 0
  */
Tower::Tower()
	: upgradeCost(0), buyingCost(0), range(0), power(0), rateOfFire(0), level(0), shootingTimer(0), upgradeValues{ 0, 0, 0 }, critterTargettingStrategy(new TargetNearExit()) {
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
	: upgradeCost(0), buyingCost(buyingCost), range(range), power(power), rateOfFire(rateOfFire), level(1), shootingTimer(0), upgradeValues{ 0, 0, 0 }, critterTargettingStrategy(new TargetNearExit()) {
	currentRenderRect = { x, y, width, width };
}

/**
 * @brief Finds a Critter to target
 *
 * @param critters Vector of currently existing Critter within the map
 * @details Iterates through all Critter in the map
 * Checks if each Critter is in range of the Tower
 * Stops after the first Critter that is in range of the Tower is found
 * @return Critter pointer for first Critter that is in range of the Tower
 * @return nullptr if no Critter is in range
 */
Critter* Tower::findCritter(std::vector<Critter*> critters) {
	return critterTargettingStrategy->targetCritter(critters, *this);
}

/**
 * @brief Clears projectiles previously fired from the Tower
 *
 * @details Used when targeted DummyCritter is invalid, usually when DummyCritter dies and target changes
 */
void Tower::clearProjectiles() {
	projectiles.clear();
}

/**
 * @brief Generates Projectiles fired by a Tower
 *
 * @details Iterates through the projectiles of a Tower to generate them, simply calling its respective function
 */
void Tower::generateAllProjectiles() {
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->updateAnimation(0.016f);
	}
}

/**
 * @brief Accessor for range
 * @return the range for the Tower
 */
int Tower::getRange() const {
	return range;
}

/**
 * @brief Accessor for power
 * @return the power for the Tower
 */
int Tower::getPower() const {
	return power;
}

/**
 * @brief Accessor for rate of fire
 * @return the rate of fire for the Tower
 */
int Tower::getRateOfFire() const {
	return rateOfFire;
}

/**
 * @brief Accessor for level
 * @return the level for the Tower
 */
int Tower::getLevel() const {
	return level;
}

/**
 * @brief Calculates the refund value of a Tower with levels in consideration
 *
 * @details Sums the initial cost of the tower plus all upgrades. Multiplied by the refund factor.
 * @return refund value of the tower
 */
int Tower::getRefundValue() {
	int upgradeCost = 0;
	for (int i = 0; i < level - 1; i++)
	{
		upgradeCost += upgradeValues.upgradeCosts[i];
	}
	return (buyingCost + upgradeCost) * REFUND_RATIO;
}

/**
 * @brief Accessor for buying cost
 * @return the cost for buying a Tower
 */
int Tower::getBuyingCost() {
	return buyingCost;
}

int Tower::getUpgradeCost() {
	if (level < getMaxLevel()) {
		return upgradeValues.upgradeCosts[level - 1];
	}
	return 0; // No cost if at max level
}

/**
 * @brief Accessor for the Tower upgrade values
 * @return a struct containing the Tower upgrade values
 */
Tower::UpgradeValues Tower::getUpgradeValues() const {
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
bool Tower::upgrade() {
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
bool Tower::isClicked(float scaleFactor) const {
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
bool Tower::isCritterInRange(Critter* critter) {
	float rangeInCells = static_cast<float>(getRange()) / Tower::STAT_CELL_RATIO;
	
	float relativeRange = rangeInCells * Global::currentMap->getPixelPerCell();
	float relativeDistance = calcDistance(critter);
	
	return relativeRange >= relativeDistance;
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
float Tower::calcDistance(Critter* critter) const {
	// considers Tower size
	float halfTowerSize = currentRenderRect.w / 2.0f;
	float posX = currentRenderRect.x + halfTowerSize;
	float posY = currentRenderRect.y + halfTowerSize;

	// considers Critter size
	float halfCritterSize = Global::currentMap->getPixelPerCell() * Critter::CRITTER_WIDTH_SCALE / 2.0f;
	float critterPosX = critter->getPosition().x + halfCritterSize;
	float critterPosY = critter->getPosition().y + halfCritterSize;

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

void Tower::setShootingTimer(int newShootingTimer) {
	shootingTimer = newShootingTimer;
}

/**
 * @brief Mutator for Tower Strategy
 * @param newStrategy the new TowerStrategy to use
 */
void Tower::setCritterTargettingStrategy(TowerStrategy* newStrategy) {
	critterTargettingStrategy = newStrategy;
	notify();
}

/**
 * @brief Accessor for Tower Strategy
 */
TowerStrategy* Tower::getCritterTargettingStrategy() {
	return critterTargettingStrategy;
}

/**
 * @brief Moves projectiles fired by a tower forward.
 * @param multiplier multiplier for moving the projectile slower/faster.
 * @param critter The critter that the tower should shoot at.
 */
void Tower::moveProjectiles(float multiplier, Critter* critter) {
	for (int i = 0; i < projectiles.size(); ) { // Note: no increment here
		projectiles[i]->move(multiplier / Tower::STAT_CELL_RATIO * Global::currentMap->getPixelPerCell());
		if (!projectiles[i]->isActive() || projectiles[i]->isOutside()) {
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
		}
		else {
			++i; // Only increment if no erasure
		}
	}
}

/**
 * @brief Renders the Towers as an image
 */
void Tower::render() {
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->generateProjectile();
	}

	// Render the tower texture
	towerTexture.render(currentRenderRect.x, currentRenderRect.y, nullptr, currentRenderRect.w, currentRenderRect.h, rotationAngle);
}
