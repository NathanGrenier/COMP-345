/**
 * @file Part1State.cpp
 * @brief The drive file for part 1 of assignment 1 (Map)
 * @author Nathan Grenier
 * @date 2025-02-15
 *
 * @details Controls:
 *  - Left Click: Place a wall.
 *  - Right Click: Remove a wall.
 *  - Shift + Left Click: Set the target cell.
 *  - Shift + Right Click: Set the spawner cell.
 */

#include <states/parts/Part1State.h>
#include <Global.h>
#include <util/TextureLoader.h>
#include <map/Map.h>
#include <ui/LTexture.h>

 /** @brief Singleton instance of the Part1State */
Part1State Part1State::sPart1State;


/**
 * @brief Default constructor for Part1State
 */
Part1State::Part1State() {

}

/**
 * @brief Get the singleton instance of Part1State
 * @return Pointer to the Part1State singleton
 */
Part1State* Part1State::get() {
	return &sPart1State;
}

/**
 * @brief Initialize the state
 *
 * @details Creates a new Map instance sized to fit the screen dimensions
 *
 * @return true if initialization was successful
 */
bool Part1State::enter() {
	map = new Map(gRenderer, Global::kScreenWidth / Map::PIXELS_PER_CELL, Global::kScreenHeight / Map::PIXELS_PER_CELL);
	return true;
}

/**
 * @brief Clean up the state
 *
 * @details Deallocates textures and frees the map
 *
 * @return true if cleanup was successful
 */
bool Part1State::exit() {
	TextureLoader::deallocateTextures();
	delete map;
	map = nullptr;
	return true;
}

/**
 * @brief Handle input events
 *
 * @details Processes mouse and keyboard input to:
 *  - Place/remove walls with left/right click
 *  - Set target/spawner with Shift + left/right click
 *
 * @param e SDL event to process
 */
void Part1State::handleEvent(SDL_Event& e) {
	bool mouseDownThisFrame = false;
	bool keyDownThisFrame = false;

	// Process mouse events
	switch (e.type) {
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			mouseDownThisFrame = (mouseDownStatus == 0);
			if (e.button.button == SDL_BUTTON_LEFT)
				mouseDownStatus = SDL_BUTTON_LEFT;
			else if (e.button.button == SDL_BUTTON_RIGHT)
				mouseDownStatus = SDL_BUTTON_RIGHT;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			mouseDownStatus = 0;
			break;
	}

	// Process keyboard events
	switch (e.type) {
		case SDL_EVENT_KEY_DOWN:
			keyDownThisFrame = (keyDownStatus == 0);
			keyDownStatus = e.key.scancode;
			break;
		case SDL_EVENT_KEY_UP:
			keyDownStatus = 0;
			break;
	}

	// Handle mouse input
	float mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	Vector2D mousePosition((float)mouseX / Map::PIXELS_PER_CELL, (float)mouseY / Map::PIXELS_PER_CELL);

	if (mouseDownStatus > 0) {
		switch (mouseDownStatus) {
			case SDL_BUTTON_LEFT:
				if (SDL_GetModState() & SDL_KMOD_SHIFT) {
					// Set Target
					if (map != nullptr) {
						map->setTarget((int)mousePosition.x, (int)mousePosition.y);
					}
				} else {
					if (map != nullptr) {
						map->setCellWall((int)mousePosition.x, (int)mousePosition.y, true);
					}
				}
				break;
			case SDL_BUTTON_RIGHT:
				if (SDL_GetModState() & SDL_KMOD_SHIFT) {
					// Set Spawn
					if (map != nullptr) {
						map->setSpawner((int)mousePosition.x, (int)mousePosition.y);
					}
				} else {
					// Remove Wall
					if (map != nullptr) {
						map->setCellWall((int)mousePosition.x, (int)mousePosition.y, false);
					}
				}
				break;
		}
	}

	// Handle keyboard input
	switch (e.key.scancode) {
		case SDL_SCANCODE_H:
			if (map != nullptr) {
				// SDL_Log("Valid Path: %s", map->isValidPath() ? "true" : "false");
			}
			break;
	}
}

/**
 * @brief Update the state
 *
 * @details Currently empty, reserved for future implementation
 */
void Part1State::update() {

}

/**
 * @brief Render the state
 *
 * @details Draws the map and displays path validity status in the top-left corner
 */
void Part1State::render() {
	if (map != nullptr) {
		map->draw(gRenderer);
	}

	// Render the validity of the path
	SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF };
	std::string validPathText = "Valid Path: " + std::string(map->isValidPath() ? "TRUE" : "FALSE");
	LTexture validityTexture;
	validityTexture.loadFromRenderedText(validPathText, textColor);
	int textPadding = 20;
	validityTexture.render(0 + textPadding, 0 + textPadding);

	return;
}