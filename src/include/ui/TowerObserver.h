/**
 * @file TowerObserver.h
 * @brief
 * @author Denmar Ermitano
 * @date 2025-02-29
 *
 * @details
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
	TowerObserver(int startingX, int startingY);
	~TowerObserver();
	
	void update(Observable* observable);
	void handleButtonEvents(SDL_Event* e);

	void setCurrentTower(Tower* tower);
	Tower* getCurrentTower();

	std::vector<DetailDisplayComponent*> getTowerComponents();
	bool initializeTowerComponents();
	void render();

	void addToBuyTowers(Tower* tower);

protected:
	std::vector<DetailDisplayComponent*> towerComponents; /** @brief vector of all components */
	std::vector<Tower*> buyTowers; /** @brief Towers used for showing values when buying */
	Tower* currentTower;


private:
	void updateAttributes();
	std::string formatValueStr(int currentValue, int upgradeValue, bool towerAtMaxLevel);
};