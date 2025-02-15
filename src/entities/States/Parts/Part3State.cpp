#include "Part3State.h"

Part3State Part3State::sPart3State;

//Hollow exit state
Part3State* Part3State::get()
{
    return &sPart3State;
}

bool Part3State::enter()
{
    return true;
}

bool Part3State::exit()
{
    return true;
}

void Part3State::handleEvent(SDL_Event& e)
{

}

void Part3State::update()
{

}

void Part3State::render()
{

}

Part3State::Part3State()
{

}