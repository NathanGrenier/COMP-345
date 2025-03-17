/**
 * @class DetailDisplay
 * @brief Implementation of DetailDisplay class
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/DetailDisplay.h>
#include <Global.h>

 /**
  * @brief Default Constructor
  */
DetailDisplay::DetailDisplay() : mPosition{ 0.f, 0.f }, width(0), height(0) {

}

/**
 * @brief Constructor with width and height for the display
 * @param width the width of the display
 * @param height the height of the display
 */
DetailDisplay::DetailDisplay(int width, int height) : mPosition{ 0.f, 0.f }, width(width), height(height) {

}

/**
 * @brief Accessor for width
 * @return the width of the display
 */
int DetailDisplay::getWidth() {
	return width;
}

/**
 * @brief Mutator for width
 * @param width the new width
 */
void DetailDisplay::setWidth(int width) {
	DetailDisplay::width = width;
}

/**
 * @brief Accessor for height
 * @return the height of the display
 */
int DetailDisplay::getHeight() {
	return height;
}

/**
 * @brief Mutator for height
 * @param height the new height of the display
 */
void DetailDisplay::setHeight(int height) {
	DetailDisplay::height = height;
}

/**
 * @brief Sets the DetailDisplay's position.
 *
 * @param x The x-coordinate of the DetailDisplay.
 * @param y The y-coordinate of the DetailDisplay.
 */
void DetailDisplay::setPosition(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

/**
 * @brief Gets the current position of the DetailDisplay.
 *
 * @return The SDL_FPoint representing the DetailDisplay's position.
 */
SDL_FPoint DetailDisplay::getPosition() {
	return mPosition;
}

/**
 * @brief To render the DetailDisplay
 *
 * @details Draws the box for the DetailDisplay
 * Generates a rectangle with a border
 */
void DetailDisplay::render() {
	// draws border
	SDL_FRect borderRect = { mPosition.x, mPosition.y, static_cast<float>(width), static_cast<float>(height) };
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_RenderFillRect(gRenderer, &borderRect);

	// draws inside of the display
	SDL_FRect innerRect = {
		mPosition.x + DETAIL_BORDER_WIDTH,
		mPosition.y + DETAIL_BORDER_WIDTH,
		static_cast<float>(width - (2 * DETAIL_BORDER_WIDTH)),
		static_cast<float>(height - (2 * DETAIL_BORDER_WIDTH))
	};
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
	SDL_RenderFillRect(gRenderer, &innerRect);
}

