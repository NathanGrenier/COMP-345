#pragma once

#include <towers/decorators/TowerDecorator.h>
#include <unordered_map>

/**
 * @file BurningDecorator.h
 * @brief The header file for the BurningDecorator class, which applies a burn effect to towers.
 * @author Nirav Patel
 * @date 2025-03-19
 *
 * @details This file contains the declaration of the BurningDecorator class, which
 *          adds a burn effect to a tower. Critters that are damaged by the tower
 *          will burn over time, losing health until the burn effect expires.
 */

 /**
  * @class BurningDecorator
  * @brief A decorator that applies a burn effect to a tower.
  *
  * The BurningDecorator class enhances a tower's functionality by applying a burn
  * effect to critters it damages. This burn effect reduces the critter's health
  * gradually over a specified duration and also animates the effect with a fire texture.
  */
class BurningDecorator : public TowerDecorator {
public:
    /**
     * @brief Constructs a BurningDecorator that adds a burn effect to the tower.
     *
     * Initializes the burn effect, including loading the fire sprite sheet and
     * the indicator texture. It also calculates the dimensions of each animation frame.
     *
     * @param tower The tower to decorate with the burn effect.
     * @param towerPosition The position of the tower in the game world.
     * @param indicatorPath The file path for the fire indicator texture.
     */
    BurningDecorator(Tower* tower, SDL_FRect towerPosition, std::string indicatorPath)
        : TowerDecorator(tower, towerPosition, indicatorPath), wrappedTower(tower), currentFrame(0), frameTime(0),
        burnDuration(1.0f) {  // Initialize burn timer and burn duration
        burningTexture.loadFromFile("assets/tower/fire-4f.png");
        indicatorTexture.loadFromFile(indicatorPath);  // Load indicator texture

        // Calculate individual frame dimensions for the fire sprite sheet
        frameWidth = burningTexture.getWidth() / 4.0f;
        frameHeight = burningTexture.getHeight();

        // Populate sprite clips for animation
        for (int i = 0; i < 4; ++i) {
            spriteClips[i] = { i * frameWidth, 0.f, frameWidth, frameHeight };
        }
    }

    /**
     * @brief Destructor for BurningDecorator.
     *
     * Cleans up any resources, including the indicator texture.
     */
    ~BurningDecorator() {
        indicatorTexture.destroy();
    }

    /**
     * @brief Applies a projectile attack to a critter.
     *
     * This method is called when the tower shoots a projectile. It also applies
     * the burn effect to the critter if it is alive and damaged, and updates the
     * animation frames for the burn effect.
     *
     * @param critter The critter being attacked by the tower.
     */
    void shootProjectile(Critter* critter) override {
        wrappedTower->shootProjectile(critter);

        updateAnimation(0.008f);  // Update animation frame based on delta time

        if (critter == nullptr) {
            return;
        }

        // Only apply the burn effect if the critter is damaged and alive
        if (critter->isDamaged() && critter->isAlive()) {
            // Apply burn effect only if the critter is not already burning
            if (burnTimers.find(critter) == burnTimers.end()) {
                // Reduce the critter's hit points gradually

                // Initialize the burn timer
                burnTimers[critter] = burnDuration;  // Set the burn duration
            }
        }
        else {
            // If the critter is not alive or not damaged, reset its burn effect
            if (burnTimers.find(critter) != burnTimers.end() && !critter->isAlive()) {
                burnTimers.erase(critter);
            }
        }
    }

    /**
     * @brief Updates the burn effect animation and timers.
     *
     * This method updates the animation frames for the burn effect and handles
     * the countdown for each critter's burn timer.
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

        // Update burn timers and handle each critter's burn effect
        for (auto it = burnTimers.begin(); it != burnTimers.end();) {
            Critter* critter = it->first;
            float& timer = it->second;

            if (critter->isAlive() && timer > 0) {
                timer -= deltaTime;  // Decrease the burn timer over time
                ++it;
            }
            else {
                // Reset the burn effect after timer expires
                it = burnTimers.erase(it);  // Remove the critter from the burnTimers map
            }
        }
    }

    /**
     * @brief Renders the burning effect for all critters with an active burn effect.
     *
     * This method renders the fire animation on each critter that has an
     * active burn effect and reduces the critter's health over time.
     */
    void renderBurningEffect() {
        // Render the burning effect for each critter with an active burn effect
        for (auto& pair : burnTimers) {
            Critter* critter = pair.first;

            // Get the burn position of the critter
            SDL_FRect burnRect = critter->getCurrentRenderRect();

            // Only render the burning effect if the burn timer is active
            if (burnTimers.find(critter) != burnTimers.end()) {
                critter->setHitPoints(critter->getHitPoints() - 1);

                burningTexture.render(burnRect.x, burnRect.y, &spriteClips[currentFrame], burnRect.w, burnRect.h);
            }
        }
    }

    /**
     * @brief Renders the tower along with its burning effect.
     *
     * This method renders the base tower and overlays the burn effect and
     * indicator texture.
     */
    void render() override {
        wrappedTower->render();  // Render the base tower

        renderBurningEffect();  // Render the burning effect

        // Render the indicator texture (fire effect) over the towerPosition
        renderIndicator();
    }

    /**
     * @brief Renders the fire indicator above the tower.
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
    LTexture burningTexture; /**< The texture for the burn effect animation (fire effect) */
    LTexture indicatorTexture; /**< The texture for the fire indicator above the tower */
    SDL_FRect spriteClips[4]; /**< The frames for the burn effect animation */
    int currentFrame; /**< The current frame in the burn animation */
    float frameTime; /**< Time accumulator for animation frame updates */
    float frameWidth, frameHeight; /**< Dimensions of each animation frame */

    std::unordered_map<Critter*, float> burnTimers; /**< A map of critters with active burn timers */

    const float burnDuration; /**< The duration the burn effect lasts for each critter */
};
