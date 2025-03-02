#include <ui/DetailDisplayComponent.h>
#include <Global.h>

DetailDisplayComponent::DetailDisplayComponent()
    : width(0), height(0), mPosition{ 0.f, 0.f }
{
    
}


DetailDisplayComponent::DetailDisplayComponent(int width, int height)
    : width(width), height(height), mPosition{ 0.f, 0.f }
{

}

int DetailDisplayComponent::getWidth()
{
    return width;
}

void DetailDisplayComponent::setWidth(int width)
{
    DetailDisplayComponent::width = width;
}

int DetailDisplayComponent::getHeight()
{
    return height;
}

void DetailDisplayComponent::setHeight(int height)
{
    DetailDisplayComponent::height = height;
}

/**
 * @brief Sets the DetailDisplayComponent's position.
 *
 * @param x The x-coordinate of the DetailDisplayComponent.
 * @param y The y-coordinate of the DetailDisplayComponent.
 */
void DetailDisplayComponent::setPosition(float x, float y)
{
    mPosition.x = x;
    mPosition.y = y;
}

/**
 * @brief Gets the current position of the DetailDisplayComponent.
 *
 * @return The SDL_FPoint representing the DetailDisplayComponent's position.
 */
SDL_FPoint DetailDisplayComponent::getPosition()
{
    return mPosition;
}

void DetailDisplayComponent::render() 
{
    SDL_FRect borderRect = { mPosition.x, mPosition.y, width, height };
    SDL_SetRenderDrawColor(gRenderer, 0x77, 0x77, 0x77, 0xFF);

    SDL_RenderFillRect(gRenderer, &borderRect);

    SDL_FRect innerRect = {
        mPosition.x + COMPONENT_BORDER_WIDTH,
        mPosition.y + COMPONENT_BORDER_WIDTH,
        width - (2 * COMPONENT_BORDER_WIDTH),
        height - (2 * COMPONENT_BORDER_WIDTH)
    };

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderFillRect(gRenderer, &innerRect);
}

