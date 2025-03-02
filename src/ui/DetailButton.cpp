#include <ui/DetailButton.h>
#include <Global.h>
#include <string>

DetailButton::DetailButton()
    : DetailDisplayComponent::DetailDisplayComponent(), LButton::LButton()
{
    
}

DetailButton::DetailButton(int width, std::string path)
    : DetailDisplayComponent::DetailDisplayComponent(width), LButton::LButton()
{
    LButton::loadFromFile(path);
    LButton::setSizeWithAspectRatio(width, 0);
}

void DetailButton::setComponentPosition(float x, float y) 
{
    LButton::setPosition(x, y);
    xPosition = x;
    yPosition = y;
}

/**
 * @brief
 */
void DetailButton::render()
{
    LButton::render();
}

///**
// * @brief
// */
//void DetailButton::render()
//{
//    setButtonPosition();
//
//    DetailDisplayComponent::render();
//    //LButton::render();
//
//    int additionalTextLength = 100;
//
//    SDL_FRect fRect = { 0.f, 0.f, width + additionalTextLength, height };
//    gButtonSpriteTexture.render(DetailDisplayComponent::mPosition.x, DetailDisplayComponent::mPosition.y, &fRect, 0.0f, 25.0f);
//}

