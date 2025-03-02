#include <ui/DetailLabel.h>
#include <Global.h>

DetailLabel::DetailLabel()
	: DetailDisplayComponent::DetailDisplayComponent()
{

}

DetailLabel::DetailLabel(int width, std::string path)
	: DetailDisplayComponent::DetailDisplayComponent(width)
{
	LTexture::loadFromFile(path);
}

///**
// * @brief Sets the text on the DetailLabel.
// *
// * @param text The text to be displayed.
// * @param textColor The color of the text.
// * @return True if the text texture was successfully created, false otherwise.
// */
//bool DetailLabel::setText(const std::string& text, SDL_Color textColor) {
//	return detailLabelTexture.loadFromRenderedText(text, textColor);
//}

/**
 * @brief
 */
void DetailLabel::render()
{
	LTexture::render(xPosition, yPosition, nullptr, width, 0);
	
	//DetailDisplayComponent::render();
	//int additionalTextLength = 100;

	//SDL_FRect fRect = { 0.f, 0.f, width + additionalTextLength, height };
	//detailLabelTexture.render(mPosition.x, mPosition.y, &fRect, 0.0f, 20.0f);
}

