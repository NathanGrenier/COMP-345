#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/decorators/BurningDecorator.h>

class FirePowerup : public Powerup {
public:
    FirePowerup(SDL_FRect position);
    ~FirePowerup() override;

    Tower* applyPowerupToTower(Tower* tower) override;
    bool isClicked(float mouseX, float mouseY) const override;
};
