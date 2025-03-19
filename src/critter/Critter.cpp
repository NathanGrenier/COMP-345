#include <critter/Critter.h>
#include <cmath>
#include <critter/CritterGroup.h>
#include <util/Vector2D.h>
#include <Global.h>

/**
 * @brief Constructs a new Critter object with specified attributes and initializes its position and target.
 *
 * @param level The level of the critter.
 * @param speed The speed at which the critter moves.
 * @param hitPoints The initial hit points of the critter.
 * @param strength The strength of the critter, used for stealing gold.
 * @param reward The reward given when the critter is defeated.
 * @param start The starting position of the critter.
 * @param map Pointer to the game map.
 *
 * @details Initializes the critter's position, target, and subscribes to the map's flow field.
 *          If the map is null, it sets safe default values for the target position.
 */
Critter::Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, Map* map)
	: level(level), speed(speed), hitPoints(hitPoints), strength(strength), reward(reward),
	position(start), isAtExit(false), maxHitPoints(hitPoints), map(map) {

	currentRenderRect = {};

	if (map != nullptr) {
		map->subscribe(this);

		SDL_FRect renderRect = map->getCurrentRenderRect();
		float centerX = position.x + position.w / 2.0f;
		float centerY = position.y + position.h / 2.0f;
		auto [initialCellX, initialCellY] = map->getCellFromPosition(centerX, centerY, renderRect);
		Vector2D flowDir = map->getFlowNormal(initialCellX, initialCellY);
		int flowX = static_cast<int>(flowDir.x);
		int flowY = static_cast<int>(flowDir.y);
		targetCellX = initialCellX + flowX;
		targetCellY = initialCellY + flowY;
		targetPos = map->getCellCenter(targetCellX, targetCellY, renderRect);
	} else {
		// Handle null map case with safe defaults
		targetCellX = -1;
		targetCellY = -1;
		targetPos = { 0.0f, 0.0f };
	}

	// Initialize target
	critterTexture.loadFromFile("assets/critter/avatar/rat.png");
}

Critter::~Critter() {
	critterTexture.destroy();
}

/**
 * @brief Callback function for when the flow field changes.
 *
 * This function is currently empty as critters poll the map each move,
 * so no immediate action is needed.
 */
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

/**
 * @brief Checks if two rectangles intersect.
 *
 * @param a The first rectangle.
 * @param b The second rectangle.
 * @return True if the rectangles intersect, false otherwise.
 */
static bool rectanglesIntersect(const SDL_FRect& a, const SDL_FRect& b) {
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
void Critter::move(float deltaTime, const std::vector<Critter*> critters, float spacing) {
	if (isAtExit) return;

	if (map == nullptr) return;

	SDL_FRect renderRect = map->getCurrentRenderRect();

	// Calculate critter's center as Vector2D
	Vector2D critterCenter(position.x + position.w / 2.0f, position.y + position.h / 2.0f);
	// Convert targetPos (SDL_FPoint) to Vector2D
	Vector2D targetVec(targetPos.x, targetPos.y);

	// Calculate direction
	Vector2D direction = targetVec - critterCenter;
	float distanceToTarget = direction.magnitude();
	if (distanceToTarget > 0) {
		direction = direction.normalize();
		float deltaX = direction.x * speed * deltaTime;
		float deltaY = direction.y * speed * deltaTime;
		SDL_FRect nextPosition = { position.x + deltaX, position.y + deltaY, position.w, position.h };

		// Collision check
		bool collisionDetected = false;
		for (const Critter* other : critters) {
			if (other == this) continue; // Compare pointers correctly
			if (rectanglesIntersect(nextPosition, other->getPosition())) { // Use . to access methods
				collisionDetected = true;
				break;
			}
		}

		if (!collisionDetected) {
			position = nextPosition;
		}
	}

	// Check if target is reached
	distanceToTarget = (targetVec - critterCenter).magnitude();
	if (distanceToTarget < 1.0f) { // Tolerance of 1 pixel
		// Update to next target
		int currentCellX = targetCellX;
		int currentCellY = targetCellY;
		Vector2D flowDir = map->getFlowNormal(currentCellX, currentCellY);
		if (flowDir.x == 0 && flowDir.y == 0) {
			setAtExit(true);
			notify();
		} else {
			int flowX = static_cast<int>(flowDir.x);
			int flowY = static_cast<int>(flowDir.y);
			targetCellX = currentCellX + flowX;
			targetCellY = currentCellY + flowY;
			targetPos = map->getCellCenter(targetCellX, targetCellY, renderRect);
		}
	}
}

/**
 * @brief Reduces the critter's hit points by the specified damage amount.
 *
 * Makes the health bar visible for a short time after taking damage.
 *
 * @param damage The amount of damage to apply.
 */
void Critter::takeDamage(float damage) {
	hitPoints -= damage;
	isHurt = true;
	notify();
	damageTimer = SDL_GetTicks();
}

void Critter::update() {
	if (isHurt) {
		// Get the time passed since the damage was taken
		Uint64 elapsedTime = SDL_GetTicks() - damageTimer;

		// Gradually increase the red component and adjust green/blue
		if (elapsedTime < damageDuration) {
			// Fade in (increase the red component)
			redTintAlpha = maxRedAlpha * (elapsedTime / float(damageDuration));
		}
		else {
			redTintAlpha = maxRedAlpha - maxRedAlpha * ((elapsedTime - damageDuration) / float(damageDuration));

			if (redTintAlpha <= 0.0f) {
				isHurt = false;
				redTintAlpha = 255;
				greenTintAlpha = 255;
				blueTintAlpha = 255;
			}
		}

		// Apply the red, green, and blue tint using setColor
		critterTexture.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
	}
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
bool Critter::atExit() const {
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
* @brief Checks if the critter is clicked based on mouse position.
*
* @param mouseX The x-coordinate of the mouse.
* @param mouseY The y-coordinate of the mouse.
* @return true if the critter was clicked; false otherwise.
*/
bool Critter::isClicked() const {
	// Get the current mouse position
	float mouseXPos, mouseYPos;
	SDL_GetMouseState(&mouseXPos, &mouseYPos);

	// Checking if the mouse position is inside the critter's rectangle
	if (mouseXPos >= currentRenderRect.x && mouseXPos <= currentRenderRect.x + currentRenderRect.w &&
		mouseYPos >= currentRenderRect.y && mouseYPos <= currentRenderRect.y + currentRenderRect.h) {
		return true;
	}
	return false;
}

/**
 * @brief Sets the critter's exit status.
 *
 * Marks the critter as having reached the exit and reduces its hit points to 0.
 *
 * @param con Boolean indicating if the critter is at the exit.
 */
void Critter::setAtExit(bool con) {
	isAtExit = con;
}

/**
 * @brief Gets the speed of the critter.
 * @return The speed of the critter.
 */
float Critter::getSpeed() const  {
	return speed;
}

/**
 * @brief Sets the speed of the critter.
 * @param newSpeed The new speed value.
 */
void Critter::setSpeed(float newSpeed) {
	speed = newSpeed;
}

/**
 * @brief Renders the critter on the screen.
 *
 * The critter is rendered as a red rectangle, and its health bar is shown above it.
 *
 * @param renderer The SDL_Renderer used to draw the critter.
 */
void Critter::render() {
	float currentCellSize = Global::currentMap->getPixelPerCell();

	currentRenderRect = { position.x, position.y, currentCellSize * CRITTER_WIDTH_SCALE, currentCellSize * CRITTER_WIDTH_SCALE };

	critterTexture.render(currentRenderRect.x, currentRenderRect.y, nullptr, currentRenderRect.w, currentRenderRect.h);

	// Health bar positioning and size (above the critter)
	SDL_FRect healthBarRect = { position.x, position.y - (currentCellSize * CRITTER_HEALTHBAR_PADDING),  currentCellSize * CRITTER_WIDTH_SCALE, currentCellSize * CRITTER_HEALTHBAR_HEIGHT };  // Just above the critter
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
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);  // Green
	SDL_RenderFillRect(gRenderer, &greenBar);

	// Set color for the red part (remaining health)
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);  // Red
	SDL_RenderFillRect(gRenderer, &redBar);
}

/**
 * @brief Reduces the player's gold based on the critter's strength.
 *
 * This function allows the critter to "steal" gold from the player.
 *
 * @param playerGold The player's current gold amount, which will be reduced by the critter's strength.
 */
void Critter::stealGold(int& playerGold) const {
	playerGold -= strength;
}
