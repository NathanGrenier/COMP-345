#include <critter/CritterGroup.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ui/LTexture.h>
#include <iostream>

CritterGroup::CritterGroup(int waveLevel, SDL_FRect startPosition, SDL_FRect endPosition)
    : waveLevel(waveLevel) {
}

void CritterGroup::generateCritters(SDL_FRect startPosition, SDL_FRect endPosition, float deltaTime) {
    static int critterIndex = 0;  // Keeps track of which critter we're generating
    static float timeElapsed = 0.0f;  // Keeps track of the elapsed time for sequential generation

    // Define the spacing between critters
    const float spacing = 100.0f;  // Distance between critters in the line
    const float generationDelay = 5.0f;  // Delay between each critter's generation (1 second)

    // Check if enough time has passed to generate the next critter
    timeElapsed += deltaTime;
    if (timeElapsed >= generationDelay && critterIndex < waveLevel * 10) {
        int level = waveLevel;

        float speed = 50.0f;

        int hitPoints = 100 + level * 20;
        int strength = level * 5;
        int reward = level * 10;

        // Create critter with the current level's parameters and path info (start/end positions)
        critters.emplace_back(level, speed, hitPoints, strength, reward, startPosition, endPosition);

        // Increment the critter index and reset timeElapsed for the next critter
        critterIndex++;
        timeElapsed = 0.0f;
    }
}

void CritterGroup::update(float deltaTime, SDL_FRect newEndPosition) {
    // Debug: Print the status of the wave countdown
    std::cout << "Wave Countdown: " << waveCountdown << " seconds remaining." << std::endl;

    if (!waveInProgress) {
        waveCountdown -= deltaTime;
        // Debug: Print countdown state
        std::cout << "Wave countdown in progress, remaining time: " << waveCountdown << " seconds." << std::endl;

        if (waveCountdown <= 0.0f) {
            waveInProgress = true;
            waveLevel++;  // Increase wave level
            // Debug: Print wave level increase
            std::cout << "Wave started! New wave level: " << waveLevel << std::endl;
            generateCritters(newEndPosition, newEndPosition, 0.1f);
        }
        return; // Skip updating critters during countdown
    }

    int aliveCritters = 0;

    // Debug: Print the number of critters to update
    std::cout << "Updating critters. Total critters: " << critters.size() << std::endl;

    // Iterate through critters and update their status
    for (auto it = critters.begin(); it != critters.end(); ) {
        if (!it->isAlive()) {
            // Debug: Print if a critter is being erased
            std::cout << "Critter dead. Removing critter." << std::endl;
            it = critters.erase(it);  // Remove the critter if it's dead
            ++crittersSpawned;
        }
        else {
            ++aliveCritters;
            it->setEndPosition(newEndPosition);
            it->move(deltaTime, critters, 5.0f);

            // Debug: Print if the critter is alive and moving
            std::cout << "Critter alive. Moving critter. Alive critters: " << aliveCritters << std::endl;

            ++it;
        }
    }

    // Debug: Print the number of alive critters after loop
    std::cout << "Alive critters after update: " << aliveCritters << std::endl;

    // If no critters are alive and all have been spawned, start the countdown for the next wave
    if (aliveCritters == 0 && crittersSpawned >= waveLevel * 10) {
        waveInProgress = false;  // End current wave
        waveCountdown = 3.0f;  // Start countdown for new wave

        // Debug: Print when the wave ends and countdown starts
        std::cout << "All critters dead. Wave ended. Starting countdown for next wave." << std::endl;
    }
}



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
    std::string aliveMessage = "Alive Critters: " + std::to_string(aliveCrittersCount);
    aliveText.loadFromRenderedText(aliveMessage, textColor);
    aliveText.render(10, 50);  // Display text at the top-left (you can adjust position)

    // Render the countdown message for the next wave
    if (!waveInProgress) {
        LTexture countdownText;
        std::string countdownMessage = "Next wave in: " + std::to_string((int)std::ceil(waveCountdown));
        countdownText.loadFromRenderedText(countdownMessage, textColor);
        countdownText.render(10, 80);  // Display text at the top-center
    }

    // Render critters
    for (auto& critter : critters) {
        critter.render(renderer);
    }
}

void CritterGroup::attackTowers(int damage) {
    for (auto& critter : critters) {
        critter.takeDamage(damage);
    }
}

void CritterGroup::handleExit(int& playerCoins) {
    for (auto& critter : critters) {
        if (!critter.isAlive()) {
            playerCoins += critter.getReward();  // Reward player for killing critter
        }
        else if (critter.isAlive() && critter.getHitPoints() == 0) {
            critter.stealCoins(playerCoins);  // Critter stole coins
        }
    }
}
