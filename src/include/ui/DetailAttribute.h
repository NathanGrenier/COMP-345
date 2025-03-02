/**
 * @file DetailAttribute.h
 * @brief The header file for the DetailAttribute class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>

class DetailAttribute : public DetailDisplayComponent
{
public:
	static const int DETAIL_ATTRIBUTE_SPACING = 20;
	static const int DETAIL_VALUE_PADDING = 120;

	DetailAttribute();
	DetailAttribute(int width);

	bool setAttributeText(const std::string& text, SDL_Color textColor);
	bool setValueText(const std::string& text, SDL_Color textColor);
	void render();

protected:
	LTexture detailAttributeTexture;
	LTexture detailValueTexture;
};

