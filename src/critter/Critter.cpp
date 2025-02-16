#include <critter/Critter.h>
#include <cmath>
#include <critter/CritterGroup.h>

Critter::Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, SDL_FRect end)
    : level(level), speed(speed ), hitPoints(hitPoints), strength(strength), reward(reward),
    startPosition(start), exitPosition(end), isAtExit(false) {
    position = start;
}

void Critter::move(float deltaTime, const std::vector<Critter>& critters, float spacing) {
    // Move logic towards the current end position (exit)
    float directionX = 0.0f;
    float directionY = 0.0f;

    // Default movement towards the exit position
    if (exitPosition.x > position.x) {
        directionX = 1.0f;  // Move right
    }
    else if (exitPosition.x < position.x) {
        directionX = -1.0f;  // Move left
    }

    if (exitPosition.y > position.y) {
        directionY = 1.0f;  // Move down
    }
    else if (exitPosition.y < position.y) {
        directionY = -1.0f;  // Move up
    }

    // Move the critter based on the direction (exit position)
    position.x += directionX * speed * deltaTime;
    position.y += directionY * speed * deltaTime;

    // Check if the critter is at the exit position (within a small range)
    if (!isAtExit &&
        std::abs(position.x - exitPosition.x) <= 1.0f &&  // More precise check for X position
        std::abs(position.y - exitPosition.y) <= 1.0f) {  // More precise check for Y position
        isAtExit = true;  // Mark as at the exit
    }

    if (isAtExit) {
        hitPoints = 0;
    }
}

void Critter::takeDamage(int damage) {
    hitPoints -= damage;
}

bool Critter::isAlive() const {
    return hitPoints > 0;
}

void Critter::stealCoins(int& playerCoins) {
    playerCoins -= strength;  // Steal coins from the player based on the critter's strength
}

void Critter::render(SDL_Renderer* renderer) {
    // Render critter as a red rectangle
    SDL_FRect rect = { position.x, position.y, 20.0f, 20.0f };  // 20x20 size
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &rect);
}

// Dynamically update the critter's end position
void Critter::setEndPosition(SDL_FRect newEndPosition) {
    exitPosition = newEndPosition;  // Update the exit (end) position
}
