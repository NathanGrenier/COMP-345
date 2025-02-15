#include "Titlestate.h"
#include "Global.h"
#include "UI/LButton.cpp"

TitleState TitleState::sTitleState;

//Place buttons
constexpr int kButtonCount = 4;
LButton buttons[kButtonCount];

//TitleState Implementation
TitleState* TitleState::get()
{
    //Get static instance
    return &sTitleState;
}

bool TitleState::enter()
{
    //Loading success flag
    bool success = true;

    //Load text
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
    if (success &= mMessageTexture.loadFromRenderedText("Tower Defense - The Game", textColor); !success)
    {
        printf("Failed to render title text!\n");
        success = false;
    }

    return success;
}

bool TitleState::exit()
{
    //Free background and text
    mBackgroundTexture.destroy();
    mMessageTexture.destroy();

    return true;
}

void TitleState::handleEvent(SDL_Event& e)
{
    //Handle button events
    for (int i = 0; i < kButtonCount; ++i)
    {
        buttons[i].handleEvent(&e);
    }
}

void TitleState::update()
{
    //Fill the background
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //Render buttons
    for (int i = 0; i < kButtonCount; i++)
    {
        buttons[i].render();
    }

    //Update screen
    SDL_RenderPresent(gRenderer);
}

void TitleState::render()
{
    buttons[0].setPosition(0, 0);
    buttons[1].setPosition(Global::kScreenWidth - LButton::kButtonWidth, 0);
    buttons[2].setPosition(0, Global::kScreenHeight - LButton::kButtonHeight);
    buttons[3].setPosition(Global::kScreenWidth - LButton::kButtonWidth, Global::kScreenHeight - LButton::kButtonHeight);

    //Show the background
    mBackgroundTexture.render(0, 0);

    //Show the message
    mMessageTexture.render((Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f, 20);
}

TitleState::TitleState()
{
    //No public instantiation
}