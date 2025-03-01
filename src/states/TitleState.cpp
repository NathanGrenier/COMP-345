#include <states/TitleState.h>
#include <ui/MainMenuButton.h>
#include <states/parts/MainGameState.h>
#include <states/parts/Part1State.h>
#include <states/parts/Part2State.h>
#include <states/parts/Part3State.h>
#include <states/parts/UITestState.h>
#include <Global.h>

/** @class TitleState
 *  @brief Implementation of the title screen state.
 *
 *  This state handles the title screen, where players can start the game, load different parts,
 *  and interact with the main menu buttons.
 *
 *  @author Nirav Patel
 */

 /// Static instance of the TitleState
TitleState TitleState::sTitleState;

/// @brief Number of buttons in the main menu.
constexpr int kButtonCount = 4;

/// @brief Array of main menu buttons.
MainMenuButton buttons[kButtonCount];  // Use MainMenuButton instead of LButton

/**
 * @brief Gets the singleton instance of TitleState.
 *
 * @return Pointer to the TitleState instance.
 */
TitleState* TitleState::get() {
    return &sTitleState;
}

/**
 * @brief Enters the title state, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool TitleState::enter() {
    bool success = true;
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    if (!(success &= mMessageTexture.loadFromRenderedText("Tower Defense - The Game", textColor))) {
        printf("Failed to render title text!\n");
    }

    // Button labels
    const char* buttonLabels[kButtonCount] = {
        "Load Main Game",
        "Load Part 1",
        "Load Part 2",
        "Load Part 3"
    };

    // Initialize buttons
    for (int i = 0; i < kButtonCount; ++i) {
        if (!buttons[i].setText(buttonLabels[i], textColor)) {
            printf("Failed to set button text: %s\n", buttonLabels[i]);
            success = false;
        }
    }

    // Define vertical spacing for buttons
    constexpr int buttonSpacing = 20;
    constexpr int startY = (Global::kScreenHeight - ((MainMenuButton::kButtonHeight * kButtonCount) + (buttonSpacing * (kButtonCount - 1)))) / 2;

    // Set button positions centered on the screen
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].setPosition((Global::kScreenWidth - MainMenuButton::kButtonWidth) / 2, startY + i * (MainMenuButton::kButtonHeight + buttonSpacing));
    }

    return success;
}

/**
 * @brief Exits the title state and frees resources.
 *
 * Cleans up textures before transitioning to another state.
 *
 * @return Always returns true.
 */
bool TitleState::exit() {
    mBackgroundTexture.destroy();
    mMessageTexture.destroy();
    return true;
}

/**
 * @brief Handles user input and button clicks.
 *
 * Processes mouse events and transitions to the appropriate game state when a button is clicked.
 *
 * @param e The SDL_Event object containing input data.
 */
void TitleState::handleEvent(SDL_Event& e) {
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].handleEvent(&e);

        // Check if a button is clicked
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (buttons[i].isClicked()) {
                // Transition to the corresponding game state
                switch (i) {
                case 0:
                    setNextState(MainGameState::get());  // Load main game
                    break;
                case 1:
                    setNextState(Part1State::get());  // Load Part 1
                    break;
                case 2:
                    //setNextState(Part2State::get());  // Load Part 2
                    setNextState(UITestState::get());  // Load Part 2
                    break;
                case 3:
                    setNextState(Part3State::get());  // Load Part 3
                    break;
                }
            }
        }
    }
}

/**
 * @brief Updates the title state logic.
 *
 * Currently, this function does not perform any updates.
 */
void TitleState::update() {
    // No updates needed for title state
}

/**
 * @brief Renders the title state screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void TitleState::render() {
    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // Render background
    mBackgroundTexture.render(0, 0);

    // Render title text centered at the top
    mMessageTexture.render((Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f, 20);

    // Render menu buttons
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i].render();
    }
}

/**
 * @brief Private constructor to prevent direct instantiation.
 *
 * The TitleState follows the singleton pattern.
 */
TitleState::TitleState() {
    // No public instantiation allowed
}
