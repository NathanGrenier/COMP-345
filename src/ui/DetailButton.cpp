/**
 * @class DetailButton
 * @brief Implementation of the DetailButton class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/DetailButton.h>
#include <Global.h>
#include <string>

/**
 * @brief Default constructor.
 */
DetailButton::DetailButton()
    : DetailDisplayComponent::DetailDisplayComponent()
{
    
}

/**
 * @brief Constructor for DetailButton with width and image path
 * @param width the width of the DetailButton
 * @param path the path of the LButton image
 */
DetailButton::DetailButton(float width, std::string path)
    : DetailDisplayComponent::DetailDisplayComponent(width)
{
    buttonTexture.loadFromFile(path);
    buttonTexture.setSizeWithAspectRatio(width, 0.f);
}

/**
 * @brief Sets the DetailButton's position.
 *
 * @details sets both superclasses positions:
 * xPosition and yPosition from DetailDisplayComponent,
 * mPosition from LButton
 * @param x The x-coordinate of the DetailButton.
 * @param y The y-coordinate of the DetailButton.
 */
void DetailButton::setComponentPosition(float x, float y) 
{
    buttonTexture.setPosition(x, y);
    xPosition = x;
    yPosition = y;
}

/**
 * @brief Sets the DetailButton's path.
 *
 * @details sets the image path of a DetailButton, usually to change its text.
 * Assumes that position has already been set previously.
 * @param path the path of the LButton image
 */
void DetailButton::setComponentImagePath(std::string path)
{
    buttonTexture.loadFromFile(path);
    buttonTexture.setSizeWithAspectRatio(width, 0);
    buttonTexture.setPosition(xPosition, yPosition);
}

/**
 * @brief Renders the button
 * @details Simply calls the LButton render() method
 * This allows the DetailButton to use render() without problem as a DetailDisplayComponent,
 * removing the need to specifyfrom which superclass render() is from
 */
void DetailButton::render()
{
    buttonTexture.render();
}

bool DetailButton::isClicked()
{
    return buttonTexture.isClicked();
}

void DetailButton::handleEvent(SDL_Event* e)
{
    return buttonTexture.handleEvent(e);
}