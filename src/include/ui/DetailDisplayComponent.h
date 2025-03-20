/**
 * @file DetailDisplayComponent.h
 * @brief The header file for the DetailDisplayComponent class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 * 
 * @details Wrapper class for the UI elements in a DetailDisplay
 * Wraps up buttons, labels, and aggregate UI (DetailAttribute has 2 labels for instance)
 * Allows for DetailDisplayComponent objects to be placed within a single array/vector
 */
#pragma once

#include <Global.h>
#include <ui/LTexture.h>

class DetailDisplayComponent {
public:
	static const int DETAIL_COMPONENT_PADDING = 8; /** @brief padding for components, used on sides */
	static const int DETAIL_COMPONENT_SPACING = 40; /** @brief spacing for components, used vertically */
	static const int TOWER_COMPONENT_SPACING = 20; /** @brief spacing for components, used vertically */
	static const int CRITTER_COMPONENT_SPACING = 20; /** @brief spacing for components, used vertically */
	static const int COMPONENT_BORDER_WIDTH = 2;    /** @brief width of the border around a component */
	static const int DETAIL_COMPONENT_HEIGHT = 15;  /** @brief default height of a component */

	// constructors
	DetailDisplayComponent();
	DetailDisplayComponent(int width);

	virtual void setComponentPosition(float x, float y);
	float getComponentXPosition();
	float getComponentYPosition();

	virtual void render();

protected:
	float xPosition; /** @brief x position of the component */
	float yPosition; /** @brief y position of the component */
	int width; /** @brief width of the component, used for scaling */
};

