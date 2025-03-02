#include <states/parts/MainGameState.h>
#include <critter/CritterGroup.h>
#include <Global.h>
#include <util/TextureLoader.h>

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
	if (Global::currentMap == nullptr || Global::currentMap->getName().empty()) {
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	playerGold = 100;
	waveLevel = 1;

	std::cout << "Global::currentMap name: " + Global::currentMap->name << std::endl;

	map = new Map(*Global::currentMap);

	map->setCurrentRenderRect(Global::mapViewRect);

	critterGroup = new CritterGroup(waveLevel, playerGold, map->getSpawnerPos(Global::mapViewRect), map->getTargetPos(Global::mapViewRect), map);

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
	TextureLoader::deallocateTextures();

	if (critterGroup != nullptr) {
		delete critterGroup;
		critterGroup = nullptr;
	}

	if (map != nullptr) {
		delete map;
		map = nullptr;
	}

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
	critterGroup->generateCritters(0.16f);
	critterGroup->update(0.016f);

	if (playerGold < 0) {
		warningMessage = "You are out of gold!";
	} else {
		warningMessage = "";
	}
}

/**
 * @brief Renders the main game state.
 *
 * This function is called every frame to render the game's visuals.
 */
void MainGameState::render() {
	map->drawOnTargetRect(gRenderer, map->getCurrentRenderRect());

	critterGroup->render(gRenderer);

	//for (auto& tower : towers) {
	//	tower.render(gRenderer);
	//}

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
void MainGameState::renderText(const std::string& text, float x, float y) {
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}