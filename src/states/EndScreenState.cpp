/** @class EndScreenState
 *  @brief Implementation of the EndScreenState.
 *
 *
 *  @author Denmar Ermitano
 */

#include <states/EndScreenState.h>
#include <states/MainGameState.h>
#include <states/MapSelectState.h>
#include <Global.h>
#include <ui/LButton.h>
#include <ui/Texture.h>
#include <string>
#include <critter/Critter.h>
#include <states/ExitState.h>

 /// Static instance of the EndScreenState
EndScreenState EndScreenState::sEndScreenState;
const std::string EndScreenState::DEFAULT_KILLER_CRITTER_STR = "NONE";

/**
 * @brief Gets the singleton instance of EndScreenState.
 *
 * @return Pointer to the EndScreenState instance.
 */
EndScreenState* EndScreenState::get() {
	return &sEndScreenState;
}

/**
 * @brief Enters the title state, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool EndScreenState::enter() {
	buttons = new LButton[BUTTON_COUNT];
	statLabels = new Texture[STAT_COUNT];
	stats = new Texture[STAT_COUNT];

	bg = new ParallaxBackground();
	std::srand(std::time(0));

	for (int i = 0; i < Global::numberOfProps; ++i) {
		float randomSpeed = 5.0f + std::rand() % 11;
		bg->addLayer(randomSpeed, Global::kScreenHeight);
	}

	bool success = true;
	SDL_Color textColor{ 0xFF, 0xFF, 0xFF, 0xFF };
	

	std::string titlePath;

	// Initialize buttons
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		buttons[i] = LButton();
	}

	for (int i = 0; i < STAT_COUNT; ++i) {
		stats[i] = Texture();
		statLabels[i] = Texture();
	}

	statLabels[0].loadFromRenderedText("Killed by:", textColor);
	statLabels[1].loadFromRenderedText("Wave:", textColor);
	statLabels[2].loadFromRenderedText("Towers Bought:", textColor);
	statLabels[3].loadFromRenderedText("Critters Killed:", textColor);

	stats[1].loadFromRenderedText(std::to_string(wave), textColor);
	stats[2].loadFromRenderedText(std::to_string(towersBought), textColor);
	stats[3].loadFromRenderedText(std::to_string(crittersKilled), textColor);

	if (!killedBy.compare(EndScreenState::DEFAULT_KILLER_CRITTER_STR))
	{
		stats[0].loadFromRenderedText("None", textColor);
		buttons[0].loadFromFile("ui/PlayAgain.png");
		titlePath = "ui/YouWin.png";
	}
	else
	{
		stats[0].loadFromRenderedText(killedBy, textColor);
		buttons[0].loadFromFile("ui/Retry.png");
		titlePath = "ui/GameOver.png";
	}

	if (!(success &= messageTexture.loadFromFile(titlePath))) {
		printf("Failed to render title text!\n");
	}

	buttons[1].loadFromFile("ui/MapSelect.png");
	buttons[2].loadFromFile("ui/Quit.png");

	float buttonHeight = (2.0f / 3.0f) * Global::kScreenHeight;

	// Set button positions centered on the screen
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		buttons[i].setSizeWithAspectRatio(0, EndScreenState::END_SCREEN_BUTTON_HEIGHT);
		buttons[i].setPosition((Global::kScreenWidth - buttons[i].kButtonWidth) / 2, buttonHeight);
		
		buttonHeight += buttons[i].kButtonHeight + EndScreenState::END_SCREEN_UI_SPACING;
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
bool EndScreenState::exit()
{
	/*for (int i = 0; i < BUTTON_COUNT; ++i) {
		buttons[i].destroy();
	}

	for (int i = 0; i < STAT_COUNT; ++i) {
		stats[i].destroy();
	}*/

	delete bg;
	bg = nullptr;

	delete[] stats;
	stats = nullptr;

	delete[] buttons;
	buttons = nullptr;

	return true;
}

/**
 * @brief Handles user input and button clicks.
 *
 * Processes mouse events and transitions to the appropriate game state when a button is clicked.
 *
 * @param e The SDL_Event object containing input data.
 */
void EndScreenState::handleEvent(SDL_Event& e) {
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		buttons[i].handleEvent(&e);

		// Check if a button is clicked
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
			if (buttons[i].isClicked()) {
				// Transition to the corresponding game state
				switch (i) {
				case 0:
					setNextState(MapSelectState::get());
					break;
				case 1:
					setNextState(MapSelectState::get());
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
 * @brief Updates the EndScreenState logic.
 *
 */
void EndScreenState::update() {
	bg->update(0.016f);
}

/**
 * @brief Renders the EndScreenState screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void EndScreenState::render() {
	bg->render();

	// Render title text centered at the top
	messageTexture.render(
			(Global::kScreenWidth - EndScreenState::END_SCREEN_TITLE_WIDTH) / 2.f, 
			EndScreenState::END_SCREEN_UI_SPACING, 
			nullptr, 
			EndScreenState::END_SCREEN_TITLE_WIDTH, -1);

	for (int i = 0; i < BUTTON_COUNT; ++i) {
		buttons[i].render();
	}


	float statsHeight = (1.0f / 2.0f) * Global::kScreenHeight;


	for (int i = 0; i < STAT_COUNT; ++i) {
		if (i == 0 && !killedBy.compare(EndScreenState::DEFAULT_KILLER_CRITTER_STR))
		{
			continue;
		}

		statLabels[i].render(
			Global::kScreenWidth / 2.0f - EndScreenState::END_SCREEN_STATS_WIDTH, 
			statsHeight, 
			nullptr, 
			0, EndScreenState::END_SCREEN_STATS_HEIGHT);
		
		stats[i].render(
			Global::kScreenWidth / 2.0f + EndScreenState::END_SCREEN_STATS_WIDTH,
			statsHeight,
			nullptr,
			0, EndScreenState::END_SCREEN_STATS_HEIGHT);


		statsHeight += statLabels[i].getHeight() * 0.9;
	}
}

/**
 * @brief Mutator for wave number
 * @param wave new wave number
 */
void EndScreenState::setWave(int wave)
{
	EndScreenState::wave = wave;
}

/**
 * @brief Mutator for number of towers bought
 * @param towersBought new number of towers that were bought
 */
void EndScreenState::setTowersBought(int towersBought)
{
	EndScreenState::towersBought = towersBought;
}

/**
 * @brief Mutator for number of critters killed
 * @param crittersKilled new number of critters killed
 */
void EndScreenState::setCrittersKilled(int crittersKilled)
{
	EndScreenState::crittersKilled = crittersKilled;
}

/**
 * @brief Mutator for killedBy
 * @param killedBy Critter that killed the player during game
 */
void EndScreenState::setKilledBy(std::string killedBy)
{
	EndScreenState::killedBy = killedBy;
}
