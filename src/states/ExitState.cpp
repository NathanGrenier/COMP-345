#include <SDL3/SDL_events.h>
#include <states/ExitState.h>
#include <SDL3/SDL_init.h>

/** @class ExitState
 *  @brief Implementation of the exit state.
 *
 *  This state is responsible for gracefully quitting the SDL framework
 *  when the game is exiting.
 *
 *  @author Nirav Patel
 */

 /// Static instance of the ExitState
ExitState ExitState::sExitState;

/**
 * @brief Retrieves the singleton instance of the ExitState.
 *
 * @return Pointer to the ExitState instance.
 */
ExitState* ExitState::get() {
    return &sExitState;
}

/**
 * @brief Handles the entry into the exit state.
 *
 * Cleans up SDL resources by calling SDL_Quit() before exiting.
 *
 * @return Always returns true.
 */
bool ExitState::enter() {
    SDL_Quit();
    return true;
}

/**
 * @brief Handles the exit process of the exit state.
 *
 * Since SDL_Quit() is already called in enter(), this function does nothing.
 *
 * @return Always returns true.
 */
bool ExitState::exit() {
    return true;
}

/**
 * @brief Handles events in the exit state.
 *
 * No event handling is required in this state.
 *
 * @param e The SDL_Event object containing input data.
 */
void ExitState::handleEvent(SDL_Event& e) {
    // No event handling required
}

/**
 * @brief Updates the exit state logic.
 *
 * No updates are necessary for this state.
 */
void ExitState::update() {
    // No update logic needed
}

/**
 * @brief Renders the exit state.
 *
 * This function is empty since the exit state does not display anything.
 */
void ExitState::render() {
    // No rendering needed
}
