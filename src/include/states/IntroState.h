/**
 * @file IntroState.h
 * @brief The header file for the IntroState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"
#include "../UI/LTexture.h"

class IntroState : public GameState {
public:
    //Static accessor
    static IntroState* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    //Static instance
    static IntroState sIntroState;

    //Intro background
    LTexture mBackgroundTexture;

    //Intro message
    LTexture mMessageTexture;
    
    LTexture mCursorPromptTexture;

    float oscillationOffset = 0.0f;
};
