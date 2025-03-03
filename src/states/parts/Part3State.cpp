/**
 * @class Part3State
 * @brief Contains the implementation for handling the third part of the game logic.
 *
 * @author Nirav Patel
 * @date 2025-02-16
 */

#include <states/parts/Part3State.h>
#include "../../critter/CritterGroup.cpp"
#include <Global.h>

 // Forward declarations and global variables

 /**
  * @brief Start square at the left edge of the screen.
  */
SDL_FRect startSquare;

/**
 * @brief End square at the right edge of the screen.
 */
SDL_FRect endSquare;

/**
 * @brief Player's current gold amount.
 */
int playerGold;

/**
 * @brief Current wave level in the game.
 */
int waveLevel;

/**
 * @brief Warning message that can be displayed on the screen.
 */
std::string warningMessage = "";

/**
 * @brief Time duration for which the warning message will be displayed.
 */
float warningTime = 0.0f;

/**
 * @brief Singleton instance of Part3State.
 */
Part3State Part3State::sPart3State;

/**
 * @brief Pointer to the CritterGroup instance.
 */
CritterGroup* critterGroup;

/**
 * @brief Retrieves the singleton instance of Part3State.
 *
 * @return Pointer to the Part3State instance.
 */
Part3State* Part3State::get() {
	return &sPart3State;
}

/**
 * @brief Initializes the Part3State, including setting the start and end square positions,
 *        setting the initial player gold, and creating the CritterGroup.
 *
 * @return True if the state was successfully entered.
 */
bool Part3State::enter() {
	Global::currentMap = new Map(15, 15, "Default");
	startSquare = { 0.0f, 300.0f, 50.0f, 50.0f };
	endSquare = { Global::kScreenWidth - 50.0f, 300.0f, 50.0f, 50.0f };
	playerGold = 100;
	waveLevel = 1;
	critterGroup = new CritterGroup(waveLevel, playerGold, startSquare, endSquare, Global::currentMap);
	return true;
}

/**
 * @brief Exits the Part3State. Currently does not perform any specific cleanup.
 *
 * @return True when the state is successfully exited.
 */
bool Part3State::exit() {
	return true;
}

/**
 * @brief Handles user input events like key presses and mouse clicks.
 *
 * @param e The SDL_Event representing the user input event.
 */
void Part3State::handleEvent(SDL_Event& e) {
	if (e.type == SDL_EVENT_KEY_DOWN) {
		switch (e.key.key) {
			case SDLK_W:  // Move start square up
				startSquare.y -= 10.0f;
				break;
			case SDLK_S:  // Move start square down
				startSquare.y += 10.0f;
				break;
			case SDLK_UP:  // Move end square up
				endSquare.y -= 10.0f;
				break;
			case SDLK_DOWN:  // Move end square down
				endSquare.y += 10.0f;
				break;
		}
	} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float x, y;
		SDL_GetMouseState(&x, &y);
	}
}

/**
 * @brief Updates the game logic for the Part3State, including generating and moving critters.
 */
void Part3State::update() {
	critterGroup->generateCritters(0.16f);
	critterGroup->update(0.016f);
}

/**
 * @brief Renders the game objects and UI elements for Part3State, including the start/end squares,
 *        critters, towers, player gold, wave level, and any warning messages.
 */
void Part3State::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);

	// Render start square (S)
	SDL_RenderFillRect(gRenderer, &startSquare);
	renderText("S", startSquare.x + startSquare.w / 4, startSquare.y + startSquare.h / 4);  // Label for 'S'

	// Render end square (E)
	SDL_RenderFillRect(gRenderer, &endSquare);
	renderText("E", endSquare.x + endSquare.w / 4, endSquare.y + endSquare.h / 4);  // Label for 'E'

	critterGroup->render(gRenderer);

	// Render player gold
	renderText("Gold: " + std::to_string(playerGold), 10.0f, 10.0f);

	// Render wave level
	std::string waveText = "Wave: " + std::to_string(waveLevel);
	float waveX = (Global::kScreenWidth - (waveText.length() * 10.0f)) / 2.0f;
	renderText(waveText, waveX, 10.0f);

	// Render warning message if any
	if (!warningMessage.empty()) {
		float warningX = (Global::kScreenWidth - (warningMessage.length() * 10.0f)) / 2.0f;
		float warningY = Global::kScreenHeight - 40.0f;
		renderText(warningMessage, warningX, warningY);
	}
}

/**
 * @brief Helper function to render text at a specific position on the screen.
 *
 * @param text The text to be rendered.
 * @param x The x-coordinate of the text.
 * @param y The y-coordinate of the text.
 */
void Part3State::renderText(const std::string& text, float x, float y) {
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}

/**
 * @brief Constructor for Part3State.
 */
Part3State::Part3State() {
	// Initialization code
}
