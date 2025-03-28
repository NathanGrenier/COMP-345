/**
 * @file LButton.h
 * @brief The header file for the LButton class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <SDL3/SDL_events.h>
#include <string>
#include <ui/Texture.h>

class LButton {
public:
	float kButtonWidth;
	float kButtonHeight;

	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(float x, float y);

	SDL_FPoint getPosition() const;

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	bool setText(const std::string& text, SDL_Color textColor);
	bool loadFromFile(std::string path);

	//Shows button sprite
	void render();

	bool isClicked() const;

	void setSizeWithAspectRatio(float newWidth, float newHeight);

	enum eButtonSprite {
		eButtonSpriteMouseOut = 0,
		eButtonSpriteMouseOverMotion = 1,
		eButtonSpriteMouseDown = 2,
		eButtonSpriteMouseUp = 3,
		eButtonSpriteCount
	};
private:
	float originalWidth;
	float originalHeight;

	//Top left position
	SDL_FPoint mPosition;

	//Currently used global sprite
	eButtonSprite mCurrentSprite;

	Texture gButtonSpriteTexture;
};