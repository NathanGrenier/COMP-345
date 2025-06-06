/**
 * @file Projectile.h
 * @brief Projectile fired from towers, damaging critters
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class that provides a visual indicator for a tower firing
 * Projectiles of different speeds and sizes are fired from towers
 * Damages critter when collision is detected
 * isArea still needs to be implemented for damage in an area
 */

#pragma once

#include <critter/Critter.h>
#include <ui/Texture.h>

 /**
  * @class Projectile
  * @brief Represents a projectile fired from a tower that damages critters.
  *
  * The Projectile class handles the movement, animation, and collision detection
  * for projectiles fired by towers. It also handles different types of projectiles
  * including those with area-of-effect damage (to be implemented).
  */
class Projectile {
public:
	/**
	 * @brief Constructor with position, damage, and if projectile damages an area for a non-homing projectile
	 *
	 * Initializes the projectile's position, damage, speed, rotation angle,
	 * and texture for rendering. This version does not take size into account.
	 *
	 * @param x The x position of the projectile.
	 * @param y The y position of the projectile.
	 * @param damage The amount of damage the projectile does to critters.
	 * @param isArea Whether the projectile has an area of effect (not yet implemented).
	 * @param rotationAngle The rotation angle of the projectile.
	 * @param xSpeed The horizontal speed of the projectile.
	 * @param ySpeed The vertical speed of the projectile.
	 * @param texturePath The file path to the texture for the projectile.
	 */
	Projectile(float x, float y, int damage, bool isArea, float rotationAngle, float xSpeed, float ySpeed, std::string texturePath);

	/**
	 * @brief Constructor with position, damage, if projectile damages an area, and projectile size for a non-homing projectile
	 *
	 * Initializes the projectile's position, damage, speed, size, rotation angle,
	 * and texture for rendering. This version supports custom projectile sizes.
	 *
	 * @param x The x position of the projectile.
	 * @param y The y position of the projectile.
	 * @param damage The amount of damage the projectile does to critters.
	 * @param isArea Whether the projectile has an area of effect (not yet implemented).
	 * @param projectileSize The size of the projectile.
	 * @param rotationAngle The rotation angle of the projectile.
	 * @param xSpeed The horizontal speed of the projectile.
	 * @param ySpeed The vertical speed of the projectile.
	 * @param texturePath The file path to the texture for the projectile.
	 */
	Projectile(float x, float y, int damage, bool isArea, int projectileSize, float rotationAngle, float xSpeed, float ySpeed, std::string texturePath);

	/**
	 * @brief Constructor with position, damage, and if projectile damages an area for a homing projectile
	 *
	 * Initializes the projectile's position, damage, speed, rotation angle,
	 * and texture for rendering. This version does not take size into account.
	 *
	 * @param x The x position of the projectile.
	 * @param y The y position of the projectile.
	 * @param damage The amount of damage the projectile does to critters.
	 * @param isArea Whether the projectile has an area of effect (not yet implemented).
	 * @param rotationAngle The rotation angle of the projectile.
	 * @param speed The speed of the projectile.
	 * @param texturePath The file path to the texture for the projectile.
	 */
	Projectile(float x, float y, int damage, bool isArea, float rotationAngle, float speed, Critter* targettedCritter, std::string texturePath);

	/**
	 * @brief Constructor with position, damage, if projectile damages an area, and projectile size for a homing projectile
	 *
	 * Initializes the projectile's position, damage, speed, size, rotation angle,
	 * and texture for rendering. This version supports custom projectile sizes.
	 *
	 * @param x The x position of the projectile.
	 * @param y The y position of the projectile.
	 * @param damage The amount of damage the projectile does to critters.
	 * @param isArea Whether the projectile has an area of effect (not yet implemented).
	 * @param projectileSize The size of the projectile.
	 * @param rotationAngle The rotation angle of the projectile.
	 * @param speed The speed of the projectile.
	 * @param texturePath The file path to the texture for the projectile.
	 */
	Projectile(float x, float y, int damage, bool isArea, int projectileSize, float rotationAngle, float speed, Critter* targettedCritter, std::string texturePath);

	/**
	 * @brief Returns the damage value of the projectile.
	 *
	 * @return The amount of damage the projectile will inflict on critters.
	 */
	int getDamage() const;

	/**
	 * @brief Returns whether the projectile has an area-of-effect damage.
	 *
	 * @return A boolean indicating if the projectile has an area of effect.
	 */
	int getIsArea();

	/**
	 * @brief Moves the projectile based on its speed.
	 *
	 * This function updates the position of the projectile based on its speed,
	 * multiplying it by a given multiplier (usually a frame delta).
	 *
	 * @param multiplier The multiplier that adjusts the movement speed.
	 */
	void move(float multiplier);

	/**
	 * @brief Generates the projectile by loading the texture and preparing it for rendering.
	 */
	void generateProjectile();

	/**
	 * @brief Checks if projectile is outside the map area
	 *
	 * @details Check if projectile is either horizontally or vertically out of bounds
	 * Checks both lower and upper bounds
	 * Lower bounds being 0,
	 * Upper bounds being the screen size in pixels
	 * @return true if the Projectile is out of bounds
	 * @return false if the Projectile is positioned correctly within the map
	 */
	bool isOutside() const;

	/**
	 * @brief Checks if the projectile collides with a critter.
	 *
	 * @param critter The critter to check for collision with.
	 * @return A boolean indicating whether a collision occurred.
	 */
	bool checkCollision(Critter* critter) const;

	/**
	 * @brief Checks if the projectile collides with the targetted critter.
	 * This check is for homing projectiles.
	 *
	 * @return A boolean indicating whether a collision occurred with the targetted Critter.
	 */
	bool checkCollision();

	bool isActive();

	void setIsActive(bool active);

	/**
	 * @brief Updates the projectile's animation frame based on delta time.
	 *
	 * This method is responsible for updating the sprite animation of the projectile.
	 * It cycles through frames at a set frame rate.
	 *
	 * @param deltaTime The time difference between frames, used to update animation.
	 */
	void updateAnimation(float deltaTime);

	/**
	 * @brief Accessor for targetted Critter in the case of homing Projectile
	 *
	 * @return nullptr if the Projectile is non-homing
	 * @return the pointer to the targettedCritter if the Projectile is homing
	 */
	Critter* getTargettedCritter();

private:
	float x; /**< The x position of the projectile. */
	float y; /**< The y position of the projectile. */
	bool active; /**< Indicates if the projectile is active. */
	int damage; /**< The amount of damage the projectile inflicts. */
	int projectileSize; /**< The size of the projectile. */
	bool isArea; /**< Indicates if the projectile has area-of-effect damage (not implemented). */

	Texture projectileTexture; /**< The texture for rendering the projectile. */
	SDL_FRect currentRenderRect; /**< The current rectangle for rendering the projectile. */

	int currentFrame = 0; /**< The current frame index for projectile animation. */
	int frameCount = 4; /**< The total number of frames in the projectile's sprite sheet. */

	float frameTimer = 0.0f; /**< Timer to track the time elapsed between frames for animation. */
	float frameDuration = 0.1f; /**< Duration of each frame in the animation (10 fps). */

	float rotationAngle; /**< The rotation angle of the projectile. */
	float xSpeed; /**< The horizontal speed of a non-homing projectile. */
	float ySpeed; /**< The vertical speed of  a non-homing projectile. */
	float speed; /**< The overall speed of a homing projectile. */
	Critter* const targettedCritter;/**< The target of a homing projectile. */
};
