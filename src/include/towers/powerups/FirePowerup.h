#pragma once

#include <towers/powerups/Powerup.h>
#include <towers/Tower.h>
#include <towers/decorators/BurningDecorator.h>

class FirePowerup : public Powerup {
public:
    FirePowerup(SDL_FRect position) : Powerup(position, "assets/tower/firePowerup.png") {}
    ~FirePowerup() override {
        Powerup::~Powerup();
    }

    Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) override {
        return new BurningDecorator(tower, towerPosition, "assets/tower/fireIndicator.png");
    }

    bool isClicked(float mouseX, float mouseY) const override {
        return Powerup::isClicked(mouseX, mouseY);
    }
};
