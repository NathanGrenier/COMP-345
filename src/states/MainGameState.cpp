#include <states/MainGameState.h>
#include <critter/CritterGroup.h>
#include <Global.h>
#include <towers/CannonTower.h>
#include <towers/RapidFireTower.h>
#include <towers/StandardTower.h>
#include <ui/DetailButton.h>
#include <towers/TowerGroup.h>
#include <states/TitleState.h>
#include <states/EndScreenState.h>
#include <states/WinGameState.h>
#include <states/GameOverState.h>

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
	float buttonHeight = 40.0f;

	if (Global::currentMap == nullptr)
	{
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	pauseButton.loadFromFile("ui/PauseButton.png", "assets/sfx/PauseButtonPress.wav");
	pauseButton.setSizeWithAspectRatio(0, buttonHeight);
	pauseButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30, Global::kScreenHeight - buttonHeight - 20);

	playButton.loadFromFile("ui/PlayButton.png", "assets/sfx/PauseButtonPress.wav");
	playButton.setSizeWithAspectRatio(0, buttonHeight);
	playButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30, Global::kScreenHeight - buttonHeight - 20);

	currentButton = &pauseButton;

	bg = new ParallaxBackground();
	std::srand(std::time(0));

	for (int i = 0; i < Global::numberOfProps; ++i)
	{
		float randomSpeed = 5.0f + std::rand() % 11;
		bg->addLayer(randomSpeed, Global::kScreenHeight);
	}

	exitButton.loadFromFile("ui/ExitButton.png");
	exitButton.setSizeWithAspectRatio(0, buttonHeight);
	exitButton.setPosition(Global::kScreenWidth - Global::viewerWidth + 30 + pauseButton.kButtonWidth + 30, Global::kScreenHeight - buttonHeight - 20);

	map = new Map(*Global::currentMap);
	map->setFlowFieldVisibility(false);

	for (auto &cell : map->cells)
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

	detailDisplay = new DetailAttributeDisplay();
	bool success = detailDisplay->initializeComponents();

	endlessMode = false;
	critterGroup = new CritterGroup(waveLevel, playerGold, map->getSpawnerPos(Global::mapViewRect), map->getTargetPos(Global::mapViewRect), map, detailDisplay, endlessMode);
	towerGroup = new TowerGroup(playerGold, map, detailDisplay);

	return success;
}

/**
 * @brief Handles user input and events.
 *
 * @param e The SDL_Event object containing input data.
 */
void MainGameState::handleEvent(SDL_Event &e)
{
	// handles hovering, clicking of buttons
	if (!isPaused)
	{
		towerGroup->handleEvent(e);
		critterGroup->handleEvent(e);
		detailDisplay->handleButtonEvents(e);
	}

	currentButton->handleEvent(&e);
	exitButton.handleEvent(&e);

	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		if (pauseButton.isClicked())
		{

			Global::logMessage(std::format("Game {}paused", isPaused ? "un" : ""));

			if (isPaused)
			{
				currentButton = &pauseButton;
			}
			else
			{
				currentButton = &playButton;
			}
			isPaused = !isPaused;

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
	currentButton->update();
	exitButton.update();
	detailDisplay->update();

	if (isPaused)
		return;

	bg->update(0.016f);

	critterGroup->update(0.016f);

	towerGroup->update(0.016f, critterGroup->getCritters());

	if (critterGroup->isGameFinished()) {
		EndScreenState* endScreen;

		if (critterGroup->isGameWon())
		{
			endScreen = WinGameState::get();
		}
		else 
		{
			endScreen = GameOverState::get();
			endScreen->setKilledBy(critterGroup->getKillerCritterType());
		}

		endScreen->setCrittersKilled(critterGroup->getTotalCrittersKilled());
		endScreen->setTowersBought(towerGroup->getTotalTowersPlaced());
		endScreen->setWave(waveLevel);

		setNextState(endScreen);
	}
}

/**
 * @brief Renders the main game state.
 *
 * This function is called every frame to render the game's visuals.
 */
void MainGameState::render()
{
	SDL_FRect backRect = {0, 0, Global::kScreenWidth - Global::viewerWidth, Global::headerHeight};

	// Set the renderer color for the outline
	SDL_SetRenderDrawColor(gRenderer, 168, 168, 168, 255); // Gray color for outline
	SDL_RenderFillRect(gRenderer, &backRect);			   // Draw box outline

	SDL_FRect foreRect = {4, 4, Global::kScreenWidth - Global::viewerWidth - 5, Global::headerHeight - 8};

	// Set the renderer color for the gray box
	SDL_SetRenderDrawColor(gRenderer, 202, 202, 202, 255); // Gray color for box
	SDL_RenderFillRect(gRenderer, &foreRect);			   // Draw filled box

	bg->render();

	map->drawOnTargetRect(Global::mapViewRect);

	detailDisplay->render();

	critterGroup->render();
	towerGroup->render();

	currentButton->render();
	exitButton.render();

	// Render player gold
	int displayedGold = std::min(playerGold, 999);
	renderText("Gold: " + std::to_string(displayedGold), 10.0f, 10.0f);

	// Render wave level
	std::string waveText = "Wave: " + std::to_string(waveLevel);
	renderText(waveText, 10.0f, 50.0f);
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
	isPaused = false;

	currentButton = &pauseButton;

	delete critterGroup;
	critterGroup = nullptr;

	delete towerGroup;
	towerGroup = nullptr;

	delete map;
	map = nullptr;

	delete bg;
	bg = nullptr;

	playerGold = STARTING_GOLD_AMT;
	waveLevel = 0;

	return true;
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
	SDL_Color textColor = {255, 255, 255, 255};
	Texture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}