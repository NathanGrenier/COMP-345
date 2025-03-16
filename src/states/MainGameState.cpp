#include <states/MainGameState.h>
#include <critter/CritterGroup.h>
#include <Global.h>
#include <util/TextureLoader.h>
#include <towers/CannonTower.h>
#include <towers/RapidFireTower.h>
#include <towers/StandardTower.h>
#include <ui/DetailButton.h>
#include <towers/TowerGroup.h>


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
	if (Global::currentMap == nullptr || Global::currentMap->getName().empty())
	{
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	std::cout << "Global::currentMap name: " + Global::currentMap->name << std::endl;

	map = new Map(*Global::currentMap);
	map->setFlowFieldVisibility(false);
	map->setCurrentRenderRect(Global::mapViewRect);

	for (auto& cell : map->cells)
	{
		if (cell.isWall)
		{
			map->wallCellDict[cell] = false;
		}
	}

	detailDisplay = DetailAttributeDisplay();
	bool success = detailDisplay.initializeComponents();

	critterGroup = new CritterGroup(waveLevel, playerGold, map->getSpawnerPos(Global::mapViewRect), map->getTargetPos(Global::mapViewRect), map, detailDisplay);
	towerGroup = new TowerGroup(playerGold, map, detailDisplay);

	return true;
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

	if (critterGroup != nullptr)
	{
		delete critterGroup;
		critterGroup = nullptr;
	}

	if (towerGroup != nullptr)
	{
		delete towerGroup;
		towerGroup = nullptr;
	}

	if (map != nullptr)
	{
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
void MainGameState::handleEvent(SDL_Event &e)
{
	// handles hovering, clicking of buttons
	towerGroup->handleEvent(e);
	detailDisplay.handleButtonEvents(e);

	bool buttonClick = false;

	// Check if clicking on towers or critters
	if (!buttonClick && e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		bool critterClick = false;
		bool towerClick = false;

		// If not clicking on a tower, check for critters
		if (!towerClick)
		{
			for (int i = 0; i < critterGroup->getCritters().size(); i++)
			{
				Critter &critter = critterGroup->getCritters()[i];
				if (critter.isClicked())
				{
					detailDisplay.selectCritter(&critter);
					critter.notify();
					critterClick = true;
					break;
				}
			}
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
	map->drawOnTargetRect(gRenderer, map->getCurrentRenderRect());

	detailDisplay.render();

	critterGroup->render(gRenderer);

	towerGroup->render();

	// Render player gold
	renderText("Gold: " + std::to_string(playerGold), 10.0f, 10.0f);

	// Render wave level
	std::string waveText = "Wave: " + std::to_string(waveLevel);
	float waveX = (Global::kScreenWidth - (waveText.length() * 10.0f)) / 2.0f;
	renderText(waveText, waveX, 10.0f);
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
	SDL_Color textColor = {0, 0, 0, 255};
	LTexture textTexture;
	textTexture.loadFromRenderedText(text, textColor);
	textTexture.render(x, y);
}