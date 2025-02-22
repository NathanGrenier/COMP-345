#include <ui/TowerButton.h>
#include <Global.h>

/**
 * @brief Default Constructor
 */
TowerButton::TowerButton()
{

}

/**
 * @brief Generates TowerButton
 * 
 * @details Uses default button sizes to create the button
 * Draws the rectangle using SDL 
 */
void TowerButton::render() {
    //Define sprites
    SDL_FRect spriteClips[] = {
        { 0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 1 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight },
    };

    //Show current button sprite
    gButtonSpriteTexture.render(mPosition.x, mPosition.y, &spriteClips[mCurrentSprite]);
}
