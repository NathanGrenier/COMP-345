#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/Tower.h>
#include <towers/decorators/BurningDecorator.h>

/**
 * @class FirePowerup
 * @brief A powerup that applies a burning effect to a tower.
 *
 * The FirePowerup class is a derived class from Powerup that, when applied to a tower,
 * decorates the tower with a BurningDecorator, giving the tower the ability to burn enemies.
 */
class FirePowerup : public Powerup {
public:
	/**
	 * @brief Constructs a FirePowerup at a specified position.
	 *
	 * Initializes the FirePowerup with a given position and assigns an image file path
	 * for its representation.
	 *
	 * @param position The position of the FirePowerup in the game world.
	 */
	FirePowerup(SDL_FRect position) : Powerup(position, "tower/firePowerup.png") {}

	/**
	 * @brief Applies the fire powerup to a tower.
	 *
	 * This method decorates the given tower with a BurningDecorator, which modifies
	 * the behavior of the tower to apply burning effects to enemies.
	 *
	 * @param tower The tower to apply the powerup to.
	 * @param towerPosition The position of the tower.
	 * @return A new tower wrapped in a BurningDecorator.
	 */
	Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) override {
		return new BurningDecorator(tower, towerPosition, "tower/fireIndicator.png");
	}

	/**
	 * @brief Checks if the powerup is clicked.
	 *
	 * This method checks whether the mouse click occurred within the bounds of the
	 * FirePowerup.
	 *
	 * @param mouseX The X coordinate of the mouse click.
	 * @param mouseY The Y coordinate of the mouse click.
	 * @return true if the powerup is clicked, false otherwise.
	 */
	bool isClicked(float mouseX, float mouseY) const override {
		return Powerup::isClicked(mouseX, mouseY);
	}
};
