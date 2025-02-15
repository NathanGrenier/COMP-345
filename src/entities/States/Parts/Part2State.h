#include "GameState.h"
#include "../../UI/LTexture.h"

#ifndef PART2STATE_H
#define PART2STATE_H
class Part2State : public GameState
{
public:
    //Static accessor
    static Part2State* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    //Static instance
    static Part2State sPart2State;

    //Private constructor
    Part2State();

    //Intro background
    LTexture mBackgroundTexture;

    //Intro message
    LTexture mMessageTexture;
};
#endif