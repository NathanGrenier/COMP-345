#include <ui/DetailButton.h>
#include <Global.h>

DetailButton::DetailButton()
    : DetailDisplayComponent::DetailDisplayComponent(), LButton::LButton()
{
    
}

DetailButton::DetailButton(int width)
    : DetailDisplayComponent::DetailDisplayComponent(width, DETAIL_BUTTON_HEIGHT), LButton::LButton()
{
    
}

void DetailButton::setButtonPosition() 
{
    LButton::setPosition(DetailDisplayComponent::mPosition.x, DetailDisplayComponent::mPosition.y);
}

/**
 * @brief
 */
void DetailButton::render()
{
    setButtonPosition();

    DetailDisplayComponent::render();
    //LButton::render();

    int additionalTextLength = 100;

    SDL_FRect fRect = { 0.f, 0.f, width + additionalTextLength, height };
    gButtonSpriteTexture.render(DetailDisplayComponent::mPosition.x, DetailDisplayComponent::mPosition.y, &fRect, 0.0f, 25.0f);
}

