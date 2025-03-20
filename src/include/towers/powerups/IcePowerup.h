#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/Tower.h>
#include <towers/decorators/SlowingDecorator.h>

class IcePowerup : public Powerup {
public:
    IcePowerup(SDL_FRect position) : Powerup(position, "assets/tower/icePowerup.png") {}
    ~IcePowerup() override { Powerup::~Powerup(); }

    Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) override {
        return new SlowingDecorator(tower, towerPosition, "assets/tower/iceIndicator.png");
    }

    bool isClicked(float mouseX, float mouseY) const override {
        return Powerup::isClicked(mouseX, mouseY);
    }
};
