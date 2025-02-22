#include <critter/Critter.h>
#include <cmath>
#include <critter/CritterGroup.h>

/**
 * @class Critter
 * @brief Represents a critter that moves within the game world and interacts with other critters.
 * 
 * @author Nirav Patel
 */
Critter::Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, SDL_FRect end)
    : level(level), speed(speed), hitPoints(hitPoints), strength(strength), reward(reward),
    startPosition(start), exitPosition(end), isAtExit(false), healthBarWidth(0.0f), healthBarVisible(false), maxHitPoints(hitPoints) {
    position = start;
}

/**
 * @brief Gets the current position of the critter.
 * @return SDL_FRect representing the critter's current position.
 */
SDL_FRect Critter::getPosition() {
    return position;
}

/**
 * @brief Moves the critter towards the exit position while avoiding collisions with other critters.
 *
 * The critter moves horizontally or vertically depending on its current position relative to the exit.
 * If a collision with another critter is detected, the critter will adjust its movement to avoid overlap.
 *
 * @param deltaTime Time elapsed since the last frame.
 * @param critters A vector of all other critters to check for collisions.
 * @param spacing The minimum spacing between critters to avoid collision.
 */
void Critter::move(float deltaTime, const std::vector<Critter>& critters, float spacing) {
    float directionX = 0.0f;
    float directionY = 0.0f;

    // Determine movement direction (strictly horizontal or vertical)
    bool moveX = std::abs(position.x - exitPosition.x) > 1.0f;
    bool moveY = std::abs(position.y - exitPosition.y) > 1.0f;

    if (moveX && !moveY) {
        directionX = (exitPosition.x > position.x) ? 1.0f : -1.0f;
    }
    else if (moveY) {
        directionY = (exitPosition.y > position.y) ? 1.0f : -1.0f;
    }

    // Predict next position
    float nextX = position.x + directionX * speed * deltaTime;
    float nextY = position.y + directionY * speed * deltaTime;

    // Check if the next position would cause an overlap
    bool collisionDetected = false;
    for (const Critter& other : critters) {
        if (&other == this) continue; // Skip self

        float distanceX = std::abs(nextX - other.position.x);
        float distanceY = std::abs(nextY - other.position.y);

        // If another critter is within the spacing range, mark as collision
        if (distanceX < spacing && distanceY < spacing) {
            collisionDetected = true;
            break;
        }
    }

    // If there is a collision, try an alternate movement (first priority Y, then X)
    if (collisionDetected) {
        if (directionY != 0.0f) {
            nextY = position.y; // Cancel Y movement
        }
        else if (directionX != 0.0f) {
            nextX = position.x; // Cancel X movement
        }
    }

    // Apply the movement if it's valid (no collision detected)
    position.x = nextX;
    position.y = nextY;
}

/**
 * @brief Reduces the critter's hit points by the specified damage amount.
 *
 * Makes the health bar visible for a short time after taking damage.
 *
 * @param damage The amount of damage to apply.
 */
void Critter::takeDamage(int damage) {
    hitPoints -= damage;

    // Make the health bar visible for a short time after taking damage
    healthBarVisible = true;
    healthBarWidth = 50.0f;
    healthBarTime = 0.5f;
}

/**
 * @brief Checks if the critter is still alive.
 * @return True if the critter's hit points are greater than 0, false otherwise.
 */
bool Critter::isAlive() const {
    return hitPoints > 0;
}

/**
 * @brief Checks if the critter has reached the exit.
 * @return True if the critter has reached the exit, false otherwise.
 */
bool Critter::atExit() {
    return isAtExit;
}

/**
 * @brief Sets the critter's hit points.
 *
 * @param hitPoints The new hit points value.
 */
void Critter::setHitPoints(int hitPoints) {
    hitPoints = hitPoints;
}

/**
 * @brief Sets the critter's exit status.
 *
 * Marks the critter as having reached the exit and reduces its hit points to 0.
 *
 * @param con Boolean indicating if the critter is at the exit.
 */
void Critter::setAtExit(bool con) {
    hitPoints = 0;
    isAtExit = con;
}

/**
 * @brief Renders the critter on the screen.
 *
 * The critter is rendered as a red rectangle, and its health bar is shown above it.
 *
 * @param renderer The SDL_Renderer used to draw the critter.
 */
void Critter::render(SDL_Renderer* renderer) {
    // Render critter as a red rectangle
    SDL_FRect critterRect = { position.x, position.y, 20.0f, 20.0f };  // 20x20 size
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &critterRect);

    // Health bar positioning and size (above the critter)
    SDL_FRect healthBarRect = { position.x, position.y - 10.0f, 20.0f, 5.0f };  // Just above the critter
    SDL_FRect greenBar = healthBarRect;  // For the green part (current health)
    SDL_FRect redBar = healthBarRect;  // For the red part (remaining health)

    // Calculate the width based on health percentage
    float healthPercentage = static_cast<float>(hitPoints) / maxHitPoints;

    // Green bar width (representing current health)
    greenBar.w = healthBarRect.w * healthPercentage;

    // Red bar width (representing the remaining health)
    redBar.x = greenBar.x + greenBar.w;  // Start where the green bar ends
    redBar.w = healthBarRect.w - greenBar.w;

    // Set color for the green part (current health)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green
    SDL_RenderFillRect(renderer, &greenBar);

    // Set color for the red part (remaining health)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
    SDL_RenderFillRect(renderer, &redBar);
}

/**
 * @brief Updates the critter's exit position.
 *
 * This function allows dynamic updating of the critter's target exit position.
 *
 * @param newEndPosition The new exit position for the critter.
 */
void Critter::setEndPosition(SDL_FRect newEndPosition) {
    exitPosition = newEndPosition;  // Update the exit (end) position
}

/**
 * @brief Gets the critter's current exit position.
 * @return SDL_FRect representing the critter's exit position.
 */
SDL_FRect Critter::getEndPosition() {
    return exitPosition;
}

/**
 * @brief Reduces the player's gold based on the critter's strength.
 *
 * This function allows the critter to "steal" gold from the player.
 *
 * @param playerGold The player's current gold amount, which will be reduced by the critter's strength.
 */
void Critter::stealGold(int& playerGold) {
    playerGold -= strength;
}
