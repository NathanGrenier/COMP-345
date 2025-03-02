/**
 * @file DetailDisplay.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>

class DetailDisplay {
public:
	//Display border width
	static const int DETAIL_BORDER_WIDTH = 4;

	DetailDisplay();

	//Initializes internal variables
	DetailDisplay(int width, int height);

	//Sets top left position
	void setPosition(float x, float y);
	SDL_FPoint getPosition();

	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	//Shows button sprite
	virtual void render();

protected:
	int width;
	int height;

	//Top left position
	SDL_FPoint mPosition;
};

