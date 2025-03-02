/**
 * @file DetailDisplay.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 * 
 * @details Parent class for a UI group element
 * Generates a rectangular box for inserting elements inside
 */
#pragma once

#include <Global.h>

class DetailDisplay {
public:
	static const int DETAIL_BORDER_WIDTH = 4; /** @brief width of the disply border */

	// constructors
	DetailDisplay();
	DetailDisplay(int width, int height);

	// accessors and mutators
	void setPosition(float x, float y);
	SDL_FPoint getPosition();

	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	// shows rectangle box
	virtual void render();

protected:
	int width;
	int height;

	SDL_FPoint mPosition; /** @brief top left position of the display */
};

