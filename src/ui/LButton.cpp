#include <ui/LButton.h>

/**
 * @class LButton
 * @brief Represents an interactive button in the UI.
 *
 * This class handles rendering a button, detecting mouse interactions,
 * changing button states, and displaying text.
 *
 * @author Nirav Patel
 */
LButton::LButton() :
	mPosition{ 0.f, 0.f },
	mCurrentSprite{ eButtonSpriteMouseOut } {
}

/**
 * @brief Sets the button's position.
 *
 * @param x The x-coordinate of the button.
 * @param y The y-coordinate of the button.
 */
void LButton::setPosition(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

/**
 * @brief Gets the current position of the button.
 *
 * @return The SDL_FPoint representing the button's position.
 */
SDL_FPoint LButton::getPosition() {
	return mPosition;
}

/**
 * @brief Handles mouse events for the button, changing its state accordingly.
 *
 * @param e Pointer to an SDL_Event containing mouse event data.
 */
void LButton::handleEvent(SDL_Event* e) {
	// If mouse event happened
	if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		// Get mouse position
		float x = -1.f, y = -1.f;
		SDL_GetMouseState(&x, &y);

		// Check if mouse is inside the button
		bool inside = true;

		// Mouse is outside the button
		if (x < mPosition.x || x > mPosition.x + kButtonWidth ||
			y < mPosition.y || y > mPosition.y + kButtonHeight) {
			inside = false;
		}

		// Update button state based on mouse position
		if (!inside) {
			mCurrentSprite = eButtonSpriteMouseOut;
		}
		else {
			switch (e->type) {
			case SDL_EVENT_MOUSE_MOTION:
				mCurrentSprite = eButtonSpriteMouseOverMotion;
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mCurrentSprite = eButtonSpriteMouseDown;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				mCurrentSprite = eButtonSpriteMouseUp;
				break;
			}
		}
	}
}

/**
 * @brief Sets the text on the button.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return True if the text texture was successfully created, false otherwise.
 */
bool LButton::setText(const std::string& text, SDL_Color textColor) {
	return gButtonSpriteTexture.loadFromRenderedText(text, textColor);
}

/**
 * @brief Renders the button on the screen.
 */
void LButton::render() {
	// Define sprite clips for different button states
	SDL_FRect spriteClips[] = {
		{ 0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 1 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight },
	};

	// Render the button using the current sprite state
	gButtonSpriteTexture.render(mPosition.x, mPosition.y, &spriteClips[mCurrentSprite]);
}

/**
 * @brief Checks if the button has been clicked.
 *
 * @return True if the button is clicked, false otherwise.
 */
bool LButton::isClicked() {
	// Get the current mouse position
	float x, y;
	SDL_GetMouseState(&x, &y);

	// Check if the mouse position is inside the button's area
	return (x >= mPosition.x && x <= mPosition.x + kButtonWidth &&
		y >= mPosition.y && y <= mPosition.y + kButtonHeight);
}
