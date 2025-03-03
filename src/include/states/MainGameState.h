/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"
#include <ui/LTexture.h>
#include <map/Map.h>
#include <string>
#include <critter/CritterGroup.h>
#include <towers/Tower.h>
#include <vector>
#include <ui/DetailAttributeDisplay.h>
#include <unordered_map>

class MainGameState : public GameState {
public:
	//Static accessor
	static MainGameState* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

	int playerGold = 1000;
	int waveLevel = 0;

	// Helper functions
	void renderText(const std::string& text, float x, float y);

private:
	//Static instance
	static MainGameState sMainGameState;

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;

	/** @brief Pointer to the map being edited
	 *  @details nullptr if no map is currently loaded
	 */
	Map* map = nullptr;

	CritterGroup* critterGroup = nullptr;
	std::vector<Tower*> towers; /** @brief vector containing towers */

	DetailAttributeDisplay detailDisplay; /** @brief ui display for details */
	int towerBuySelect; /** @brief selected Tower as index */

	Tower* dummyStandardTower; /** @brief dummy StandardTower for details before buying Tower */
	Tower* dummyRapidFireTower; /** @brief dummy RapidFireTower for details before buying Tower */
	Tower* dummyCannonTower; /** @brief dummy CannonTower for details before buying Tower */

	const int STANDARD_TOWER_COST = 25; /** @brief gold cost for standard tower */
	const int CANNON_TOWER_COST = 100; /** @brief gold cost for cannon tower */
	const int RAPID_FIRE_TOWER_COST = 50; /** @brief gold cost for rapid fire tower */

	std::unordered_map<Map::Cell, bool> wallCellDict;

	Map::Cell targetCell;
};
