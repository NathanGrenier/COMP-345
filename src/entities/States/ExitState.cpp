#include "ExitState.h"

ExitState ExitState::sExitState;

//Hollow exit state
ExitState* ExitState::get()
{
    return &sExitState;
}

bool ExitState::enter()
{
    return true;
}

bool ExitState::exit()
{
    return true;
}

void ExitState::handleEvent(SDL_Event& e)
{

}

void ExitState::update()
{

}

void ExitState::render()
{

}

ExitState::ExitState()
{

}