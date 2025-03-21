#pragma once

#include <towers/decorators/TowerDecorator.h>
#include <unordered_map>
#include <iostream>

/**
 * @file SlowingDecorator.h
 * @brief The header file for the SlowingDecorator class, which applies a slow effect to towers.
 * @author Nirav Patel
 * @date 2025-03-19
 *
 * @details This file contains the declaration of the SlowingDecorator class, which
 *          enhances a tower's functionality by applying a slow effect to any critter
 *          it damages. The slow effect reduces the critter's speed for a specified duration.
 */

 /**
  * @class SlowingDecorator
  * @brief A decorator that applies a slow effect to a tower.
  *
  * The SlowingDecorator adds a slow effect to a tower, causing critters that are
  * damaged by the tower to slow down for a specified duration. The effect is rendered
  * as an ice animation, and the critters' speed is reduced as long as the slow timer is active.
  */
class SlowingDecorator : public TowerDecorator {
public:
    /**
     * @brief Constructs a SlowingDecorator that adds a slow effect to the tower.
     *
     * Initializes the slowing effect, including loading the ice sprite sheet and
     * the indicator texture. It also calculates the dimensions of each animation frame.
     *
     * @param tower The tower to decorate with the slow effect.
     * @param towerPosition The position of the tower in the game world.
     * @param indicatorPath The file path for the ice indicator texture.
     */
    SlowingDecorator(Tower* tower, SDL_FRect towerPosition, std::string indicatorPath)
        : TowerDecorator(tower, towerPosition, indicatorPath), wrappedTower(tower), currentFrame(0), frameTime(0),
        slowDuration(4.0f), slowFactor(0.5f) {
        iceTexture.loadFromFile("assets/tower/ice-4f.png");
        indicatorTexture.loadFromFile(indicatorPath);

        // Calculate individual frame dimensions for the ice sprite sheet
        frameWidth = iceTexture.getWidth() / 4.0f;
        frameHeight = iceTexture.getHeight();

        // Populate sprite clips for animation
        for (int i = 0; i < 4; ++i) {
            spriteClips[i] = { i * frameWidth, 0.f, frameWidth, frameHeight };
        }
    }

    /**
     * @brief Destructor for SlowingDecorator.
     *
     * Cleans up any resources, including the indicator texture.
     */
    ~SlowingDecorator() {
        indicatorTexture.destroy();
    }

    /**
     * @brief Applies a projectile attack to a critter.
     *
     * This method is called when the tower shoots a projectile. It also applies
     * the slow effect to the critter if it is alive and damaged, and updates the
     * animation frames for the slow effect.
     *
     * @param critter The critter being attacked by the tower.
     */
    void shootProjectile(Critter* critter) override {
        wrappedTower->shootProjectile(critter);

        updateAnimation(0.008f);

        if (critter == nullptr) {
            return;
        }

        // Only apply the slow effect if the critter is damaged and alive
        if (critter->isDamaged() && critter->isAlive()) {

            // Apply slow effect only if the critter is not already slowed
            if (slowTimers.find(critter) == slowTimers.end()) {
                // Store the original speed and apply the slow factor
                originalSpeeds[critter] = critter->getSpeed();
                critter->setSpeed(critter->getSpeed() * slowFactor);

                // Initialize the slow timer
                slowTimers[critter] = slowDuration;  // Set the slow duration
            }
        }
        else {
            // If the critter is not alive or not damaged, reset its slow effect
            if (slowTimers.find(critter) != slowTimers.end()) {
                critter->setSpeed(originalSpeeds[critter]);  // Restore original speed
                slowTimers.erase(critter);
                originalSpeeds.erase(critter);
            }
        }
    }

    /**
     * @brief Updates the slow effect animation and timers.
     *
     * This method updates the animation frames based on the delta time and
     * updates the slow timers for each critter with an active slow effect.
     *
     * @param deltaTime The time difference between frames, used for smooth animation.
     */
    void updateAnimation(float deltaTime) {
        frameTime += deltaTime;

        // Adjust this time interval for animation speed
        const float frameDuration = 0.05f;

        if (frameTime >= frameDuration) {
            currentFrame = (currentFrame + 1) % 4;
            frameTime = 0;
        }

        // Update slow timers and handle each critter's individual slow effect
        for (auto it = slowTimers.begin(); it != slowTimers.end();) {
            Critter* critter = it->first;
            float& timer = it->second;

            if (critter->isAlive() && timer > 0) {
                timer -= deltaTime;  // Decrease the slow timer over time
                ++it;
            }
            else {
                // Reset the slow effect after timer expires
                critter->setSpeed(originalSpeeds[critter]);  // Restore the original speed
                originalSpeeds.erase(critter);  // Remove the critter from the original speeds map
                it = slowTimers.erase(it);  // Remove the critter from the slowTimers map
            }
        }
    }

    /**
     * @brief Renders the slow effect for all critters with an active slow effect.
     *
     * This method renders the ice animation for each critter that has an
     * active slow timer. It visually represents the slow effect.
     */
    void renderSlowEffect() {
        // Render the ice effect for each critter with an active slow effect
        for (auto& pair : slowTimers) {

            Critter* critter = pair.first;

            // Get the current position of the critter
            SDL_FRect critterRect = critter->getCurrentRenderRect();

            // Only render the ice effect if the slow timer is active
            if (slowTimers.find(critter) != slowTimers.end()) {
                iceTexture.render(critterRect.x, critterRect.y, &spriteClips[currentFrame], critterRect.w, critterRect.h, 0.0f, EFFECT_RENDER_SIZE_MULTIPLIER);
            }
        }
    }

    /**
     * @brief Renders the tower along with its slow effect.
     *
     * This method renders the base tower and overlays the slow effect and
     * indicator texture.
     */
    void render() override {
        wrappedTower->render();  // Render the base tower
        renderSlowEffect();  // Render the slowing effect (ice effect)

        // Render the indicator texture over the towerPosition (this is the key part)
        renderIndicator();
    }

    /**
     * @brief Renders the indicator above the tower.
     *
     * This method adjusts the size and position of the indicator texture based on
     * the tower's position, ensuring it is centered and scaled appropriately.
     */
    void renderIndicator() {
        // Calculate the aspect ratio for the indicator texture
        float indicatorWidth = indicatorTexture.getWidth() / 6.0f;
        float indicatorHeight = indicatorTexture.getHeight();

        float aspectRatio = indicatorWidth / indicatorHeight;

        // Calculate the new width and height to fit within the towerPosition, preserving the aspect ratio
        float newWidth = towerPosition.w;
        float newHeight = towerPosition.w / aspectRatio;

        // If the new height is greater than the available height, adjust based on the height instead
        if (newHeight > towerPosition.h) {
            newHeight = towerPosition.h;
            newWidth = towerPosition.h * aspectRatio;
        }

        float centerX = towerPosition.x + towerPosition.w / 2.0f;
        float centerY = towerPosition.y + towerPosition.h / 2.0f;

        // Create the destination rectangle for the indicator texture
        SDL_FRect srcRect = { currentFrame * indicatorWidth, 0, indicatorWidth, indicatorHeight };
        SDL_FRect indicatorDestRect = {
            centerX - newWidth / 2.0f,  // Adjust the X position to center it
            centerY - newHeight / 2.0f, // Adjust the Y position to center it
            newWidth,
            newHeight
        };

        // Render the indicator texture
        indicatorTexture.render(indicatorDestRect.x, indicatorDestRect.y, &srcRect, indicatorDestRect.w, indicatorDestRect.h);
    }

private:
    Tower* wrappedTower; /**< The tower that this decorator enhances */
    LTexture iceTexture; /**< The texture for the slow effect animation (ice effect) */
    LTexture indicatorTexture; /**< The texture for the ice indicator above the tower */
    SDL_FRect spriteClips[4]; /**< The frames for the slow effect animation */
    int currentFrame; /**< The current frame in the slow animation */
    float frameTime; /**< Time accumulator for animation frame updates */
    float frameWidth, frameHeight; /**< Dimensions of each animation frame */

    std::unordered_map<Critter*, float> slowTimers; /**< A map of critters with active slow timers */
    std::unordered_map<Critter*, float> originalSpeeds; /**< A map of critters with their original speeds */

    const float slowDuration; /**< The duration the slow effect lasts for each critter */
    const float slowFactor;   /**< The factor by which the critter's speed is reduced */
};
