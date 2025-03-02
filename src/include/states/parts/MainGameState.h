/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "../GameState.h"
#include <ui/LTexture.h>
#include <map/Map.h>
#include <string>
#include <critter/CritterGroup.h>

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

	int playerGold;
	int waveLevel;
	std::string warningMessage = "";
	float warningTime = 0.0f;

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
};
