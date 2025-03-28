/**
 * @file Powerup.cpp
 * @brief Implementation of the Powerup class
 * @author Nirav Patel
 * @date 2025-03-19
 *
 * @details This file contains the implementation of the methods defined in the Powerup class. The Powerup class manages
 * power-up behavior such as rendering, detecting clicks, and applying animations like frame cycling and bobbing effects.
 */

#include <towers/powerups/Powerup.h>

 /**
  * @brief Constructs a Powerup object with a specified position and texture.
  *
  * Initializes the position of the powerup and sets up frame cycling and bobbing effects.
  * Loads the texture for the powerup from the given path.
  *
  * @param position The position of the powerup in the game world.
  * @param texturePath The file path to the texture image of the powerup.
  */
Powerup::Powerup(SDL_FRect position, std::string texturePath)
	: position(position), currentFrame(0), frameTime(0), frameDuration(1.0f / 60.0f),
	bobbingSpeed(1.0f), bobbingHeight(0.05f), bobbingOffset(0.0f),
	lifetime(0.5f), elapsedTime(0.0f), flickerStartTime(0.4f), isVisible(true) {
	powerupTexture.loadFromFile(texturePath);
}

/**
 * @brief Checks if the powerup was clicked by the player.
 *
 * Determines if the mouse cursor is within the bounds of the powerup object.
 *
 * @param mouseX The X-coordinate of the mouse cursor.
 * @param mouseY The Y-coordinate of the mouse cursor.
 * @return true if the mouse cursor is within the powerup's boundaries, false otherwise.
 */
bool Powerup::isClicked(float mouseX, float mouseY) const {
	return (mouseX >= position.x && mouseX <= position.x + position.w &&
			mouseY >= position.y && mouseY <= position.y + position.h);
}

/**
 * @brief Updates the powerup's state, including frame cycling and bobbing effect.
 *
 * The method updates the frame for animation based on the time elapsed (`deltaTime`).
 * It also applies a sinusoidal vertical bobbing effect to simulate floating movement.
 * The bobbing effect is only applied if the powerup is not being dragged.
 *
 * @param deltaTime The time elapsed since the last frame, used for updating the frame and bobbing effect.
 */
void Powerup::update(float deltaTime) {
	elapsedTime += deltaTime;

	// Flickering effect before despawning
	if (elapsedTime >= flickerStartTime && elapsedTime < lifetime) {
		// Toggle visibility every 0.2 seconds
		isVisible = (static_cast<int>(elapsedTime * 100) % 2 == 0);
	} else if (elapsedTime >= lifetime) {
		markForDespawn = true;
		return;
	}

	// Update the frame time for animation
	frameTime += deltaTime;
	if (frameTime >= frameDuration) {
		frameTime = 0.0f;
		currentFrame = (currentFrame + 1) % 4;  // Cycle through frames 0 to 3
	}

	// Update bobbing effect: Sinusoidal movement for vertical bobbing
	bobbingOffset = sin(SDL_GetTicks() / 1000.0f * bobbingSpeed) * bobbingHeight;
	if (!isDragged) {
		position.y += bobbingOffset;
	}
}


/**
 * @brief Renders the powerup on the screen.
 *
 * The method calculates the appropriate width and height for the powerup based on the aspect ratio of the texture
 * and ensures that the powerup fits within its defined position. It then renders the powerup texture at the specified position.
 */
void Powerup::render() {
	if (!isVisible) return;

	float textureWidth = powerupTexture.getWidth() / 4;
	float textureHeight = powerupTexture.getHeight();

	float aspectRatio = textureWidth / textureHeight;
	float newWidth = position.w;
	float newHeight = position.w / aspectRatio;

	if (newHeight > position.h) {
		newHeight = position.h;
		newWidth = position.h * aspectRatio;
	}

	SDL_FRect srcRect = { currentFrame * textureWidth, 0, textureWidth, textureHeight };
	SDL_FRect destRect = { position.x, position.y, newWidth, newHeight };

	powerupTexture.render(destRect.x, destRect.y, &srcRect, destRect.w, destRect.h, 0.0f, SIZE_SCALE_FACTOR);
}

