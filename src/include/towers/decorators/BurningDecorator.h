#pragma once

#include <towers/decorators/TowerDecorator.h>
#include <unordered_map>
#include <iostream>  // For debug statements

class BurningDecorator : public TowerDecorator {
public:
    BurningDecorator(Tower* tower)
        : TowerDecorator(tower), wrappedTower(tower), currentFrame(0), frameTime(0),
        burnDuration(1.0f) {  // Initialize burn timer and burn duration
        burningTexture.loadFromFile("assets/tower/fire-4f.png");

        // Calculate individual frame dimensions
        frameWidth = burningTexture.getWidth() / 4.0f;
        frameHeight = burningTexture.getHeight();

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

        // Only apply the burn effect if the critter is damaged and alive
        if (critter->isDamaged() && critter->isAlive()) {
            // Apply burn effect only if the critter is not already burning
            if (burnTimers.find(critter) == burnTimers.end()) {
                std::cout << "Applying burn effect to critter." << std::endl;
                // Reduce the critter's hit points gradually

                // Initialize the burn timer
                burnTimers[critter] = burnDuration;  // Set the burn duration
            }
        }
        else {
            // If the critter is not alive or not damaged, reset its burn effect
            if (burnTimers.find(critter) != burnTimers.end() && !critter->isAlive()) {
                std::cout << "Critter is not alive or not damaged. Resetting burn effect." << std::endl;
                burnTimers.erase(critter);
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

        // Update burn timers and handle each critter's burn effect
        for (auto it = burnTimers.begin(); it != burnTimers.end();) {
            Critter* critter = it->first;
            float& timer = it->second;

            if (critter->isAlive() && timer > 0) {
                timer -= deltaTime;  // Decrease the burn timer over time
                ++it;
            }
            else {
                std::cout << "Burn timer expired for critter. Resetting burn effect." << std::endl;
                // Reset the burn effect after timer expires
                it = burnTimers.erase(it);  // Remove the critter from the burnTimers map
            }
        }
    }

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

        updateAnimation(0.008f);  // Update animation frame based on delta time
    }

    void render() override {
        wrappedTower->render();  // Render the base tower

        renderBurningEffect();  // Render the burning effect
    }

private:
    Tower* wrappedTower;
    LTexture burningTexture;
    SDL_FRect spriteClips[4];
    int currentFrame;
    float frameTime;
    float frameWidth, frameHeight;

    std::unordered_map<Critter*, float> burnTimers;  // Map to store the burn timer for each critter

    const float burnDuration;  // Duration for how long the burn effect lasts
};
