#pragma once
#include "LButton.h"
#include <SDL3/SDL_timer.h> // For time-based animations

class MainMenuButton : public LButton {
public:
    // Constructor
    MainMenuButton();

    static const int kButtonWidth{ 150 };
    static const int kButtonHeight{ 50 };

    // Handles mouse events and triggers scaling animation
    void handleEvent(SDL_Event* e);

    // Render with scaling effect
    void render() ;

    bool isClicked();

private:
    // Scale factor for the animation
    float mScale;

    // Target scale (1.0f normal, 1.2f when hovered)
    float mTargetScale;

    // Animation speed
    static constexpr float kAnimationSpeed = 5.0f;
};
