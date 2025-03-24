/**
 * @file Powerup.h
 * @brief Base class for all power-ups in the game
 * @author Nirav Patel
 * @date 2025-03-19
 *
 * @details This class defines the base functionality for all power-ups in the game, including rendering, updating,
 * checking for clicks, and applying power-ups to towers. Derived classes will define specific power-up behavior.
 */

#pragma once

#include <SDL3/SDL.h>
#include <towers/Tower.h>
#include <Global.h>
#include <cmath>
#include <ui/LTexture.h>

 /**
  * @class Powerup
  * @brief The base class for all power-ups.
  *
  * This class defines the common attributes and methods for all power-ups, including rendering and applying them to towers.
  * Specific power-up types (e.g., damage increase, range increase) should inherit from this class and implement the
  * `applyPowerupToTower` function to apply their specific effects.
  */
class Powerup {
public:
	/**
	 * @brief Constructs a Powerup object with a specific position and texture.
	 * @param position The position of the powerup in the game world.
	 * @param powerupPath The file path to the texture of the powerup.
	 */
	Powerup(SDL_FRect position, std::string powerupPath);

	/**
	 * @brief Destructor for the Powerup class.
	 * Cleans up any allocated resources, such as textures.
	 */
	virtual ~Powerup();

	/**
	 * @brief Checks if the powerup was clicked by the player.
	 * @param mouseX The X-coordinate of the mouse cursor.
	 * @param mouseY The Y-coordinate of the mouse cursor.
	 * @return true if the powerup was clicked, false otherwise.
	 */
	virtual bool isClicked(float mouseX, float mouseY) const;

	/**
	 * @brief Applies the powerup effect to a given tower.
	 * This is a pure virtual function, meant to be implemented by derived classes to apply specific power-up effects to towers.
	 * @param tower The tower to apply the powerup to.
	 * @param towerPosition The position of the tower.
	 * @return A pointer to the modified tower.
	 */
	virtual Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) = 0;

	/**
	 * @brief Renders the powerup on the screen.
	 * This function draws the powerup at its current position on the screen.
	 */
	void render();

	/**
	 * @brief Updates the powerup's state.
	 * This method updates the powerup's frame (for animation) and applies the bobbing effect.
	 * @param deltaTime The time elapsed since the last frame, used for frame cycling and updating the bobbing effect.
	 */
	void update(float deltaTime);

	SDL_FRect position; /**< @brief The current position of the powerup in the game world */

public:
	bool isDragged = false; /**< @brief Indicates whether the powerup is being dragged by the player */
	bool markForDespawn = false;

protected:
	LTexture powerupTexture; /**< @brief The texture used for displaying the powerup */
	LTexture indicatorTexture; /**< @brief The texture used for displaying the powerup's indicator */

	// Frame cycling variables for animation
	int currentFrame; /**< @brief The current frame of the powerup's animation (0-3) */
	float frameTime; /**< @brief The accumulated time for frame cycling */
	float frameDuration; /**< @brief The duration of each frame in the animation (1/60th of a second for 60 FPS) */

	// Bobbing effect variables
	float bobbingSpeed; /**< @brief The speed at which the powerup bobs up and down */
	float bobbingHeight; /**< @brief The height of the bobbing movement */
	float bobbingOffset; /**< @brief The current vertical offset for the bobbing effect */

	float lifetime;      // Total lifetime before despawning
	float elapsedTime;   // Tracks how long the powerup has existed
	float flickerStartTime; // When the flickering should begin
	bool isVisible;

	static constexpr float SIZE_SCALE_FACTOR = 0.8f;
};
