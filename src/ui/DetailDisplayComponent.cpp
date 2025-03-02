#include <ui/DetailDisplayComponent.h>
#include <Global.h>

DetailDisplayComponent::DetailDisplayComponent()
    : width(0), xPosition(0), yPosition(0)
{
    
}


DetailDisplayComponent::DetailDisplayComponent(int width)
    : width(width), xPosition(0), yPosition(0)
{

}

//int DetailDisplayComponent::getWidth()
//{
//    return width;
//}
//
//void DetailDisplayComponent::setWidth(int width)
//{
//    DetailDisplayComponent::width = width;
//}
//
//int DetailDisplayComponent::getHeight()
//{
//    return height;
//}
//
//void DetailDisplayComponent::setHeight(int height)
//{
//    DetailDisplayComponent::height = height;
//}
//
/**
 * @brief Sets the DetailDisplayComponent's position.
 *
 * @param x The x-coordinate of the DetailDisplayComponent.
 * @param y The y-coordinate of the DetailDisplayComponent.
 */
void DetailDisplayComponent::setComponentPosition(float x, float y)
{
    xPosition = x;
    yPosition = y;
}

//void DetailDisplayComponent::render()
//{
//    return;
//}

//
///**
// * @brief Gets the current position of the DetailDisplayComponent.
// *
// * @return The SDL_FPoint representing the DetailDisplayComponent's position.
// */
//SDL_FPoint DetailDisplayComponent::getPosition()
//{
//    return mPosition;
//}
//

void DetailDisplayComponent::render() 
{
    SDL_FRect borderRect = { xPosition, yPosition, width, DETAIL_COMPONENT_HEIGHT };
    SDL_SetRenderDrawColor(gRenderer, 0x77, 0x77, 0x77, 0xFF);

    SDL_RenderFillRect(gRenderer, &borderRect);

    SDL_FRect innerRect = {
        xPosition + COMPONENT_BORDER_WIDTH,
        yPosition + COMPONENT_BORDER_WIDTH,
        width - (2 * COMPONENT_BORDER_WIDTH),
        DETAIL_COMPONENT_HEIGHT - (2 * COMPONENT_BORDER_WIDTH)
    };

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderFillRect(gRenderer, &innerRect);
}

