/**
 * @file IcePowerup.h
 * @brief IcePowerup class to apply a slowing effect to towers
 * @author Nirav Patel
 * @date 2025-03-19
 *
 * @details This class defines a specific powerup (IcePowerup) that, when applied to a tower, will add a slowing effect to the tower
 * by using a `SlowingDecorator`. It extends the `Powerup` class and implements the behavior for applying the powerup and checking clicks.
 */

#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/Tower.h>
#include <towers/decorators/SlowingDecorator.h>

 /**
  * @class IcePowerup
  * @brief A powerup that applies a slowing effect to towers.
  *
  * This class is a derived class of `Powerup` that, when activated, applies a slowing effect to the given tower. The effect is
  * achieved by decorating the tower with a `SlowingDecorator`. It also manages rendering and click detection for the powerup.
  */
class IcePowerup : public Powerup {
public:
	/**
	 * @brief Constructs an IcePowerup object.
	 *
	 * This constructor initializes the `IcePowerup` with a specified position and a texture for the powerup.
	 * The texture file for the powerup is loaded from the provided file path.
	 *
	 * @param position The position of the powerup in the game world.
	 */
	IcePowerup(SDL_FRect position) : Powerup(position, "tower/icePowerup.png") {}

	/**
	 * @brief Applies the ice powerup effect to the given tower.
	 *
	 * When this powerup is applied to a tower, it wraps the tower with a `SlowingDecorator`, which adds a slowing effect to
	 * the tower's attacks. The `SlowingDecorator` is initialized with the tower, its position, and a texture for the indicator.
	 *
	 * @param tower The tower to apply the powerup to.
	 * @param towerPosition The position of the tower in the game world.
	 * @return A new tower decorated with a `SlowingDecorator` that applies the slowing effect.
	 */
	Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) override {
		return new SlowingDecorator(tower, towerPosition, "tower/iceIndicator.png");
	}

	/**
	 * @brief Checks if the powerup was clicked by the player.
	 *
	 * Uses the base class implementation of the `isClicked` function to determine if the mouse cursor is within the bounds of the powerup.
	 *
	 * @param mouseX The X-coordinate of the mouse cursor.
	 * @param mouseY The Y-coordinate of the mouse cursor.
	 * @return true if the mouse cursor is within the bounds of the powerup, false otherwise.
	 */
	bool isClicked(float mouseX, float mouseY) const override {
		return Powerup::isClicked(mouseX, mouseY);
	}
};
