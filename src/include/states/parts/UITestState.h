/**
 * @file UITestState.h
 * @brief Header file for testing the UI for assignment 2
 * @author Denmar Ermitano
 * @date 2025-03-02
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