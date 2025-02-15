#include <states/parts/Part1State.h>

Part1State Part1State::sPart1State;

//Hollow exit state
Part1State* Part1State::get() {
	return &sPart1State;
}

bool Part1State::enter() {
	return true;
}

bool Part1State::exit() {
	return true;
}

void Part1State::handleEvent(SDL_Event& e) {

}

void Part1State::update() {

}

void Part1State::render() {

}

Part1State::Part1State() {

}