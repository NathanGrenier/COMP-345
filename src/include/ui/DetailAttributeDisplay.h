/**
 * @file DetailAttributeDisplay.h
 * @brief The header file for the DetailDisplay class.
 * @author Denmar Ermitano
 * @date 2025-02-16
 */
#pragma once

#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailDisplay.h>
#include <ui/DetailLabel.h>
#include <ui/DetailButton.h>
#include <ui/DetailAttribute.h>
#include <ui/TowerObserver.h>
#include <vector>

class DetailAttributeDisplay : public DetailDisplay {

public:
	static const int ATTRIBUTE_DISPLAY_WIDTH = 225;

	DetailAttributeDisplay();
	//~DetailAttributeDisplay();
	void render();

	bool initializeComponents();
	void selectTower(Tower* tower);
	bool isDisplayingTower();

	void handleButtonEvents(SDL_Event* e);

	TowerObserver* getTowerObserver();

	std::vector<DetailDisplayComponent*> getComponents();
	std::vector<DetailDisplayComponent*> getTowerComponents();

protected:
	std::vector<DetailDisplayComponent*> components; /** @brief vector of all components */

	TowerObserver* towerObserver;
	//CritterObserver critterObserver;
};

