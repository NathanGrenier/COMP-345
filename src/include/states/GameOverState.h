/**
 * @file GameOverState.h
 * @brief The header file for the GameOverState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include "EndScreenState.h"
#include "../UI/LTexture.h"
#include <string>

class GameOverState : public EndScreenState {
public:
    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void update() override;
    void render() override;

    //Static accessor
    static GameOverState* get();

private:
    //Static instance
    static GameOverState sGameOverState;

    //Critter animation


    //Critter taunt message
    LTexture killedByMessage;
    LTexture mMessageTexture;
};
