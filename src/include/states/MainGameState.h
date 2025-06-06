/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"
#include <ui/LButton.h>
#include <ui/Texture.h>
#include <map/Map.h>
#include <string>
#include <critter/CritterGroup.h>
#include <towers/Tower.h>
#include <vector>
#include <ui/DetailAttributeDisplay.h>
#include <unordered_map>
#include <towers/TowerGroup.h>

class MainGameState : public GameState {
public:
	static const int STARTING_GOLD_AMT = 100;

	//Static accessor
	static MainGameState* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

	int STARTING_PLAYER_GOLD = 100;
	int playerGold;
	int waveLevel = 0;

	// Helper functions
	void renderText(const std::string& text, float x, float y);

	static bool optionsMenu;
private:
	//Static instance
	static MainGameState sMainGameState;

	//Intro background
	ParallaxBackground* bg;
	Texture mMessageTexture;

	LButton pauseButton;
	LButton optionsButton;
	LButton exitButton;
	LButton* currentButton;
	LButton playButton;

	bool endlessMode = false;
	bool isPaused;
	/** @brief Pointer to the map being edited
	 *  @details nullptr if no map is currently loaded
	 */
	Map* map = nullptr;

	CritterGroup* critterGroup = nullptr;
	TowerGroup* towerGroup = nullptr;

	DetailAttributeDisplay* detailDisplay; /** @brief ui display for details */
	int towerBuySelect = 0; /** @brief selected Tower as index */
};
