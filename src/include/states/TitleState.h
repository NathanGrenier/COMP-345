/**
 * @file TitleState.h
 * @brief The header file for the TitleState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"
#include "../UI/LTexture.h"

class TitleState : public GameState {
public:
    //Static accessor
    static TitleState* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    //Static instance
    static TitleState sTitleState;

    //Intro background
    LTexture mBackgroundTexture;

    //Intro message
    LTexture mMessageTexture;
};
