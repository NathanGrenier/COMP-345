/**
 * @file TowerButton.h
 * @brief Class for buttons in Part2State
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class of TowerButton
 * Buttons used for selecting tower class, upgrading towers, and selling towers
 */

#pragma once
#include "LButton.h"

class TowerButton : public LButton {
public:
    // Constructor
    TowerButton();

    static const int kButtonWidth{ 100 }; /** @brief default width for tower buttons */
    static const int kButtonHeight{ 25 }; /** @brief default height for tower buttons */

    //void setPosition(float x, float y);

    //// Handles mouse events and triggers scaling animation
    //void handleEvent(SDL_Event* e);

    void render();

protected:
	//SDL_FPoint mPosition;
};