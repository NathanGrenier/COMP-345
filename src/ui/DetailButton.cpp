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
    : DetailDisplayComponent::DetailDisplayComponent(), LButton::LButton()
{
    
}

/**
 * @brief Constructor for DetailButton with width and image path
 * @param width the width of the DetailButton
 * @param path the path of the LButton image
 */
DetailButton::DetailButton(int width, std::string path)
    : DetailDisplayComponent::DetailDisplayComponent(width), LButton::LButton()
{
    LButton::loadFromFile(path);
    LButton::setSizeWithAspectRatio(width, 0);
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
    LButton::setPosition(x, y);
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
    LButton::loadFromFile(path);
    LButton::setSizeWithAspectRatio(width, 0);
    LButton::setPosition(xPosition, yPosition);
}

/**
 * @brief Renders the button
 * @details Simply calls the LButton render() method
 * This allows the DetailButton to use render() without problem as a DetailDisplayComponent,
 * removing the need to specifyfrom which superclass render() is from
 */
void DetailButton::render()
{
    LButton::render();
}

