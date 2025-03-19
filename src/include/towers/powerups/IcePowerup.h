#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/decorators/SlowingDecorator.h>

class IcePowerup : public Powerup {
public:
    IcePowerup(SDL_FRect position);
    ~IcePowerup() override;

    Tower* applyPowerupToTower(Tower* tower) override;
    bool isClicked(float mouseX, float mouseY) const override;
};