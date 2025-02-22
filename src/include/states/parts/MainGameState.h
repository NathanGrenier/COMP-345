/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

#include "../GameState.h"
#include <ui/LTexture.h>

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

private:
	//Static instance
	static MainGameState sMainGameState;

	//Private constructor
	MainGameState();

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;
};
#endif