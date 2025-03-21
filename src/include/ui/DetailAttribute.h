/**
 * @file DetailAttribute.h
 * @brief The header file for the DetailAttribute class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 * 
 * @details DetailDisplayComponent superclass.
 * Used for displaying an attribute. 
 * Consists of a label and a value.
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>

class DetailAttribute : public DetailDisplayComponent
{
public:
	static const float DETAIL_ATTRIBUTE_SPACING; /**< @brief padding for detail components, used on sides */
	static const float DETAIL_VALUE_PADDING;	/**< @brief spacing for detail components, used vertically */

	// constructors
	DetailAttribute();
	DetailAttribute(float width);

	bool setAttributeText(const std::string& text, SDL_Color textColor);
	bool setValueText(const std::string& text, SDL_Color textColor);
	void render();

protected:
	LTexture detailAttributeTexture; /**< @brief LTexture for labelling an attribute */
	LTexture detailValueTexture; /**< @brief LTexture for the value an attribute */
};

