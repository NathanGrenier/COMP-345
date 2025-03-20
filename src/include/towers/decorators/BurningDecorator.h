#pragma once

#include <towers/decorators/TowerDecorator.h>
#include <unordered_map>
#include <iostream>  // For debug statements

class BurningDecorator : public TowerDecorator {
public:
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

    ~BurningDecorator() {
        indicatorTexture.destroy();
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

        // Render the indicator texture (fire effect) over the towerPosition
        renderIndicator();
    }

private:
    Tower* wrappedTower;
    LTexture burningTexture;
    LTexture indicatorTexture;  // The new texture for the indicator (fire effect)
    SDL_FRect spriteClips[4];
    int currentFrame;
    float frameTime;
    float frameWidth, frameHeight;

    std::unordered_map<Critter*, float> burnTimers;  // Map to store the burn timer for each critter

    const float burnDuration;  // Duration for how long the burn effect lasts

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
};
