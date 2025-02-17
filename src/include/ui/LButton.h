#pragma once
#include <SDL3/SDL_events.h>
#include <string>
#include "LTexture.h"
class LButton {
public:
	//Button dimensions
	static const int kButtonWidth = 200;
	static const int kButtonHeight = 100;

	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(float x, float y);

	SDL_FPoint getPosition();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	bool setText(const std::string& text, SDL_Color textColor);

	//Shows button sprite
	void render();

	bool isClicked();

protected:
	enum eButtonSprite {
		eButtonSpriteMouseOut = 0,
		eButtonSpriteMouseOverMotion = 1,
		eButtonSpriteMouseDown = 2,
		eButtonSpriteMouseUp = 3
	};

	//Top left position
	SDL_FPoint mPosition;

	//Currently used global sprite
	eButtonSprite mCurrentSprite;

	LTexture gButtonSpriteTexture;
};