/**
 * @file ExitState.h
 * @brief The header file for the ExitState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"

class ExitState : public GameState {
public:
    //Static accessor
    static ExitState* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    //Static instance
    static ExitState sExitState;
};
