/**
 * @file Part1State.h
 * @brief The header file of part 1's (Map) driver.
 * @author Nathan Grenier
 * @date 2025-02-15
 */
#pragma once

#include <states/GameState.h>
#include <ui/LTexture.h>
#include <map/Map.h>

class Part1State : public GameState {
public:
	//Static accessor
	static Part1State* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

private:
	//Static instance
	static Part1State sPart1State;

	//Private constructor
	Part1State();

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;

	int mouseDownStatus = 0;
	int keyDownStatus = 0;

	/** @brief Pointer to the map being edited
	 *  @details nullptr if no map is currently loaded
	 */
	Map* map = nullptr;
};