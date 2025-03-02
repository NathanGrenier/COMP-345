/**
 * @file DetailButton.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include "LButton.h"

class DetailButton : public DetailDisplayComponent, public LButton {
public:
	static const int DETAIL_BUTTON_HEIGHT = 50;

	DetailButton();
	DetailButton(int width);
	void setButtonPosition();

	void render();
};

