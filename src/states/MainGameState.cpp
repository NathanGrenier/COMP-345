#include <states/MainGameState.h>
#include <critter/CritterGroup.h>
#include <Global.h>
#include <util/TextureLoader.h>
#include <towers/CannonTower.h>
#include <towers/RapidFireTower.h>
#include <towers/StandardTower.h>
#include <ui/DetailButton.h>
#include <towers/TowerGroup.h>
#include <states/TitleState.h>


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
MainGameState *MainGameState::get()
{
	return &sMainGameState;
}

/**
 * @brief Handles the entry logic for the main game state.
 *
 * This function is called when transitioning into the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::enter()
{
	if (Global::currentMap == nullptr)
	{
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	float intButtonHeight = 40.0f;

	pauseButton.loadFromFile("assets/ui/PauseButton.png");
	exitButton.loadFromFile("assets/ui/ExitButton.png");

	exitButton.setSizeWithAspectRatio(0, intButtonHeight);
	pauseButton.setSizeWithAspectRatio(0, intButtonHeight);

	exitButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30 + pauseButton.kButtonWidth + 30, Global::kScreenHeight - intButtonHeight - 20);
	pauseButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30, Global::kScreenHeight - intButtonHeight - 20);

	map = new Map(*Global::currentMap);
	map->setFlowFieldVisibility(false);

	for (auto& cell : map->cells)
	{
		if (cell.isWall)
		{
			map->wallCellDict[cell] = false;
		}

		else if (cell.isOnPath)
		{
			map->wallCellDict[cell] = true;
		}
	}

	detailDisplay = DetailAttributeDisplay();
	bool success = detailDisplay.initializeComponents();

	critterGroup = new CritterGroup(waveLevel, playerGold, map->getSpawnerPos(Global::mapViewRect), map->getTargetPos(Global::mapViewRect), map, detailDisplay);
	towerGroup = new TowerGroup(playerGold, map, detailDisplay);

	return success;
}

/**
 * @brief Handles the exit logic for the main game state.
 *
 * This function is called when transitioning out of the main game state.
 *
 * @return Always returns true.
 */
bool MainGameState::exit()
{
	TextureLoader::deallocateTextures();

	mBackgroundTexture.destroy();

	mMessageTexture.destroy();

	pauseButton.destroy();
	exitButton.destroy();

	isPaused = false;

	delete critterGroup;
	critterGroup = nullptr;

	delete towerGroup;
	towerGroup = nullptr;

	delete map;
	map = nullptr;

	playerGold = 999;
	waveLevel = 0;

	return true;
}

/**
 * @brief Handles user input and events.
 *
 * @param e The SDL_Event object containing input data.
 */
void MainGameState::handleEvent(SDL_Event &e)
{
	// handles hovering, clicking of buttons
	if (!isPaused) {
		towerGroup->handleEvent(e);
		critterGroup->handleEvent(e);
		detailDisplay.handleButtonEvents(e);
	}

	pauseButton.handleEvent(&e);
	exitButton.handleEvent(&e);

	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		if (pauseButton.isClicked())
		{
			float buttonHeight = 40.0f;

			if (isPaused) {
				pauseButton.loadFromFile("assets/ui/PauseButton.png");
				pauseButton.setSizeWithAspectRatio(0, buttonHeight);
				pauseButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30, Global::kScreenHeight - buttonHeight - 20);
				isPaused = false;
			}
			else {
				pauseButton.loadFromFile("assets/ui/PlayButton.png");
				pauseButton.setSizeWithAspectRatio(0, buttonHeight);
				pauseButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30, Global::kScreenHeight - buttonHeight - 20);
				isPaused = true;
			}

			
		}
		if (exitButton.isClicked())
		{
			setNextState(TitleState::get());
		}
	}
}

/**
 * @brief Updates the game logic for the main game state.
 *
 * This function is called every frame to update the game's logic.
 */
void MainGameState::update()
{
	if (isPaused) return;

	critterGroup->update(0.016f);

	towerGroup->update(0.016f, critterGroup->getCritters());
}

/**
 * @brief Renders the main game state.
 *
 * This function is called every frame to render the game's visuals.
 */
void MainGameState::render()
{
	SDL_FRect backRect = { 0, 0, Global::kScreenWidth - Global::viewerWidth, Global::headerHeight };

	// Set the renderer color for the outline
	SDL_SetRenderDrawColor(gRenderer, 168, 168, 168, 255); // Gray color for outline
	SDL_RenderFillRect(gRenderer, &backRect); // Draw box outline

	SDL_FRect foreRect = { 4, 4, Global::kScreenWidth - Global::viewerWidth - 5, Global::headerHeight - 8 };

	// Set the renderer color for the gray box
	SDL_SetRenderDrawColor(gRenderer, 202, 202, 202, 255); // Gray color for box
	SDL_RenderFillRect(gRenderer, &foreRect); // Draw filled box

	map->drawOnTargetRect(Global::mapViewRect);

	detailDisplay.render();

	critterGroup->render();

	towerGroup->render();

	pauseButton.render();
	exitButton.render();

	// Render player gold
	int displayedGold = std::min(playerGold, 999);
	renderText("Gold: " + std::to_string(playerGold), 10.0f, 10.0f);

	// Render wave level
	std::string waveText = "Wave: " + std::to_string(waveLevel);
	renderText(waveText, 10.0f, 50.0f);
}

/**
 * @brief Helper function to render text at a specific position on the screen.
 *
 * @param text The text to be rendered.
 * @param x The x-coordinate of the text.
 * @param y The y-coordinate of the text.
 */
void MainGameState::renderText(const std::string &text, float x, float y)
{
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}