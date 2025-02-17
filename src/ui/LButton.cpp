#include <ui/LButton.h>

//LButton Implementation
LButton::LButton() :
	mPosition{ 0.f, 0.f },
	mCurrentSprite{ eButtonSpriteMouseOut } {

}

void LButton::setPosition(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

SDL_FPoint LButton::getPosition() {
	return mPosition;
}

void LButton::handleEvent(SDL_Event* e) {
	//If mouse event happened
	if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		//Get mouse position
		float x = -1.f, y = -1.f;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + kButtonWidth)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + kButtonHeight)
		{
			inside = false;
		}
		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = eButtonSpriteMouseOut;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
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

bool LButton::setText(const std::string& text, SDL_Color textColor) {
	return gButtonSpriteTexture.loadFromRenderedText(text, textColor);
}

void LButton::render() {
	//Define sprites
	SDL_FRect spriteClips[] = {
		{ 0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 1 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight },
		{ 0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight },
	};

	//Show current button sprite
	gButtonSpriteTexture.render(mPosition.x, mPosition.y, &spriteClips[mCurrentSprite]);
}

bool LButton::isClicked() {
	// Get the current mouse position
	float x, y;
	SDL_GetMouseState(&x, &y);

	// Check if the mouse position is inside the button's area
	if (x >= mPosition.x && x <= mPosition.x + kButtonWidth &&
		y >= mPosition.y && y <= mPosition.y + kButtonHeight)
	{
		return true;
	}
	return false;
}
