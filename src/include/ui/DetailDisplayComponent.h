/**
 * @file DetailDisplayComponent.h
 * @brief The header file for the DetailDisplayComponent class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include "LTexture.h"

class DetailDisplayComponent {
public:
	static const int DETAIL_COMPONENT_PADDING = 4;
	static const int COMPONENT_BORDER_WIDTH = 2;

	DetailDisplayComponent();
	DetailDisplayComponent(int width, int height);

	void setPosition(float x, float y);
	SDL_FPoint getPosition();

	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	virtual void render() = 0;

protected:
	int width;
	int height;

	//Top left position
	SDL_FPoint mPosition;
};

