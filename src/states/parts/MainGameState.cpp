#include <states/parts/MainGameState.h>
#include <Global.h>

/** @class MainGameState
 *  @brief Implementation of the main game state.
 *
 *  This state is responsible for handling the main gameplay logic.
 *  It follows a singleton pattern to ensure only one instance exists.
 *
 */

 /// Static instance of MainGameState
MainGameState MainGameState::sMainGameState;

/**
 * @brief Retrieves the singleton instance of the MainGameState.
 *
 * @return Pointer to the MainGameState instance.
 */
MainGameState* MainGameState::get() {
    return &sMainGameState;
}

/**
 * @brief Handles the entry logic for the main game state.
 *
 * This function is called when transitioning into the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::enter() {
    return true;
}

/**
 * @brief Handles the exit logic for the main game state.
 *
 * This function is called when transitioning out of the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::exit() {
    return true;
}

/**
 * @brief Handles user input and events.
 *
 * @param e The SDL_Event object containing input data.
 */
void MainGameState::handleEvent(SDL_Event& e) {
    // Event handling logic will be added here in the future
}

/**
 * @brief Updates the game logic for the main game state.
 *
 * This function is called every frame to update the game's logic.
 */
void MainGameState::update() {
    // Game update logic will be added here in the future
}

/**
 * @brief Renders the main game state.
 *
 * This function is called every frame to render the game's visuals.
 */
void MainGameState::render() {
    // Rendering logic will be added here in the future
}

/**
 * @brief Private constructor to prevent direct instantiation.
 *
 * The MainGameState follows the singleton pattern.
 */
MainGameState::MainGameState() {}
