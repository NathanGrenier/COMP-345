#include <critter/CritterGroup.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ui/LTexture.h>
#include <iostream>
#include <Global.h>

/**
 * @class CritterGroup
 * @brief Represents a group of critters that spawn and move towards the exit.
 *
 * Manages waves of critters and tracks player gold.
 *
 * @author Nirav Patel
 */
CritterGroup::CritterGroup(int& waveLevel, int& playerGold, SDL_FRect startPosition, SDL_FRect endPosition, Map* map)
	: waveLevel(waveLevel), playerGold(playerGold), startPosition(startPosition), endPosition(endPosition), map(map) {}

/**
 * @brief Destructor for CritterGroup.
 *
 * Resets all necessary attributes to allow for a new wave of critters to be generated starting from wave 1.
 */
CritterGroup::~CritterGroup() {
	critters.clear();
}

/**
 * @brief Generates critters for the current wave, adding them to the critter group.
 *
 * Critters are generated sequentially at the start position. The function checks if
 * the start position is clear before spawning new critters, ensuring no overlap.
 *
 * @param startPosition The starting position of the critters.
 * @param endPosition The end (exit) position for the critters.
 * @param deltaTime Time elapsed since the last frame.
 *
 */
void CritterGroup::generateCritters(float deltaTime) {
	const float spacing = 100.0f;  // Minimum distance between critters
	const float generationDelay = 5.0f;  // Delay before generating the next critter

	// Update time elapsed
	timeElapsed += deltaTime;

	if (timeElapsed >= generationDelay && critterIndex < waveLevel * 10) {
		int level = waveLevel;
		float speed = 50.0f;
		int hitPoints = 100 + level * 20;
		int strength = level * 5;
		int reward = level * 10;

		SDL_FRect currentCellSize = Global::currentMap->getPixelPerCell();

		SDL_FRect spawnCenter = {
			startPosition.x + (startPosition.w / 2.0f) - (currentCellSize.w * Critter::CRITTER_WIDTH_SCALE / 2.0f),  // Adjust for half of critter width
			startPosition.y + (startPosition.h / 2.0f) - (currentCellSize.h * Critter::CRITTER_HEIGHT_SCALE / 2.0f),  // Adjust for half of critter height
			currentCellSize.w * Critter::CRITTER_WIDTH_SCALE,
			currentCellSize.h * Critter::CRITTER_HEIGHT_SCALE
		};

		// Check if the start position is clear (no existing critters overlap)
		bool canSpawn = true;
		for (Critter& critter : critters) {
			SDL_FRect critterPos = critter.getPosition();
			float distanceX = std::abs(spawnCenter.x - critterPos.x);
			float distanceY = std::abs(spawnCenter.y - critterPos.y);

			if (distanceX < spacing && distanceY < spacing) {
				canSpawn = false;
				break;  // Stop checking further since overlap is found
			}
		}

		// Only spawn if there's no overlap
		if (canSpawn) {
			critters.emplace_back(level, speed, hitPoints, strength, reward, spawnCenter, map);
			critterIndex++;
			timeElapsed = 0.0f;
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
 * @param newEndPosition The updated end position (exit) for critters.
 *
 */
void CritterGroup::update(float deltaTime) {
	if (!waveInProgress) {
		waveCountdown -= deltaTime;
		if (waveCountdown <= 0.0f) {
			waveInProgress = true;
			waveLevel++;
		}
		return;
	}

	int aliveCritters = 0;

	for (auto it = critters.begin(); it != critters.end(); ) {
		if (!it->isAlive()) {
			if (!it->atExit()) {
				playerGold += it->getReward();
			} else {
				it->stealGold(playerGold);
			}
			it = critters.erase(it);
			++crittersSpawned;
		} else {
			++aliveCritters;
			it->move(deltaTime, critters, 5.0f);
			++it;
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
void CritterGroup::render(SDL_Renderer* renderer) {
	int aliveCrittersCount = 0;

	// Count the number of alive critters
	for (auto& critter : critters) {
		if (critter.isAlive()) {
			++aliveCrittersCount;
		}
	}

	// Render the alive critters count at the top-left or another position on the screen
	SDL_Color textColor = { 0, 0, 0, 255 };
	LTexture aliveText;
	std::string aliveMessage = "Living Critters: " + std::to_string(aliveCrittersCount);
	aliveText.loadFromRenderedText(aliveMessage, textColor);
	aliveText.render(10, 50);  // Display text at the top-left (you can adjust position)

	// Render the countdown message for the next wave
	if (!waveInProgress) {
		LTexture countdownText;
		std::string countdownMessage = "Next wave in: " + std::to_string((int)std::ceil(waveCountdown));
		countdownText.loadFromRenderedText(countdownMessage, textColor);
		countdownText.render(10, 90);  // Display text at the top-center
	}

	// Render critters
	for (auto& critter : critters) {
		critter.render(renderer);
	}
}

/**
 * @brief Attacks all critters in the group with the specified damage.
 *
 * This function reduces the hit points of all critters in the group by the specified damage amount.
 *
 * @param damage The amount of damage to deal to each critter.
 */
void CritterGroup::attackTowers(int damage) {
	for (auto& critter : critters) {
		critter.takeDamage(damage);
	}
}
