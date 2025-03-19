#include <towers/powerups/IcePowerup.h>
#include <towers/Tower.h>
#include <towers/decorators/SlowingDecorator.h>

IcePowerup::IcePowerup(SDL_FRect position) : Powerup(position, "assets/tower/icePowerup.png") {}
IcePowerup::~IcePowerup() {}

Tower* IcePowerup::applyPowerupToTower(Tower* tower) {
    return new SlowingDecorator(tower);
}

bool IcePowerup::isClicked(float mouseX, float mouseY) const {
    return Powerup::isClicked(mouseX, mouseY);
}
