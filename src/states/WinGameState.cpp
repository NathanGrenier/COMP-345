/** @class WinGameState
 *  @brief Implementation of the WinGameState.
 *
 *
 *  @author Denmar Ermitano
 */

#include <states/WinGameState.h>
#include <states/EndScreenState.h>

 /// Static instance of the WinGameState
WinGameState WinGameState::sWinGameState;

/**
 * @brief Gets the singleton instance of WinGameState.
 *
 * @return Pointer to the WinGameState instance.
 */
WinGameState* WinGameState::get() {
	return &sWinGameState;
}

/**
 * @brief Enters the WinGameState, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool WinGameState::enter() {
	bool success = true;

	// small image for winning





	bool endScreenSuccess = EndScreenState::enter();

	return success && endScreenSuccess;
}

/**
 * @brief Exits the title state and frees resources.
 *
 * Cleans up textures before transitioning to another state.
 *
 * @return Always returns true.
 */
bool WinGameState::exit()
{
	EndScreenState::exit();

	return true;
}

/**
 * @brief Updates the WinGameState logic.
 *
 */
void WinGameState::update() {

	// animation for winning




	EndScreenState::update();
}

/**
 * @brief Renders the WinGameState screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void WinGameState::render() {
	// render winning image






	EndScreenState::render();
}
