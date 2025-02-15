#ifndef PART1STATE_H
#define PART1STATE_H

#include "../GameState.h"
#include <ui/LTexture.h>

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
};
#endif