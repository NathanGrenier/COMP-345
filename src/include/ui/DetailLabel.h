/**
 * @file DetailLabel.h
 * @brief The header file for the DetailLabel class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details DetailDisplayComponent and LTexture superclass.
 * Used for displaying a label.
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <string>

class DetailLabel : public DetailDisplayComponent, public LTexture
{
public:
	// constructors
	DetailLabel();
	DetailLabel(int width, std::string path);

	void render();
private:
};

