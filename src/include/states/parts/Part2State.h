/**
 * @file Part2State.h
 * @brief Header file for the state for part 2 of assignment 1
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#ifndef PART2STATE_H
#define PART2STATE_H

#include <states/GameState.h>
#include <ui/LTexture.h>

class Part2State : public GameState {
public:
	//Static accessor
	static Part2State* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

private:
	//Static instance
	static Part2State sPart2State;

	//Private constructor
	Part2State();

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;
};
#endif