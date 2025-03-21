/**
 * @file TowerObserver.h
 * @brief The header file for the TowerObserver class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details Extends the Observer class.
 * Shows the details relating to a Tower class
 */

#pragma once

#include <util/Observer.h>
#include <vector>
#include <string>
#include <ui/DetailDisplayComponent.h>
#include <towers/Tower.h>

class Observable;

class TowerObserver : public Observer 
{
public:
	static const int TOWER_COMPONENT_COUNT = 13; /** @brief number of tower components */
	static const int STRATEGY_COUNT = 4; /** @brief number of tower strategies */
	// constructor and destructor
	TowerObserver(float startingX, float startingY);
	~TowerObserver();
	
	// Observer override
	void update(Observable* observable);
	void handleButtonEvents(SDL_Event& e);

	// Tower DetailDisplayComponent functions
	bool initializeTowerComponents();
	std::vector<DetailDisplayComponent*> getTowerComponents();
	void addToBuyTowers(Tower* tower);

	// accessor and mutator
	void setCurrentTower(Tower* tower);
	Tower* getCurrentTower();

	void render();

protected:
	std::vector<DetailDisplayComponent*> towerComponents; /** @brief vector of all components */
	std::vector<Tower*> buyTowers; /** @brief Towers used for showing values when buying */
	Tower* currentTower; /** @brief currently selected Tower object */

private:
	// helper functions
	void updateAttributes();
	void updateStrategyButton();
	std::string formatValueStr(int currentValue, int upgradeValue, bool towerAtMaxLevel);
	int towerStrategyIndex;
	std::string* towerStrategyPaths;
};