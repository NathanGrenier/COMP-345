/**
 * @class DetailLabel
 * @brief Implementation of the DetailLabel class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/DetailLabel.h>
#include <Global.h>

/**
 * @brief Default constructor.
 */
DetailLabel::DetailLabel()
	: DetailDisplayComponent::DetailDisplayComponent()
{

}

/**
 * @brief Constructor for DetailLabel with width and image path
 * @param width the width of the DetailLabel
 * @param path the path of the LTexture image
 */
DetailLabel::DetailLabel(int width, std::string path)
	: DetailDisplayComponent::DetailDisplayComponent(width)
{
	LTexture::loadFromFile(path);
}

/**
 * @brief Renders the DetailLabel
 */
void DetailLabel::render()
{
	// sets height to 0, leaving height to be calculated with width and aspect ratio of image
	LTexture::render(xPosition, yPosition, nullptr, width, 0);
}

