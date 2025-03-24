/**
 * @file CritterGroup.h
 * @brief The header file for the CritterGroup class.
 * @author Nirav Patel
 * @date 2025-02-21
 */
#pragma once

#include <vector>
#include "Critter.h"
#include <ui/DetailAttributeDisplay.h>

enum class CritterType {
	NORMAL,
	FAST,
	TANK,
};

struct CritterSpawnInfo {
	CritterType type;
	int count;
};

struct WaveConfig {
	std::vector<CritterSpawnInfo> critterGroup;
	float spawnDelay;
	float spacing;
};

class CritterGroup {
public:
	// Constructor with parameters
	CritterGroup(int& waveLevel, int& playerGold, SDL_FRect startPosition, SDL_FRect endPosition, Map* map, DetailAttributeDisplay* detailDisplay, bool endlessMode);
	~CritterGroup();

	void generateCritters(float deltaTime);
	void update(float deltaTime);  // Update all critters in the group
	void render();  // Render all critters in the group
	void handleEvent(SDL_Event& e);
	std::vector<Critter*> getCritters() { return critters; }
	bool isGameWon() const;
private:
	int& waveLevel;
	int& playerGold;
	std::vector<Critter*> critters;
	Map* map;
	SDL_FRect startPosition;
	SDL_FRect endPosition;
	bool endlessMode;
	bool gameWon = false;

	DetailAttributeDisplay* detailDisplay;
	int crittersSpawned = 0;
	int aliveCritters = 0;
	bool waveInProgress = true;
	float waveCountdown = 3.0f;

	std::vector<WaveConfig> waveConfigs;
	std::vector<CritterType> currentWaveCrittersToSpawn;
	float currentSpawnDelay;
	float currentSpacing;
	float timeSinceLastSpawn = 0.0f;
};
