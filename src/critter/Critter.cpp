#include <critter/Critter.h>
#include <cmath>
#include <critter/CritterGroup.h>
#include <util/Vector2D.h>
#include <Global.h>

/**
 * @class Critter
 * @brief Represents a critter that moves within the game world and interacts with other critters.
 *
 * @author Nirav Patel
 */
Critter::Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, Map* map)
	: level(level), speed(speed), hitPoints(hitPoints), strength(strength), reward(reward),
	position(start), isAtExit(false), healthBarWidth(0.0f), healthBarVisible(false), maxHitPoints(hitPoints), map(map) {

	if (map) {
		map->subscribe(this);
	}
}

Critter::~Critter() {
	if (map) {
		map->unsubscribe(this);
	}
}

void Critter::onFlowFieldChanged() {
	// Currently empty; critters poll the map each move, so no immediate action needed
}

/**
 * @brief Gets the current position of the critter.
 * @return SDL_FRect representing the critter's current position.
 */
SDL_FRect Critter::getPosition() const {
	return position;
}

bool rectanglesIntersect(const SDL_FRect& a, const SDL_FRect& b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
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
	SDL_FRect renderRect = map->getCurrentRenderRect();

	// Compute cell dimensions based on the rendered map size
	float cellWidth = renderRect.w / map->cellCountX;
	float cellHeight = renderRect.h / map->cellCountY;

	// Compute critter's center for accurate cell determination
	float centerX = position.x + position.w / 2.0f;
	float centerY = position.y + position.h / 2.0f;

	// Convert pixel position to cell coordinates
	int cellX = static_cast<int>((centerX - renderRect.x) / cellWidth);
	int cellY = static_cast<int>((centerY - renderRect.y) / cellHeight);

	Vector2D direction = map->getFlowNormal(cellX, cellY);

	// If there's no valid direction, don't move
	if (direction.x == 0 && direction.y == 0 && !isAtExit) {
		return;
	}

	float deltaX = direction.x * speed * deltaTime;
	float deltaY = direction.y * speed * deltaTime;

	SDL_FRect nextPosition = { position.x + deltaX, position.y + deltaY, position.w, position.h };


	bool collisionDetected = false;
	for (const Critter& other : critters) {
		if (&other == this) continue; // Skip self
		if (rectanglesIntersect(nextPosition, other.getPosition())) {
			collisionDetected = true;
			break;
		}
	}

	if (!collisionDetected) {
		position = nextPosition;
	}
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
 * @brief Reduces the player's gold based on the critter's strength.
 *
 * This function allows the critter to "steal" gold from the player.
 *
 * @param playerGold The player's current gold amount, which will be reduced by the critter's strength.
 */
void Critter::stealGold(int& playerGold) {
	playerGold -= strength;
}
