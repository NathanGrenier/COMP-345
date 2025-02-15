#include "GameState.h"

#ifndef INTROSTATE_H
#define INTROSTATE_H
class IntroState : public GameState
{
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

    //Private constructor
    IntroState();

    //Intro background
    LTexture mBackgroundTexture;

    //Intro message
    LTexture mMessageTexture;
};
#endif