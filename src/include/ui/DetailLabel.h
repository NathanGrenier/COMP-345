/**
 * @file DetailLabel.h
 * @brief The header file for the DetailLabel class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>

class DetailLabel : public DetailDisplayComponent 
{
public:
	static const int DETAIL_LABEL_HEIGHT = 30;

	DetailLabel();
	DetailLabel(int width);

	bool setText(const std::string& text, SDL_Color textColor);
	void render();

protected:
	LTexture detailLabelTexture;
};

