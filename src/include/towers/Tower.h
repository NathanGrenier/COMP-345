/**
 * @file Tower.h
 * @brief Parent class for towers to fire at critters
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Parent class of all Towers to shoot at critters.
 * Implements calculating distance between tower and critter to find critters in range.
 * Also implements deleting projectiles, for when critters are killed but projectile is fired.
 */

#pragma once
#include <towers/Projectile.h>
#include <util/Observable.h>
#include <critter/Critter.h>
#include <ui/Texture.h>
#include <towers/TowerStrategy.h>
#include <string>

 /** @brief Ratio between original tower cost and refund value */
const double REFUND_RATIO = 0.8;

/**
 * @brief The Tower class is the parent class for all tower types that can shoot at critters.
 * It provides common functionality for tower attributes like range, power, rate of fire, and the ability to shoot projectiles.
 */
class Tower : public Observable {
public:
	static const int MAX_SHOOTING_TIMER = 100; /**< @brief Shooting timer to be decremented */
	static const int REFUND_PER_UPGRADE = 50; /**< @brief Additional gold refunded per level */
	static const int STAT_CELL_RATIO = 20; /**< @brief Ratio of Tower stat unit (range) per map cell */
	static constexpr float PI_CONSTANT = 3.14159265358979323846f; /**< @brief The constant PI for tower calculations */

	/**
	 * @brief Default constructor for Tower.
	 * Initializes a tower with default attributes.
	 */
	Tower();

	/**
	 * @brief Constructs a Tower with specific position, size, cost, range, power, and rate of fire.
	 * @param x The X-coordinate of the tower's position.
	 * @param y The Y-coordinate of the tower's position.
	 * @param width The width of the tower.
	 * @param buyingCost The cost to buy the tower.
	 * @param range The range of the tower.
	 * @param power The power of the tower.
	 * @param rateOfFire The rate of fire of the tower.
	 */
	Tower(float x, float y, float width, int buyingCost, int range, int power, int rateOfFire);

	/**
	 * @brief Constructs a Tower with specific position, size, cost, refund value, range, power, and rate of fire.
	 * @param x The X-coordinate of the tower's position.
	 * @param y The Y-coordinate of the tower's position.
	 * @param width The width of the tower.
	 * @param buyingCost The cost to buy the tower.
	 * @param refundValue The refund value when the tower is sold.
	 * @param range The range of the tower.
	 * @param power The power of the tower.
	 * @param rateOfFire The rate of fire of the tower.
	 */
	Tower(float x, float y, float width, int buyingCost, int refundValue, int range, int power, int rateOfFire);

	/**
	 * @brief Retrieves the projectiles fired by the tower.
	 * @return A reference to the vector of projectiles.
	 */
	virtual std::vector<Projectile*>& getProjectiles() { return projectiles; }

	/**
	 * @brief Finds a critter in the list of critters that is within the range of the tower.
	 * @param critters The list of critters to search.
	 * @return A pointer to the critter that is in range.
	 */
	virtual Critter* findCritter(std::vector<Critter*> critters);

	/**
	 * @brief Struct that contains the upgrade values for the tower.
	 * It defines the increase in range, power, and rate of fire upon upgrading.
	 */
	struct UpgradeValues {
		int rangeIncrease; /**< @brief The amount of range to increase upon upgrade */
		int powerIncrease; /**< @brief The amount of power to increase upon upgrade */
		int rateOfFireIncrease; /**< @brief The amount of rate of fire to increase upon upgrade */
	};

	UpgradeValues upgradeValues; /**< @brief The upgrade values for the tower */

	/**
	 * @brief Generates all projectiles for the tower.
	 * This function handles creating new projectiles when the tower shoots.
	 */
	virtual void generateAllProjectiles();

	/**
	 * @brief Retrieves the range of the tower.
	 * @return The range of the tower.
	 */
	virtual int getRange() const;

	/**
	 * @brief Retrieves the buying cost of the tower.
	 * @return The cost of the tower in gold.
	 */
	int getBuyingCost();

	/**
	 * @brief Retrieves the power of the tower.
	 * @return The power of the tower.
	 */
	virtual int getPower() const;

	/**
	 * @brief Retrieves the rate of fire of the tower.
	 * @return The rate of fire of the tower.
	 */
	virtual int getRateOfFire() const;

	/**
	 * @brief Retrieves the level of the tower.
	 * @return The level of the tower.
	 */
	virtual int getLevel() const;

	/**
	 * @brief Retrieves the refund value of the tower.
	 * @return The refund value of the tower.
	 */
	virtual int getRefundValue();

	/**
	 * @brief Retrieves the upgrade cost of the tower.
	 * @return The upgrade cost of the tower.
	 */
	virtual int getUpgradeCost();

	/**
	 * @brief Retrieves the maximum upgrade level for the tower.
	 * @return The maximum level of the tower.
	 */
	virtual int getMaxLevel() = 0;

	/**
	 * @brief Upgrades the tower if possible.
	 * @return true if the upgrade was successful, false otherwise.
	 */
	virtual bool upgrade();

	/**
	 * @brief Makes the tower shoot a projectile at a given critter.
	 * @param targettedCritter The critter that the tower should shoot at.
	 */
	virtual void shootProjectile(Critter* targettedCritter) = 0;

	/**
	 * @brief Moves projectiles fired by a tower forward.
	 * @param multiplier multiplier for moving the projectile slower/faster.
	 * @param critter The critter that the tower should shoot at.
	 */
	void moveProjectiles(float multiplier, Critter* critter);

	/**
	 * @brief Clears all projectiles fired by the tower.
	 * This is useful when a critter is killed but the projectile is still in flight.
	 */
	virtual void clearProjectiles();

	/**
	 * @brief Checks if the tower has been clicked.
	 * @param scaleFactor The scale factor for the click detection.
	 * @return true if the tower was clicked, false otherwise.
	 */
	virtual bool isClicked(float scaleFactor) const;

	/**
	 * @brief Checks if a critter is within range of the tower.
	 * @param critter The critter to check.
	 * @return true if the critter is in range, false otherwise.
	 */
	virtual bool isCritterInRange(Critter* critter);

	/**
	 * @brief Retrieves the upgrade values for the tower.
	 * @return The upgrade values for the tower.
	 */
	virtual UpgradeValues getUpgradeValues() const;

	/**
	 * @brief Renders the tower on the screen.
	 */
	virtual void render();

	/**
	 * @brief Sets the rotation angle for the tower.
	 * @param angle The rotation angle to set for the tower.
	 */
	virtual void setRotation(float angle);

	/**
	 * @brief Retrieves the current rotation angle of the tower.
	 * @return The rotation angle of the tower.
	 */
	virtual float getRotation() const { return rotationAngle; }

	/**
	 * @brief Sets the render rectangle for the tower.
	 * @param originalX The X-coordinate of the render rectangle.
	 * @param originalY The Y-coordinate of the render rectangle.
	 * @param w The width of the render rectangle.
	 * @param h The height of the render rectangle.
	 */
	virtual void setCurrentRenderRect(float originalX, float originalY, float w, float h);

	/**
	 * @brief Retrieves the current render rectangle for the tower.
	 * @return The current render rectangle.
	 */
	virtual SDL_FRect getCurrentRenderRect() const;

	/**
	 * @brief Retrieves the texture for the tower.
	 * @return A reference to the tower's texture.
	 */
	virtual Texture& getTowerTexture() { return towerTexture; };

	/**
	 * @brief Retrieves the shooting timer for the tower.
	 * @return A reference to the shooting timer.
	 */
	virtual int& getShootingTimer() { return shootingTimer; };

	/**
	 * @brief Sets the shooting timer for the tower.
	 * @param newShootingTimer The new value for the shooting timer of the Tower.
	 */
	virtual void setShootingTimer(int newShootingTimer);

	/**
	 * @brief Mutator for Tower Strategy
	 * @param newStrategy the new TowerStrategy to use
	 */
	virtual void setCritterTargettingStrategy(TowerStrategy* newStrategy);

	/**
	 * @brief Accessor for Tower Strategy
	 * @returns the TargettingStrategy object that determines the Tower's targetting pattern
	 */
	virtual TowerStrategy* getCritterTargettingStrategy();

	/**
	 * @brief Calculates the distance between the tower and a critter.
	 * @param critter The critter to calculate the distance to.
	 * @return The calculated distance between the tower and the critter.
	 */
	float calcDistance(Critter* critter) const;

private:
	SDL_FRect currentRenderRect; /**< @brief The current render rectangle for the tower */
	int buyingCost; /**< @brief The cost to purchase the tower */
	int upgradeCost; /**< @brief The cost to upgrade the tower */
	int refundValue; /**< @brief The refund value when selling the tower */
	float rotationAngle = 0.0f; /**< @brief The rotation angle of the tower */
	int range; /**< @brief The range of the tower */
	int power; /**< @brief The power of the tower */
	int rateOfFire; /**< @brief The rate of fire of the tower */
	int level; /**< @brief The level of the tower */
	int shootingTimer; /**< @brief The shooting timer of the tower */

	std::vector<Projectile*> projectiles; /**< @brief A vector of all projectiles fired by the tower */
	Texture towerTexture; /**< @brief The texture of the tower */

	TowerStrategy* critterTargettingStrategy; /**< @brief The TowerStrategy object for the tower to use */
};
