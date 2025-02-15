#include "GameState.h"

#ifndef EXITSTATE_H
#define EXITSTATE_H
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

    //Private constructor
    ExitState();
};
#endif