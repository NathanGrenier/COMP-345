#include <cstdlib>
#include <string>
#include <cmath>
#include <iostream>
#include <Global.h>
#include <ui/LTexture.h>
#include <critter/CritterObserver.h>
#include <critter/CritterGroup.h>
#include <critter/NormalCritter.h>
#include <critter/FastCritter.h>
#include <critter/TankCritter.h>
#include <critter/CritterFactory.h>
#include <states/EndScreenState.h>

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
CritterGroup::CritterGroup(int& waveLevel, int& playerGold, SDL_FRect startPosition, SDL_FRect endPosition, Map* map, DetailAttributeDisplay* detailDisplay, bool endlessMode)
	: waveLevel(waveLevel), playerGold(playerGold), startPosition(startPosition), endPosition(endPosition), map(map), detailDisplay(detailDisplay), endlessMode(endlessMode), currentSpawnDelay(0.0f), currentSpacing(0.0f) {
	NormalCritter::loadTextures();
	FastCritter::loadTextures();
	TankCritter::loadTextures();

	waveConfigs = {
		{ {{CritterType::NORMAL, 5}}, 1.0f, 50.0f },
		{ {{CritterType::FAST, 3}}, 0.5f, 60.0f },
		{ {{CritterType::TANK, 5}}, 0.5f, 75.0f },
		{ {{CritterType::NORMAL, 10}, {CritterType::TANK, 2}, {CritterType::FAST, 5}}, 0.75f, 25.0f }
	};
}

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

std::string critterTypeToString(CritterType type) {
	switch (type) {
		case CritterType::NORMAL: return "Normal";
		case CritterType::FAST: return "Fast";
		case CritterType::TANK: return "Tank";
		default: return "Normal";
	}
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
	if (currentWaveCrittersToSpawn.empty()) {
		return; // No more critters to spawn this wave
	}

	timeSinceLastSpawn += deltaTime;
	if (timeSinceLastSpawn >= currentSpawnDelay) {
		float currentCellSize = Global::currentMap->getPixelPerCell();
		SDL_FRect spawnCenter = {
			startPosition.x + (startPosition.w / 2.0f) - (currentCellSize * Critter::CRITTER_WIDTH_SCALE / 2.0f),
			startPosition.y + (startPosition.h / 2.0f) - (currentCellSize * Critter::CRITTER_HEIGHT_SCALE / 2.0f),
			currentCellSize * Critter::CRITTER_WIDTH_SCALE,
			currentCellSize * Critter::CRITTER_HEIGHT_SCALE
		};

		bool canSpawn = true;
		for (Critter* critter : critters) {
			SDL_FRect critterPos = critter->getPosition();
			float distanceX = std::abs(spawnCenter.x - critterPos.x);
			float distanceY = std::abs(spawnCenter.y - critterPos.y);
			if (distanceX < currentSpacing && distanceY < currentSpacing) {
				canSpawn = false;
				break;
			}
		}

		if (canSpawn) {
			CritterType type = currentWaveCrittersToSpawn.front();
			currentWaveCrittersToSpawn.erase(currentWaveCrittersToSpawn.begin());
			std::string typeStr = critterTypeToString(type);
			Critter* newCritter = CritterFactory::createCritterByType(typeStr, waveLevel, spawnCenter, map);
			critters.push_back(newCritter);
			aliveCritters++;
			crittersSpawned++;
			newCritter->attach(detailDisplay->getCritterObserver());
			timeSinceLastSpawn = 0.0f;
		}
	}
}


void CritterGroup::handleEvent(SDL_Event& e) {
	// Check if clicking on towers or critters
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
	{

		for (int i = 0; i < critters.size(); i++)
		{
			Critter* critter = critters[i];
			if (critter->isClicked())
			{
				detailDisplay->selectCritter(critter);
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
	if (isGameWon()) {
		return;
	}

	if (!waveInProgress) {
		waveCountdown -= deltaTime;
		if (waveCountdown <= 0.0f) {
			int nextWave = waveLevel + 1;
			if (endlessMode || nextWave <= static_cast<int>(waveConfigs.size())) {
				waveInProgress = true;
				waveLevel = nextWave;
				int configIndex = endlessMode ? (waveLevel - 1) % waveConfigs.size() : (waveLevel - 1);
				const WaveConfig& config = waveConfigs[configIndex];

				currentWaveCrittersToSpawn.clear();
				for (const auto& info : config.critterGroup) {
					currentWaveCrittersToSpawn.insert(
						currentWaveCrittersToSpawn.end(),
						info.count,
						info.type
					);
				}
				currentSpawnDelay = config.spawnDelay;
				currentSpacing = config.spacing;
				timeSinceLastSpawn = 0.0f;
			}
		}
		return;
	}

	generateCritters(deltaTime);

	for (auto it = critters.begin(); it != critters.end(); ) {
		Critter* critter = *it;
		critter->update(deltaTime);
		if (critter->atExit()) {
			if (critter->getHitPoints() > playerGold)
			{
				playerGold = 0;
				killerCritterType = critter->getType();
				gameLost = true;
			}
			else
			{
				critter->stealGold(playerGold);
			}

			critter->detach(detailDisplay->getCritterObserver());
			it = critters.erase(it);
			--aliveCritters;

		} else if (!critter->isAlive() && !critter->isDying()) {
			playerGold += critter->getReward();
			critter->detach(detailDisplay->getCritterObserver());
			it = critters.erase(it);
			--aliveCritters;

			++totalCrittersKilled;

		} else {
			critter->move(deltaTime, critters);
			++it;
		}
	}

	// End wave when all critters are spawned and none are alive
	if (aliveCritters == 0 && currentWaveCrittersToSpawn.empty()) {
		waveInProgress = false;
		// Check if this is the last wave in non-endless mode
		if (!endlessMode && waveLevel == static_cast<int>(waveConfigs.size())) {
			if (!gameLost)
			{
				gameWon = true;
			}
		} else {
			waveCountdown = 3.0f; // Set countdown for the next wave
		}
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
	SDL_Color textColor = { 255, 255, 255, 255 };
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

/**
 * @brief Accessor for isGameWon bool
 * @return true if game has been won, false if it has not (yet)
 */
bool CritterGroup::isGameWon() const {
	return gameWon;
}

/**
 * @brief Accessor for isGameLost bool
 * @return true if game has been lost, false if it has not (yet)
 */
bool CritterGroup::isGameLost() const {
	return gameLost;
}

/**
 * @brief Checks if game is currently finished
 * @return true if game has either been won or lost
 */
bool CritterGroup::isGameFinished() const {
	return gameWon || gameLost;
}

/**
 * @brief Accessor for total number of critters killed
 * @return the number of critters that have been killed
 */
int CritterGroup::getTotalCrittersKilled() const
{
	return totalCrittersKilled;
}

/**
 * @brief Accessor for total number of critters killed
 * @return the number of critters that have been killed
 */
std::string CritterGroup::getKillerCritterType() const
{
	return killerCritterType;
}