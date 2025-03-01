/**
 * @file LButton.h
 * @brief The header file for the LButton class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <SDL3/SDL_events.h>
#include <string>
#include "LTexture.h"
class LButton {
public:
	int kButtonWidth;
	int kButtonHeight;

	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(float x, float y);

	SDL_FPoint getPosition();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	bool setText(const std::string& text, SDL_Color textColor);
	bool loadFromFile(std::string path);

	//Shows button sprite
	void render();

	bool isClicked();

	void setSizeWithAspectRatio(int newWidth, int newHeight);

	enum eButtonSprite {
		eButtonSpriteMouseOut = 0,
		eButtonSpriteMouseOverMotion = 1,
		eButtonSpriteMouseDown = 2,
		eButtonSpriteMouseUp = 3,
		eButtonSpriteCount
	};
private:
	int originalWidth;
	int originalHeight;

	//Top left position
	SDL_FPoint mPosition;

	//Currently used global sprite
	eButtonSprite mCurrentSprite;

	LTexture gButtonSpriteTexture;
};