#include <states/IntroState.h>
#include <SDL3/SDL_log.h>
#include <states/TitleState.h>
#include <Global.h>
#include <cmath>

/** @class IntroState
 *  @brief Implementation of the introduction state.
 *
 *  This state displays an introductory message with a simple oscillation effect
 *  before transitioning to the title screen.
 *
 *  @author Nirav Patel
 */

 /// Static instance of the IntroState
IntroState IntroState::sIntroState;

/// @brief Time variable used for text oscillation effect.
float mOscillationTime = 0.0f;

/**
 * @brief Retrieves the singleton instance of the IntroState.
 *
 * @return Pointer to the IntroState instance.
 */
IntroState* IntroState::get() {
    return &sIntroState;
}

/**
 * @brief Enters the introduction state and initializes resources.
 *
 * Loads the introductory text and prepares the state for rendering.
 *
 * @return True if successful, false otherwise.
 */
bool IntroState::enter() {
    bool success = true;

    // Load intro text
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
    if (success &= mMessageTexture.loadFromRenderedText("The NullTerminators Presents...", textColor); !success) {
        SDL_Log("Failed to render intro text!\n");
        success = false;
    }

    return success;
}

/**
 * @brief Exits the introduction state and frees resources.
 *
 * Cleans up textures before transitioning to another state.
 *
 * @return Always returns true.
 */
bool IntroState::exit() {
    mBackgroundTexture.destroy();
    mMessageTexture.destroy();
    return true;
}

/**
 * @brief Handles user input events in the introduction state.
 *
 * If the user clicks the mouse button, the game transitions to the title state.
 *
 * @param e The SDL_Event object containing input data.
 */
void IntroState::handleEvent(SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        // Transition to the title state
        setNextState(TitleState::get());
    }
}

/**
 * @brief Updates the introduction state logic.
 *
 * Currently, no updates are required for this state.
 */
void IntroState::update() {
    // No update logic needed for this state
}

/**
 * @brief Renders the introduction screen with a text oscillation effect.
 *
 * The message text moves up and down slightly using a sine wave oscillation effect.
 */
void IntroState::render() {
    // Update oscillation time
    mOscillationTime += 0.05f;  // Adjust speed of oscillation

    // Calculate the vertical oscillation offset (range of -5 to 5 pixels)
    float oscillationOffset = std::sin(mOscillationTime) * 5.0f;

    // Render background
    mBackgroundTexture.render(0, 0);

    // Render message text with oscillation effect
    mMessageTexture.render(
        (Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f,
        (Global::kScreenHeight - mMessageTexture.getHeight()) / 2.f + oscillationOffset
    );
}

/**
 * @brief Private constructor to prevent direct instantiation.
 *
 * The IntroState follows the singleton pattern.
 */
IntroState::IntroState() {}
