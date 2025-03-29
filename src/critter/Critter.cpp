#include <critter/Critter.h>
#include <cmath>
#include <critter/CritterGroup.h>
#include <util/Vector2D.h>
#include <Global.h>

Critter::Critter(int level, SDL_FRect start, Map* map)
	: level(level), position(start), isAtExit(false), map(map),
	currentFrame(0), animationTimer(0.0f), frameTime(0.1f), distanceTravelled(0.0f),
	isHurt(false), damageTimer(0), redTintAlpha(255), greenTintAlpha(255), blueTintAlpha(255), currentState(State::ALIVE) {
	currentRenderRect = {};
	if (map != nullptr)
	{
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

		// Set initial direction
		if (flowX == 1)
			currentDirection = Direction::RIGHT;
		else if (flowX == -1)
			currentDirection = Direction::LEFT;
		else if (flowY == 1)
			currentDirection = Direction::DOWN;
		else if (flowY == -1)
			currentDirection = Direction::UP;
	} else
	{
		targetCellX = -1;
		targetCellY = -1;
		targetPos = { 0.0f, 0.0f };
		currentDirection = Direction::DOWN;
	}
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
 * @brief Moves the critter towards the exit position while avoiding collisions with other critters.
 *
 * The critter moves horizontally or vertically depending on its current position relative to the exit.
 * If a collision with another critter is detected, the critter will adjust its movement to avoid overlap.
 *
 * @param deltaTime Time elapsed since the last frame.
 * @param critters A vector of all other critters to check for collisions.
 */

void Critter::move(float deltaTime, const std::vector<Critter*> critters) {
	if (isAtExit || map == nullptr || currentState != State::ALIVE)
		return;

	SDL_FRect renderRect = map->getCurrentRenderRect();
	Vector2D critterCenter(position.x + position.w / 2.0f, position.y + position.h / 2.0f);
	Vector2D targetVec(targetPos.x, targetPos.y);
	Vector2D direction = targetVec - critterCenter;
	float distanceToTarget = direction.magnitude();


	if (distanceToTarget > 0) {
		direction = direction.normalize();

		float scaledSpeed = getSpeed() / Critter::SPEED_PER_CELL * deltaTime * Global::currentMap->getPixelPerCell();

		float deltaX = direction.x * scaledSpeed;
		float deltaY = direction.y * scaledSpeed;

		float addedDistance = static_cast<float>(sqrt(pow(deltaX, 2) + pow(deltaY, 2)));
		distanceTravelled += addedDistance;

		position = { position.x + deltaX, position.y + deltaY, position.w, position.h };
	}

	// Check if the critter has reached or passed the target
	bool reachedTarget = false;
	if (distanceToTarget < 1.0f)
	{
		reachedTarget = true;
	} else
	{
		Vector2D toTarget = targetVec - critterCenter;
		if (currentDirection == Direction::RIGHT && toTarget.x <= 0)
		{
			reachedTarget = true;
		} else if (currentDirection == Direction::LEFT && toTarget.x >= 0)
		{
			reachedTarget = true;
		} else if (currentDirection == Direction::DOWN && toTarget.y <= 0)
		{
			reachedTarget = true;
		} else if (currentDirection == Direction::UP && toTarget.y >= 0)
		{
			reachedTarget = true;
		}
	}

	if (reachedTarget)
	{
		int currentCellX = targetCellX;
		int currentCellY = targetCellY;
		Vector2D flowDir = map->getFlowNormal(currentCellX, currentCellY);
		if (flowDir.x == 0 && flowDir.y == 0)
		{
			setAtExit(true);
			notify();
		} else
		{
			int flowX = static_cast<int>(flowDir.x);
			int flowY = static_cast<int>(flowDir.y);
			targetCellX = currentCellX + flowX;
			targetCellY = currentCellY + flowY;
			targetPos = map->getCellCenter(targetCellX, targetCellY, renderRect);
			if (flowX == 1)
				currentDirection = Direction::RIGHT;
			else if (flowX == -1)
				currentDirection = Direction::LEFT;
			else if (flowY == 1)
				currentDirection = Direction::DOWN;
			else if (flowY == -1)
				currentDirection = Direction::UP;
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
	setHitPoints(getHitPoints() - damage);
	isHurt = true;
	notify();
	damageTimer = SDL_GetTicks();
	if (getHitPoints() <= 0 && currentState == State::ALIVE)
	{
		currentState = State::DYING;
		currentFrame = 0; // Reset frame for death animation
		animationTimer = 0.0f;
	}
}

void Critter::update(float deltaTime) {
	if (currentState == State::ALIVE)
	{
		if (isHurt)
		{
			Uint64 elapsedTime = SDL_GetTicks() - damageTimer;
			if (elapsedTime < damageDuration)
			{
				redTintAlpha = maxRedAlpha * (elapsedTime / float(damageDuration));
			} else
			{
				redTintAlpha = maxRedAlpha - maxRedAlpha * ((elapsedTime - damageDuration) / float(damageDuration));
				if (redTintAlpha <= 0.0f)
				{
					isHurt = false;
					redTintAlpha = 255;
					greenTintAlpha = 255;
					blueTintAlpha = 255;
				}
			}
		} else
		{
			redTintAlpha = 255;
			greenTintAlpha = 255;
			blueTintAlpha = 255;
		}

		// Walking animation logic
		animationTimer += deltaTime;
		if (animationTimer >= frameTime)
		{
			std::vector<SDL_FRect>* currentFrames = nullptr;
			switch (currentDirection)
			{
				case Direction::UP:
					currentFrames = &animationFramesWalkUp;
					break;
				case Direction::DOWN:
					currentFrames = &animationFramesWalkDown;
					break;
				case Direction::LEFT:
				case Direction::RIGHT:
					currentFrames = &animationFramesWalkSide;
					break;
			}
			if (currentFrames && !currentFrames->empty())
			{
				currentFrame = (currentFrame + 1) % currentFrames->size();
			}
			animationTimer = 0.0f;
		}
	} else if (currentState == State::DYING)
	{
		// Death animation logic
		animationTimer += deltaTime;
		if (animationTimer >= frameTime)
		{
			std::vector<SDL_FRect>* currentFrames = nullptr;
			switch (currentDirection)
			{
				case Direction::UP:
					currentFrames = &animationFramesDeathUp;
					break;
				case Direction::DOWN:
					currentFrames = &animationFramesDeathDown;
					break;
				case Direction::LEFT:
				case Direction::RIGHT:
					currentFrames = &animationFramesDeathSide;
					break;
			}
			if (currentFrames && !currentFrames->empty())
			{
				currentFrame++;
				if (currentFrame >= static_cast<int>(currentFrames->size()))
				{
					currentState = State::DEAD;				  // Transition to DEAD after animation completes
					currentFrame = currentFrames->size() - 1; // Hold on last frame
				}
			}
			animationTimer = 0.0f;
		}
	}

	// Apply color tint to all textures
	textureWalkUp.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
	textureWalkDown.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
	textureWalkSide.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);

	// TODO: check if we need to set the texture's color for death animations
	//textureDeathUp.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
	//textureDeathDown.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
	//textureDeathSide.setColor((Uint8)redTintAlpha, (Uint8)greenTintAlpha, (Uint8)blueTintAlpha);
}

bool Critter::isAlive() const {
	return this->currentState == State::ALIVE;
}

bool Critter::isDying() const {
	return this->currentState == State::DYING;
}

/**
 * @brief Checks if the critter has reached the exit.
 * @return True if the critter has reached the exit, false otherwise.
 */
bool Critter::atExit() const {
	return isAtExit;
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
		mouseYPos >= currentRenderRect.y && mouseYPos <= currentRenderRect.y + currentRenderRect.h)
	{
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
 * @brief Renders the critter on the screen.
 *
 * The critter is rendered as a red rectangle, and its health bar is shown above it.
 *
 * @param renderer The SDL_Renderer used to draw the critter.
 */
void Critter::render() {
	float currentCellSize = Global::currentMap->getPixelPerCell();
	currentRenderRect = { position.x, position.y, currentCellSize * CRITTER_WIDTH_SCALE, currentCellSize * CRITTER_HEIGHT_SCALE };

	Texture* currentTexture = nullptr;
	std::vector<SDL_FRect>* currentFrames = nullptr;
	SDL_FlipMode flip = SDL_FLIP_NONE;

	if (currentState == State::ALIVE)
	{
		// Render walking animation
		switch (currentDirection)
		{
			case Direction::UP:
				currentTexture = &textureWalkUp;
				currentFrames = &animationFramesWalkUp;
				break;
			case Direction::DOWN:
				currentTexture = &textureWalkDown;
				currentFrames = &animationFramesWalkDown;
				break;
			case Direction::RIGHT:
				currentTexture = &textureWalkSide;
				currentFrames = &animationFramesWalkSide;
				flip = SDL_FLIP_HORIZONTAL;
				break;
			case Direction::LEFT:
				currentTexture = &textureWalkSide;
				currentFrames = &animationFramesWalkSide;
				break;
		}
	} else if (currentState == State::DYING || currentState == State::DEAD)
	{
		// Render death animation
		switch (currentDirection)
		{
			case Direction::UP:
				currentTexture = &textureDeathUp;
				currentFrames = &animationFramesDeathUp;
				break;
			case Direction::DOWN:
				currentTexture = &textureDeathDown;
				currentFrames = &animationFramesDeathDown;
				break;
			case Direction::RIGHT:
				currentTexture = &textureDeathSide;
				currentFrames = &animationFramesDeathSide;
				flip = SDL_FLIP_HORIZONTAL;
				break;
			case Direction::LEFT:
				currentTexture = &textureDeathSide;
				currentFrames = &animationFramesDeathSide;
				break;
		}
	}

	if (currentTexture && currentFrames && !currentFrames->empty())
	{
		SDL_FRect destRect = { currentRenderRect.x, currentRenderRect.y, currentRenderRect.w, currentRenderRect.h };
		SDL_FRect* clip = &(*currentFrames)[currentFrame];

		currentTexture->render(destRect.x, destRect.y, clip, destRect.w, destRect.h, 0.0, CRITTER_SPRITE_MODIFIER, nullptr, flip);
	}

	// Render health bar only if ALIVE
	if (currentState == State::ALIVE)
	{
		SDL_FRect healthBarRect = { position.x, position.y - (currentCellSize * CRITTER_HEALTHBAR_PADDING),
								   currentCellSize * CRITTER_WIDTH_SCALE, currentCellSize * CRITTER_HEALTHBAR_HEIGHT };
		SDL_FRect greenBar = healthBarRect;
		SDL_FRect redBar = healthBarRect;

		float healthPercentage = static_cast<float>(getHitPoints()) / getMaxHitPoints();
		greenBar.w = healthBarRect.w * healthPercentage;
		redBar.x = greenBar.x + greenBar.w;
		redBar.w = healthBarRect.w - greenBar.w;

		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Green
		SDL_RenderFillRect(gRenderer, &greenBar);

		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Red
		SDL_RenderFillRect(gRenderer, &redBar);
	}
}

/**
 * @brief Reduces the player's gold based on the critter's strength.
 *
 * This function allows the critter to "steal" gold from the player.
 *
 * @param playerGold The player's current gold amount, which will be reduced by the critter's strength.
 */
void Critter::stealGold(int& playerGold) const {
	playerGold -= getHitPoints();
}
