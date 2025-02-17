#pragma once
#include "LButton.h"

class TowerButton : public LButton {
public:
    // Constructor
    TowerButton();

    static const int kButtonWidth{ 100 };
    static const int kButtonHeight{ 25 };

    //void setPosition(float x, float y);

    //// Handles mouse events and triggers scaling animation
    //void handleEvent(SDL_Event* e);

    void render();

protected:
	//SDL_FPoint mPosition;
};