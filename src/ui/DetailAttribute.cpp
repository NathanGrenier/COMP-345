/**
 * @class DetailAttribute
 * @brief Implementation of the DetailAttribute class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/DetailAttribute.h>
#include <Global.h>

const float DetailAttribute::DETAIL_ATTRIBUTE_SPACING = 20; /** @brief padding for detail components, used on sides */
const float DetailAttribute::DETAIL_VALUE_PADDING = 120;

/**
 * @brief Default constructor.
 */
DetailAttribute::DetailAttribute()
	: DetailDisplayComponent::DetailDisplayComponent()
{

}

/**
 * @brief Constructor with width
 * @param width the width of a DetailAttribute
 */
DetailAttribute::DetailAttribute(float width)
	: DetailDisplayComponent::DetailDisplayComponent(width)
{

}

/**
 * @brief Sets the text of the attribute label on the DetailAttribute.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return true if the text texture was successfully created
 * @return false could not set the text
 */
bool DetailAttribute::setAttributeText(const std::string& text, SDL_Color textColor) {
	return detailAttributeTexture.loadFromRenderedText(text, textColor);
}

/**
 * @brief Sets the text of the value on the DetailAttribute.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return true if the text texture was successfully created
 * @return false could not set the text
 */
bool DetailAttribute::setValueText(const std::string& text, SDL_Color textColor) {
	return detailValueTexture.loadFromRenderedText(text, textColor);
}

/**
 * @brief Displays the DetailAttribute
 * @details Draws the rectangle from the DetailDisplayComponent base class,
 * renders both the label and value text
 */
void DetailAttribute::render()
{
	// uses the default rectangle in DetailDisplayComponent
	DetailDisplayComponent::render();

	// renders text
	float verticalPadding = 3.0f;
	float horizontalPadding = 5.0f;
	detailAttributeTexture.render(xPosition + horizontalPadding, yPosition + verticalPadding, nullptr, 0, DetailDisplayComponent::DETAIL_COMPONENT_HEIGHT * 0.7);
	detailValueTexture.render(xPosition + DETAIL_VALUE_PADDING, yPosition + verticalPadding, nullptr, 0, DetailDisplayComponent::DETAIL_COMPONENT_HEIGHT * 0.7);
}

