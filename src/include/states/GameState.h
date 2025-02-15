#pragma once
#include <SDL3/SDL_events.h> 
class GameState {
public:
	//State transitions
	virtual bool enter() = 0;
	virtual bool exit() = 0;

	//Main loop functions
	virtual void handleEvent(SDL_Event& e) = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	//Make sure to call child destructor
	virtual ~GameState() = default;
};