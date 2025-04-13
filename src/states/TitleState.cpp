#include <states/TitleState.h>
#include <states/MainGameState.h>
#include <states/MapSelectState.h>
#include <states/ExitState.h>
#include <states/OptionsState.h>
#include <Global.h>
#include <ui/LButton.h>

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

/**
 * @brief Gets the singleton instance of TitleState.
 *
 * @return Pointer to the TitleState instance.
 */
TitleState *TitleState::get()
{
	return &sTitleState;
}

/**
 * @brief Enters the title state, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool TitleState::enter()
{
	bool success = true;
	SDL_Color textColor{0x00, 0x00, 0x00, 0xFF};

	if (!(success &= mMessageTexture.loadFromFile("ui/TitleMessage.png")))
	{
		printf("Failed to render title text!\n");
	}

	// Button labels
	const char *buttonLabels[kButtonCount] = {
		"Load Main Game"};

	// Initialize buttons
	buttons[0].loadFromFile("ui/Start.png");
	buttons[1].loadFromFile("ui/Options.png");
	buttons[2].loadFromFile("ui/QuitButton.png");

	buttons[0].setSizeWithAspectRatio(400, 0);
	buttons[1].setSizeWithAspectRatio(400, 0);
	buttons[2].setSizeWithAspectRatio(400, 0);

	// Define vertical spacing for buttons
	constexpr int buttonSpacing = 20;
	float totalHeight = 0;
	for (int i = 0; i < kButtonCount; ++i)
	{
		totalHeight += buttons[i].kButtonHeight + buttonSpacing;
	}
	totalHeight -= buttonSpacing; // Remove extra spacing after the last button

	float startY = (Global::kScreenHeight - totalHeight) / 2;

	// Set button positions centered on the screen
	for (int i = 0; i < kButtonCount; ++i)
	{
		buttons[i].setPosition((Global::kScreenWidth - buttons[i].kButtonWidth) / 2, startY);
		startY += buttons[i].kButtonHeight + buttonSpacing;
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
bool TitleState::exit()
{
	return true;
}

/**
 * @brief Handles user input and button clicks.
 *
 * Processes mouse events and transitions to the appropriate game state when a button is clicked.
 *
 * @param e The SDL_Event object containing input data.
 */
void TitleState::handleEvent(SDL_Event &e)
{
	for (int i = 0; i < kButtonCount; ++i)
	{
		buttons[i].handleEvent(&e);

		// Check if a button is clicked
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if (buttons[i].isClicked())
			{
				// Transition to the corresponding game state
				switch (i)
				{
				case 0:
					setNextState(MapSelectState::get());
					break;
				case 1:
					setNextState(OptionsState::get());
					break;
				case 2:
					setNextState(ExitState::get());
					break;
				}
			}
		}
	}
}

/**
 * @brief Updates the title state logic.
 *
 */
void TitleState::update() {
	for (int i = 0; i < kButtonCount; ++i) {
		buttons[i].update();
	}
}

/**
 * @brief Renders the title state screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void TitleState::render()
{
	// Render title text centered at the top
	mMessageTexture.render((Global::kScreenWidth - Global::kScreenWidth * 0.9) / 2.f, 40, nullptr, Global::kScreenWidth * 0.9, -1);

	for (int i = 0; i < kButtonCount; ++i)
	{
		buttons[i].render();
	}
}