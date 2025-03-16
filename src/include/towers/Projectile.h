/**
 * @file Projectile.h
 * @brief Projectile fired from towers, damaging critters
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class that provides a visual indicator for a tower firing
 * Projectiles of different speeds and sizes are fired from towers
 * Damages critter when collision is detected
 * isArea still needs to be implemented for damage in an area
 */

#pragma once

#include <critter/Critter.h>
#include <ui/LTexture.h>

class Projectile
{
    public:
        Projectile(float x, float y, int damage, bool isArea, int rotationAngle, float xSpeed, float ySpeed, std::string texturePath);
        Projectile(float x, float y, int damage, bool isArea, int projectileSize, int rotationAngle, float xSpeed, float ySpeed, std::string texturePath);
        int getDamage();
        int getIsArea();

        void move(float multiplier);
        void generateProjectile();
        bool isOutside();

        bool checkCollision(Critter& critter);
        void updateAnimation(float deltaTime);

        void destroy();
    private:
        float x; /** @brief x position for projectile */
        float y; /** @brief y position for projectile */
        int damage;
        int projectileSize;
        bool isArea; /** @brief if projectile has area of effect, to be implemented */

        LTexture projectileTexture;
        SDL_FRect currentRenderRect;

        int currentFrame = 0;   // Current frame index
        int frameCount = 4;     // Total number of frames in sprite sheet

        float frameTimer = 0.0f;    // Timer to track when to switch frame
        float frameDuration = 0.1f; // Duration of each frame (0.1s -> 10 fps)

        int rotationAngle;
        float xSpeed;
        float ySpeed;
};
