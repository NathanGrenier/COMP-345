#include <SDL3/SDL_events.h>
#include <states/ExitState.h>
#include <SDL3/SDL_init.h>

ExitState ExitState::sExitState;

//Hollow exit state
ExitState* ExitState::get() {
	return &sExitState;
}

bool ExitState::enter() {
	SDL_Quit();
	return true;
}

bool ExitState::exit() {
	return true;
}

void ExitState::handleEvent(SDL_Event& e) {
}

void ExitState::update() {

}

void ExitState::render() {

}

ExitState::ExitState() {

}