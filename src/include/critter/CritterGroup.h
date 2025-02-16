#pragma once

#include <vector>
#include "Critter.h"

class CritterGroup {
public:
    // Constructor with parameters
    CritterGroup(int waveLevel, SDL_FRect startPosition, SDL_FRect endPosition);

    void generateCritters(SDL_FRect startPosition, SDL_FRect endPosition, float deltaTime);
    void update(float deltaTime, SDL_FRect newEndPosition);  // Update all critters in the group
    void render(SDL_Renderer* renderer);  // Render all critters in the group
    void attackTowers(int damage);  // Apply damage to critters from towers
    void handleExit(int& playerCoins);  // Handle critters that reach the exit

    const std::vector<Critter>& getCritters() const { return critters; }
    int waveLevel;
private:
    std::vector<Critter> critters;
    SDL_FRect startPosition;
    SDL_FRect endPosition;
    float spawnTimer = 0.0f;
    const float spawnInterval = 1.0f;
    int crittersSpawned = 0;
    bool waveInProgress = true;
    float waveCountdown = 3.0f;
};
