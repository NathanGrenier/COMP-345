/**
 * @file Part3State.h
 * @brief The header file of part 3's (Critter) driver.
 * @author Nirav Patel
 * @date 2025-02-21
 */
#pragma once

#include <states/GameState.h>
#include <ui/LTexture.h>

class Part3State : public GameState {
public:
	//Static accessor
	static Part3State* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

	void renderText(const std::string& text, float x, float y);
private:
	//Static instance
	static Part3State sPart3State;

	//Private constructor
	Part3State();

	LTexture mBackgroundTexture;

	LTexture mMessageTexture;
	int playerCoins = 100;
};
