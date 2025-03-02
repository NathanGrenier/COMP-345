/**
 * @file DetailButton.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>
#include <ui/LButton.h>

class DetailButton : public DetailDisplayComponent, public LButton {
public:
	DetailButton();
	DetailButton(int width, std::string path);

	void setComponentPosition(float x, float y);

	void render();
};

