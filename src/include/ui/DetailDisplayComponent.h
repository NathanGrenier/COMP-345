/**
 * @file DetailDisplayComponent.h
 * @brief The header file for the DetailDisplayComponent class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/LTexture.h>

class DetailDisplayComponent {
public:
	static const int DETAIL_COMPONENT_PADDING = 10;
	static const int DETAIL_COMPONENT_SPACING = 50;
	static const int COMPONENT_BORDER_WIDTH = 2;
	static const int DETAIL_COMPONENT_HEIGHT = 15;

	DetailDisplayComponent();
	DetailDisplayComponent(int width);

	virtual void setComponentPosition(float x, float y);
	//SDL_FPoint getPosition();

	//void setWidth(int width);
	//int getWidth();

	//void setHeight(int height);
	//int getHeight();

	virtual void render();

protected:
	float xPosition;
	float yPosition;
	int width;
//	int height;
//
//	//Top left position
//	SDL_FPoint mPosition;
};

