/**
 * @file DetailButton.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details DetailDisplayComponent and LButton superclass.
 * Used for displaying a button.
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>
#include <ui/LButton.h>

class DetailButton : public DetailDisplayComponent, public LButton {
public:
	// constructors
	DetailButton();
	DetailButton(int width, std::string path);

	void setComponentPosition(float x, float y);

	void render();
};

