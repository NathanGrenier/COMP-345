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
#include <SDL3_mixer/SDL_mixer.h>

class LButton
{
public:
	float kButtonWidth;
	float kButtonHeight;

	// Initializes internal variables
	LButton();
	~LButton();

	// Sets top left position
	void setPosition(float x, float y);

	SDL_FPoint getPosition() const;

	// Handles mouse event
	void handleEvent(SDL_Event *e);

	bool setText(const std::string& text, SDL_Color textColor);
	bool loadFromFile(std::string path, std::string buttonPressPath = "", std::string buttonHoverPath = "", bool noSound = false);

	// Shows button sprite
	void render();
	void update();

	bool isClicked() const;

	void setSizeWithAspectRatio(float newWidth, float newHeight);

	enum eButtonSprite
	{
		eButtonSpriteMouseOut = 0,
		eButtonSpriteMouseOverMotion = 1,
		eButtonSpriteMouseDown = 2,
		eButtonSpriteMouseUp = 3,
		eButtonSpriteCount
	};

private:
	float originalWidth;
	float originalHeight;

	// Top left position
	SDL_FPoint mPosition;

	// Currently used global sprite
	eButtonSprite mCurrentSprite;

	Texture mButtonSpriteTexture;

	Mix_Chunk* mButtonPress{ nullptr };
	Mix_Chunk* mButtonHover{ nullptr };

	Mix_Chunk* currentSound = nullptr;
	Mix_Chunk* previousSound = nullptr;
	bool soundPlaying = false;
};