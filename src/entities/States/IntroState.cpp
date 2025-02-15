#include "IntroState.h"
#include "../Global.h"
#include "main.h"

IntroState IntroState::sIntroState;

//InrtoState Implementation
IntroState* IntroState::get()
{
    //Get static instance
    return &sIntroState;
}

bool IntroState::enter()
{
    //Loading success flag
    bool success = true;

    //Load text
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
    if (success &= mMessageTexture.loadFromRenderedText("The NullTerminators Present...", textColor); !success)
    {
        SDL_Log("Failed to render intro text!\n");
        success = false;
    }

    return success;
}

bool IntroState::exit()
{
    //Free background and text
    mBackgroundTexture.destroy();
    mMessageTexture.destroy();

    return true;
}

void IntroState::handleEvent(SDL_Event& e)
{
    //If the user pressed enter
    if ((e.type == SDL_EVENT_MOUSE_BUTTON_DOWN))
    {
        //Move onto title state
        setNextState(TitleState::get());
    }
}

void IntroState::update()
{

}

void IntroState::render()
{
    //Show the background
    mBackgroundTexture.render(0, 0);

    //Show the message
    mMessageTexture.render((Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f, (Global::kScreenHeight - mMessageTexture.getHeight()) / 2.f);
}

IntroState::IntroState() {
}
