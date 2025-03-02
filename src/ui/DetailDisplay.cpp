#include <ui/DetailDisplay.h>
#include <Global.h>

DetailDisplay::DetailDisplay() : mPosition{ 0.f, 0.f }, width(0), height(0)
{

}

/**
 * @class DetailDisplay
 * @brief 
 *
 * @author Denmar Ermitano
 */
DetailDisplay::DetailDisplay(int width, int height) : mPosition { 0.f, 0.f }, width(width), height(height)
{

}

int DetailDisplay::getWidth()
{
    return width;
}

void DetailDisplay::setWidth(int width)
{
    DetailDisplay::width = width;
}

int DetailDisplay::getHeight()
{
    return height;
}

void DetailDisplay::setHeight(int height)
{
    DetailDisplay::height = height;
}

/**
 * @brief Sets the DetailDisplay's position.
 *
 * @param x The x-coordinate of the DetailDisplay.
 * @param y The y-coordinate of the DetailDisplay.
 */
void DetailDisplay::setPosition(float x, float y) 
{
    mPosition.x = x;
    mPosition.y = y;
}

/**
 * @brief Gets the current position of the DetailDisplay.
 *
 * @return The SDL_FPoint representing the DetailDisplay's position.
 */
SDL_FPoint DetailDisplay::getPosition() 
{
    return mPosition;
}

/**
 * @brief 
 */
void DetailDisplay::render() 
{
    SDL_FRect borderRect = { mPosition.x, mPosition.y, width, height };
    SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);

    SDL_RenderFillRect(gRenderer, &borderRect);

    SDL_FRect innerRect = {
        mPosition.x + DETAIL_BORDER_WIDTH,
        mPosition.y + DETAIL_BORDER_WIDTH,
        width - (2 * DETAIL_BORDER_WIDTH),
        height - (2 * DETAIL_BORDER_WIDTH)
    };
    SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);

    SDL_RenderFillRect(gRenderer, &innerRect);
}

