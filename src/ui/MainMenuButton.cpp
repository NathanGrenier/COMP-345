#include <ui/MainMenuButton.h>
#include <Global.h>

MainMenuButton::MainMenuButton()
    : mScale(1.0f), mTargetScale(1.0f) {
}

void MainMenuButton::handleEvent(SDL_Event* e) {

    // Handle mouse events, similar to LButton
    if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        // Get mouse position
        float x = -1.f, y = -1.f;
        SDL_GetMouseState(&x, &y);

        // Check if the mouse is inside the button
        bool inside = true;

        // Use mPosition for MainMenuButton
        SDL_FPoint mPosition = this->mPosition;

        if (x < mPosition.x || x > mPosition.x + kButtonWidth * mScale ||  // Use scaled width
            y < mPosition.y || y > mPosition.y + kButtonHeight * mScale) { // Use scaled height
            inside = false;
        }

        // Update the target scale based on whether the mouse is inside the button
        if (inside) {
            switch (e->type) {
            case SDL_EVENT_MOUSE_MOTION:
                mTargetScale = 1.2f;  // Scale up when mouse moves over button
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mTargetScale = 1.1f;  // Slightly shrink on click
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mTargetScale = 1.2f;  // Scale back up after click
                break;
            }
        }
        else {
            mTargetScale = 1.0f;  // Return to normal scale when mouse is outside
        }
    }
}

void MainMenuButton::render() {
    // Interpolate the scale between current and target scale
    if (mScale < mTargetScale) {
        mScale += kAnimationSpeed * SDL_GetTicks() / 1000.0f;
        if (mScale > mTargetScale) {
            mScale = mTargetScale;  // Clamp to target scale
        }
    }
    else if (mScale > mTargetScale) {
        mScale -= kAnimationSpeed * SDL_GetTicks() / 1000.0f;
        if (mScale < mTargetScale) {
            mScale = mTargetScale;  // Clamp to target scale
        }
    }

    // Calculate the centered position based on the scaled size
    float centerX = (Global::kScreenWidth - (kButtonWidth * mScale)) / 2;

    // Create the destRect with the correct scaled size
    SDL_FRect destRect = { centerX, mPosition.y, kButtonWidth * mScale, kButtonHeight * mScale };

    // Define sprite clips (assuming you have sprites for different button states)
    SDL_FRect spriteClips[] = {
        { 0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 1 * destRect.h, destRect.w, destRect.h },
        { 0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight },
    };

    // Render the button with the destRect for the position and size
    gButtonSpriteTexture.render(destRect.x, mPosition.y, nullptr,
        destRect.w, NULL, 0, nullptr, SDL_FLIP_NONE);
}


bool MainMenuButton::isClicked() {
    // Get the current mouse position
    float x, y;
    SDL_GetMouseState(&x, &y);

    // Use mPosition for MainMenuButton and scaled area for click detection
    SDL_FPoint mPosition = this->mPosition;

    // Check if the mouse position is inside the button's scaled area
    if (x >= mPosition.x && x <= mPosition.x + kButtonWidth * mScale &&
        y >= mPosition.y && y <= mPosition.y + kButtonHeight * mScale) {
        return true;
    }
    return false;
}

