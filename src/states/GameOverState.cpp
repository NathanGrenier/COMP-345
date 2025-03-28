/** @class GameOverState
 *  @brief Implementation of the GameOverState.
 *
 *
 *  @author Denmar Ermitano
 */

#include <states/GameOverState.h>
#include <states/EndScreenState.h>

 /// Static instance of the GameOverState
GameOverState GameOverState::sGameOverState;

/**
 * @brief Gets the singleton instance of GameOverState.
 *
 * @return Pointer to the GameOverState instance.
 */
GameOverState* GameOverState::get() {
	return &sGameOverState;
}

/**
 * @brief Enters the GameOverState, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool GameOverState::enter() {
	bool success = true;

	// generate critter sprite




	// TODO grab random taunt message





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
bool GameOverState::exit()
{
	EndScreenState::exit();

	return true;
}

/**
 * @brief Updates the GameOverState logic.
 *
 */
void GameOverState::update() {
	// animate critter





	
	
	
	EndScreenState::update();
}

/**
 * @brief Renders the GameOverState screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void GameOverState::render() {
	// render critter





	// render taunt





	EndScreenState::render();
}
