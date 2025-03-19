#include <towers/powerups/FirePowerup.h>
#include <towers/Tower.h>

FirePowerup::FirePowerup(SDL_FRect position)
    : Powerup(position, "assets/tower/firePowerup.png") {}

FirePowerup::~FirePowerup() {}

Tower* FirePowerup::applyPowerupToTower(Tower* tower) {
    return new BurningDecorator(tower);
}

bool FirePowerup::isClicked(float mouseX, float mouseY) const {
    return Powerup::isClicked(mouseX, mouseY);
}