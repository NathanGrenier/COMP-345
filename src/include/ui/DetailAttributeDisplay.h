/**
 * @file DetailAttributeDisplay.h
 * @brief The header file for the DetailAttributeDisplay class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details Renders the UI on the right side
 * Displays buttons for buying Towers
 * Displays attributes of a selected Tower
 * Displays buttons for upgrading and selling a selected Tower
 */
#pragma once


#include <Global.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailDisplay.h>
#include <ui/TowerObserver.h>
#include <vector>

class CritterObserver;

class DetailAttributeDisplay : public DetailDisplay {

public:
	static const int ATTRIBUTE_DISPLAY_WIDTH = Global::viewerWidth; /** @brief width of the display */

	// constructor
	DetailAttributeDisplay();
	void addToBuyTowers(Tower* tower);

	// methods for towerObserver
	bool initializeComponents();
	void selectTower(Tower* tower);
	bool isDisplayingTower();		// also needs function for displaying critter
	//bool isDisplayingCritter();

	// for button hovers, clicks
	void handleButtonEvents(SDL_Event& e);

	// accessors
	TowerObserver* getTowerObserver();
	std::vector<DetailDisplayComponent*> getComponents();
	std::vector<DetailDisplayComponent*> getTowerComponents();

	// Critter related methods
	std::vector<DetailDisplayComponent*> getCritterComponents();
	CritterObserver* getCritterObserver();
	void selectCritter(Critter* critter);
	bool isDisplayingCritter();

	void render();

protected:
	std::vector<DetailDisplayComponent*> components; /** @brief vector of all components */

	CritterObserver* critterObserver;
	TowerObserver* towerObserver;

	std::vector<DetailDisplayComponent*> critterComponents;
};
