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

class CritterGroup {
public:
	// Constructor with parameters
	CritterGroup(int& waveLevel, int& playerGold, SDL_FRect startPosition, SDL_FRect endPosition, Map* map, DetailAttributeDisplay& detailDisplay);
	~CritterGroup();

	void generateCritters(float deltaTime);
	void update(float deltaTime);  // Update all critters in the group
	void render();  // Render all critters in the group
	void handleEvent(SDL_Event& e);
	std::vector<Critter*> getCritters() { return critters; }
private:
	int& waveLevel;
	int& playerGold;
	std::vector<Critter*> critters;
	Map* map;
	SDL_FRect startPosition;
	SDL_FRect endPosition;

	DetailAttributeDisplay& detailDisplay;
	float spawnTimer = 0.0f;
	const float spawnInterval = 1.0f;
	int crittersSpawned = 0;
	int aliveCritters = 0;
	bool waveInProgress = true;
	float waveCountdown = 3.0f;
	int critterIndex = 0;
	float timeElapsed = 0.0f;
};
