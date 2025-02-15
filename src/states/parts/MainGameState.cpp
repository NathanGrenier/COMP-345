#include "parts/MainGameState.h"

MainGameState MainGameState::sMainGameState;

//Hollow exit state
MainGameState* MainGameState::get() {
	return &sMainGameState;
}

bool MainGameState::enter() {
	return true;
}

bool MainGameState::exit() {
	return true;
}

void MainGameState::handleEvent(SDL_Event& e) {

}

void MainGameState::update() {

}

void MainGameState::render() {

}

MainGameState::MainGameState() {

}