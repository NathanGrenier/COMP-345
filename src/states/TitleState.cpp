#include <states/TitleState.h>
#include <ui/MainMenuButton.h>  // Include the MainMenuButton header
#include <states/parts/MainGameState.h>
#include <states/parts/Part1State.h>
#include <states/parts/Part2State.h>
#include <states/parts/Part3State.h>
#include <Global.h>

TitleState TitleState::sTitleState;

// Place buttons
constexpr int kButtonCount = 4;
MainMenuButton buttons[kButtonCount];  // Use MainMenuButton instead of LButton

// TitleState Implementation
TitleState* TitleState::get() {
    // Get static instance
    return &sTitleState;
}

bool TitleState::enter() {
    bool success = true;

    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    if (!(success &= mMessageTexture.loadFromRenderedText("Tower Defense - The Game", textColor))) {
        printf("Failed to render title text!\n");
    }

    // Assign text to buttons
    const char* buttonLabels[kButtonCount] = {
        "Load Main Game",
        "Load Part 1",
        "Load Part 2",
        "Load Part 3"
    };

    for (int i = 0; i < kButtonCount; ++i) {
        if (!buttons[i].setText(buttonLabels[i], textColor)) {
            printf("Failed to set button text: %s\n", buttonLabels[i]);
            success = false;
        }
    }

    // Define vertical spacing
    constexpr int buttonSpacing = 20;  // Space between buttons
    constexpr int startY = (Global::kScreenHeight - ((MainMenuButton::kButtonHeight * kButtonCount) + (buttonSpacing * (kButtonCount - 1)))) / 2;

    // Set button positions
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].setPosition((Global::kScreenWidth - MainMenuButton::kButtonWidth) / 2, startY + i * (MainMenuButton::kButtonHeight + buttonSpacing));
    }

    return success;
}

bool TitleState::exit() {
    // Free background and text
    mBackgroundTexture.destroy();
    mMessageTexture.destroy();

    return true;
}

void TitleState::handleEvent(SDL_Event& e) {
    // Handle button events
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].handleEvent(&e);

        // Check if any of the buttons were clicked and set the next state accordingly
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (buttons[i].isClicked()) {
                // Transition to the corresponding part state
                switch (i) {
                case 0:  // "Load Main Game" - Can stay in TitleState or go to main game
                    setNextState(MainGameState::get()); // Assuming you have a MainGameState
                    break;

                case 1:  // "Load Part 1"
                    setNextState(Part1State::get());
                    break;

                case 2:  // "Load Part 2"
                    setNextState(Part2State::get());
                    break;

                case 3:  // "Load Part 3"
                    setNextState(Part3State::get());
                    break;
                }
            }
        }
    }
}

void TitleState::update() {
    // Update logic if any
}

void TitleState::render() {
    // Fill the background
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // Show the background
    mBackgroundTexture.render(0, 0);

    // Show the message
    mMessageTexture.render((Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f, 20);

    // Render buttons
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].render();  // Render the MainMenuButton instead of LButton
    }
}

TitleState::TitleState() {
    // No public instantiation
}
