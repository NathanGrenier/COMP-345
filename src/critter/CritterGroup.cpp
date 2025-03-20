#include <critter/CritterGroup.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ui/LTexture.h>
#include <iostream>
#include <Global.h>
#include <critter/CritterObserver.h>

/**
 * @class CritterGroup
 * @brief Represents a group of critters that spawn and move towards the exit.
 *
 * Manages waves of critters and tracks player gold.
 *
 * @author Nirav Patel
 */

 /**
  * @brief Constructs a new CritterGroup object.
  *
  * @param waveLevel Reference to the current wave level.
  * @param playerGold Reference to the player's current gold amount.
  * @param startPosition The starting position of the critters.
  * @param endPosition The end (exit) position for the critters.
  * @param map Pointer to the game map.
  * @param detailDisplay Reference to the DetailAttributeDisplay for critter details.
  */
CritterGroup::CritterGroup(int& waveLevel, int& playerGold, SDL_FRect startPosition, SDL_FRect endPosition, Map* map, DetailAttributeDisplay& detailDisplay)
	: waveLevel(waveLevel), playerGold(playerGold), startPosition(startPosition), endPosition(endPosition), map(map), detailDisplay(detailDisplay) {}


/**
 * @brief Destructor for CritterGroup.
 *
 * Resets all necessary attributes to allow for a new wave of critters to be generated starting from wave 1.
 */
CritterGroup::~CritterGroup() {
	for (Critter* critter : critters) {
		delete critter;
	}

	critters.clear();
}

/**
 * @brief Generates critters for the current wave, adding them to the critter group.
 *
 * Critters are generated sequentially at the start position. The function checks if
 * the start position is clear before spawning new critters, ensuring no overlap.
 *
 * @param deltaTime Time elapsed since the last frame.
 */
void CritterGroup::generateCritters(float deltaTime) {
	const float spacing = 100.0f;  // Minimum distance between critters
	const float generationDelay = 5.0f;  // Delay before generating the next critter

	// Update time elapsed
	timeElapsed += deltaTime;

	if (timeElapsed >= generationDelay && critterIndex < waveLevel * 10) {
		int level = waveLevel;
		float speed = 75.0f;
		float hitPoints = 20 + level * 2.0f;
		int strength = level * 2;
		int reward = level * 10;

		float currentCellSize = Global::currentMap->getPixelPerCell();

		SDL_FRect spawnCenter = {
			startPosition.x + (startPosition.w / 2.0f) - (currentCellSize * Critter::CRITTER_WIDTH_SCALE / 2.0f),  // Adjust for half of critter width
			startPosition.y + (startPosition.h / 2.0f) - (currentCellSize * Critter::CRITTER_HEIGHT_SCALE / 2.0f),  // Adjust for half of critter height
			currentCellSize * Critter::CRITTER_WIDTH_SCALE,
			currentCellSize * Critter::CRITTER_HEIGHT_SCALE
		};

		// Check if the start position is clear (no existing critters overlap)
		bool canSpawn = true;
		for (Critter* critter : critters) {
			SDL_FRect critterPos = critter->getPosition(); // Use -> to access methods
			float distanceX = std::abs(spawnCenter.x - critterPos.x);
			float distanceY = std::abs(spawnCenter.y - critterPos.y);

			if (distanceX < spacing && distanceY < spacing) {
				canSpawn = false;
				break;  // Stop checking further since overlap is found
			}
		}

		// Only spawn if there's no overlap
		if (canSpawn) {
			// Dynamically allocate a new Critter and push the pointer into the list
			Critter* newCritter = new Critter(level, speed, hitPoints, strength, reward, spawnCenter, map);
			critters.push_back(newCritter);

			aliveCritters++;
			crittersSpawned++;

			// Attach new critter to the DetailAttributeDisplay observer
			newCritter->attach(detailDisplay.getCritterObserver());

			critterIndex++;
			timeElapsed = 0.0f;
		}

	}
}

void CritterGroup::handleEvent(SDL_Event& e)
{
	// Check if clicking on towers or critters
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{

		for (int i = 0; i < critters.size(); i++)
		{
			Critter* critter = critters[i];
			if (critter->isClicked())
			{
				detailDisplay.selectCritter(critter);
				critter->notify();
				break;
			}
		}
	}
}

/**
 * @brief Updates the state of critters, including their movement and handling of waves.
 *
 * If the wave is not in progress, it counts down to the next wave. When the wave starts,
 * it updates the critters, checking for their death, movement, and gold rewards.
 *
 * @param deltaTime Time elapsed since the last frame.
 */
void CritterGroup::update(float deltaTime) {
	generateCritters(0.16f);

	if (!waveInProgress) {
		waveCountdown -= deltaTime;
		if (waveCountdown <= 0.0f) {
			waveInProgress = true;
			waveLevel++;
		}
		return;
	}

	for (auto it = critters.begin(); it != critters.end(); ) {
		Critter* critter = *it;

		critter->update();

		if (critter->atExit()) {
			critter->stealGold(playerGold);  // Take gold when reaching exit
			critter->detach(detailDisplay.getCritterObserver()); // Detach observer
			it = critters.erase(it);  // Erase critter and advance iterator
			--aliveCritters;
		}
		else if (!critter->isAlive()) {
			playerGold += critter->getReward();  // Reward player if critter is killed before exit
			critter->detach(detailDisplay.getCritterObserver()); // Detach observer
			it = critters.erase(it);  // Erase critter and advance iterator
			--aliveCritters;
		}
		else {
			critter->move(deltaTime, critters, 5.0f);  // Move if alive and not at exit
			++it;  // Advance to next critter
		}
	}


	if (aliveCritters == 0 && crittersSpawned >= waveLevel * 10) {
		waveInProgress = false;
		waveCountdown = 3.0f;
	}
}

/**
 * @brief Renders the critter group, including the number of alive critters, wave countdown, and the critters themselves.
 *
 * The function displays the number of alive critters and the countdown for the next wave on the screen.
 * It also renders each critter.
 *
 * @param renderer The SDL_Renderer used to render the game elements.
 */
void CritterGroup::render() {
	// Render critters
	for (auto& critter : critters) {
		critter->render();
	}

	// Render the alive critters count at the top-left
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture aliveText;
	std::string aliveMessage = "Living Critters: " + std::to_string(aliveCritters);
	aliveText.loadFromRenderedText(aliveMessage, textColor);
	aliveText.render(210, 10);  // Display text at the top-left

	// Render the countdown message for the next wave
	if (!waveInProgress) {
		LTexture countdownText;
		std::string countdownMessage = "Next wave in: " + std::to_string((int)std::ceil(waveCountdown));
		countdownText.loadFromRenderedText(countdownMessage, textColor);
		countdownText.render(210, 50);  // Display text at the top-center
	}
}
