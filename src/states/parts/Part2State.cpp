#include <states/parts/Part2State.h>

Part2State Part2State::sPart2State;

//Hollow exit state
Part2State* Part2State::get() {
	return &sPart2State;
}

bool Part2State::enter() {
	return true;
}

bool Part2State::exit() {
	return true;
}

void Part2State::handleEvent(SDL_Event& e) {

}

void Part2State::update() {

}

void Part2State::render() {

}

Part2State::Part2State() {

}