/**
 * @file UITestState.h
 * @brief Header file for the state for part 2 of assignment 1
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#ifndef UITESTSTATE_H
#define UITESTSTATE_H

#include <states/GameState.h>
#include <ui/LTexture.h>

class UITestState : public GameState {
public:
	//Static accessor
	static UITestState* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

private:
	//Static instance
	static UITestState sUITestState;

	//Private constructor
	UITestState();

};
#endif