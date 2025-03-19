#pragma once

#include <towers/decorators/TowerDecorator.h>
#include <unordered_map>
#include <iostream>

class SlowingDecorator : public TowerDecorator {
public:
    SlowingDecorator(Tower* tower)
        : TowerDecorator(tower), wrappedTower(tower), currentFrame(0), frameTime(0),
        slowDuration(4.0f), slowFactor(0.5f) {
        iceTexture.loadFromFile("assets/tower/ice-4f.png");

        // Calculate individual frame dimensions for the ice sprite sheet
        frameWidth = iceTexture.getWidth() / 4.0f;
        frameHeight = iceTexture.getHeight();

        // Populate sprite clips for animation
        for (int i = 0; i < 4; ++i) {
            spriteClips[i] = { i * frameWidth, 0.f, frameWidth, frameHeight };
        }
    }

    void shootProjectile(Critter* critter) override {
        wrappedTower->shootProjectile(critter);

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

    void renderSlowEffect() {
        // Render the ice effect for each critter with an active slow effect
        for (auto& pair : slowTimers) {

            Critter* critter = pair.first;

            // Get the current position of the critter
            SDL_FRect critterRect = critter->getCurrentRenderRect();

            // Only render the ice effect if the slow timer is active
            if (slowTimers.find(critter) != slowTimers.end()) {
                iceTexture.render(critterRect.x, critterRect.y, &spriteClips[currentFrame], critterRect.w, critterRect.h);
            }
        }
        updateAnimation(0.008f);
    }

    void render() override {
        wrappedTower->render();  // Render the base tower
        renderSlowEffect();  // Render the slowing effect (ice effect)
    }

private:
    Tower* wrappedTower;
    LTexture iceTexture;
    SDL_FRect spriteClips[4];
    int currentFrame;
    float frameTime;
    float frameWidth, frameHeight;

    std::unordered_map<Critter*, float> slowTimers;  // Map to store the slow timer for each critter
    std::unordered_map<Critter*, float> originalSpeeds;  // Map to store the original speed for each critter

    const float slowDuration; // Duration for how long the slow effect lasts
    const float slowFactor;   // Factor by which the critter's speed is reduced
};
