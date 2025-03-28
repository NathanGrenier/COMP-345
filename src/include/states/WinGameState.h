/**
 * @file WinGameState.h
 * @brief The header file for the WinGameState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include "EndScreenState.h"
#include "../UI/LTexture.h"
#include <string>

class WinGameState : public EndScreenState {
public:
    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void update() override;
    void render() override;

    //Static accessor
    static WinGameState* get();

private:
    //Static instance
    static WinGameState sWinGameState;

    //Win animation
    LTexture winAnimation;
};
