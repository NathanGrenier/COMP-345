/**
 * @class DetailDisplayComponent
 * @brief Implementation of the DetailDisplayComponent class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */
#include <ui/DetailDisplayComponent.h>
#include <Global.h>

 const float DetailDisplayComponent::DETAIL_COMPONENT_PADDING = 8.0f;
 const float DetailDisplayComponent::DETAIL_COMPONENT_SPACING = 40.0f; 
 const float DetailDisplayComponent::TOWER_COMPONENT_SPACING = 20.0f;
 const float DetailDisplayComponent::CRITTER_COMPONENT_SPACING = 20.0f;
 const float DetailDisplayComponent::COMPONENT_BORDER_WIDTH = 2.0f;
 const float DetailDisplayComponent::DETAIL_COMPONENT_HEIGHT = 15.0f;

/**
 * @brief Default constructor.
 */
DetailDisplayComponent::DetailDisplayComponent()
	: width(0), xPosition(0), yPosition(0) {

}

/**
 * @brief Constructor containing width
 * @param width the width of a DetailDisplayComponent
 *
 * @details Width is used for scaling.
 * Generally, components are scaled with aspect ratio kept constant.
 * Having a width locks the height based on the aspect ratio of the DetailDisplayComponent
 */
DetailDisplayComponent::DetailDisplayComponent(float width)
    : width(width), xPosition(0), yPosition(0)
{

}

/**
 * @brief Sets the DetailDisplayComponent's position.
 *
 * @param x The x-coordinate of the DetailDisplayComponent.
 * @param y The y-coordinate of the DetailDisplayComponent.
 */
void DetailDisplayComponent::setComponentPosition(float x, float y) {
	xPosition = x;
	yPosition = y;
}

/**
 * @brief Accessor for the DetailDisplayComponent's horizontal position.
 *
 * @return The x-coordinate of the DetailDisplayComponent.
 */
float DetailDisplayComponent::getComponentXPosition()
{
    return xPosition;
}

/**
 * @brief Accessor for the DetailDisplayComponent's vertical position.
 *
 * @return The y-coordinate of the DetailDisplayComponent.
 */
float DetailDisplayComponent::getComponentYPosition()
{
    return yPosition;
}

/**
 * @brief Draws a box for a DetailDisplayComponent
 *
 * @details Rectangle is based on width and default height.
 * Generally, this method is overridden when the DetailDisplayComponent instead uses an image (like in the case of buttons)
 */
void DetailDisplayComponent::render() 
{
    // draws rectangle for border
    SDL_FRect borderRect = { xPosition, yPosition, static_cast<float>(width), static_cast<float>(DETAIL_COMPONENT_HEIGHT) };
    SDL_SetRenderDrawColor(gRenderer, 0x77, 0x77, 0x77, 0xFF);
    SDL_RenderFillRect(gRenderer, &borderRect);

    // draws inner rectangle
    SDL_FRect innerRect = {
        xPosition + COMPONENT_BORDER_WIDTH,
        yPosition + COMPONENT_BORDER_WIDTH,
        width - (2 * COMPONENT_BORDER_WIDTH),
        static_cast<float>(DETAIL_COMPONENT_HEIGHT - (2 * COMPONENT_BORDER_WIDTH))
    };
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &innerRect);
}

