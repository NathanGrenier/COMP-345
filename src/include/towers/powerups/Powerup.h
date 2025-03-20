#pragma once

#include <SDL3/SDL.h>
#include <towers/Tower.h>
#include <Global.h>
#include <cmath>
#include <ui/LTexture.h>

// Base class for all powerups
class Powerup {
public:
    Powerup(SDL_FRect position, std::string powerupPath);
    virtual ~Powerup();

    // Check if the powerup is clicked
    virtual bool isClicked(float mouseX, float mouseY) const;

    // Apply the powerup to a tower
    virtual Tower* applyPowerupToTower(Tower* tower, SDL_FRect towerPosition) = 0;

    // Render the powerup on screen
    void render();

    // Update the powerup state (e.g., for frame cycling and bobbing)
    void update(float deltaTime);

    SDL_FRect position;
public:
    bool isDragged = false;
protected:
    LTexture powerupTexture;
    LTexture indicatorTexture;

    // Frame cycling variables
    int currentFrame;      // Current frame to display (0-3)
    float frameTime;       // Time accumulated for frame cycling
    float frameDuration;   // Duration per frame (1/60th of a second for 60 FPS)

    // Bobbing effect variables
    float bobbingSpeed;    // Speed of the bobbing movement
    float bobbingHeight;   // Height of the bobbing movement
    float bobbingOffset;   // Current vertical offset for bobbing
};
