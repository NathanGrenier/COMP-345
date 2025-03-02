#include <ui/DetailAttribute.h>
#include <Global.h>

DetailAttribute::DetailAttribute()
	: DetailDisplayComponent::DetailDisplayComponent()
{

}

DetailAttribute::DetailAttribute(int width)
	: DetailDisplayComponent::DetailDisplayComponent(width)
{

}

/**
 * @brief Sets the text on the DetailAttribute.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return True if the text texture was successfully created, false otherwise.
 */
bool DetailAttribute::setAttributeText(const std::string& text, SDL_Color textColor) {
	return detailAttributeTexture.loadFromRenderedText(text, textColor);
}

/**
 * @brief Sets the text on the DetailAttribute.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return True if the text texture was successfully created, false otherwise.
 */
bool DetailAttribute::setValueText(const std::string& text, SDL_Color textColor) {
	return detailValueTexture.loadFromRenderedText(text, textColor);
}

/**
 * @brief
 */
void DetailAttribute::render()
{
	DetailDisplayComponent::render();

	//SDL_FRect attributeRect = { 0.f, 0.f, width, height };
	//SDL_FRect valueRect = { 0.f, 0.f, width, height };

	detailAttributeTexture.render(xPosition, yPosition, nullptr, 0, DetailDisplayComponent::DETAIL_COMPONENT_HEIGHT);
	detailValueTexture.render(xPosition + DETAIL_VALUE_PADDING, yPosition, nullptr, 0, DetailDisplayComponent::DETAIL_COMPONENT_HEIGHT);
}

