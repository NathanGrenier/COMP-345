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

class Projectile
{
    public:
        Projectile(float x, float y, int damage, bool isArea, int speed);
        Projectile(float x, float y, int damage, bool isArea, int speed, int projectileSize);
        Projectile(float x, float y, int damage, bool isArea, int speed, const Critter* targettedCritter);
        Projectile(float x, float y, int damage, bool isArea, int speed, int projectileSize, const Critter* targettedCritter);
        int getDamage();
        int getIsArea();

        void move();
        void generateProjectile();
        bool isOutside();

        bool checkCollision();

        Critter* getTargettedCritter();

    private:
        float x; /** @brief x position for projectile */
        float y; /** @brief y position for projectile */
        int damage;
        int projectileSize;
        bool isArea; /** @brief if projectile has area of effect, to be implemented */
        int speed;
        const Critter* targettedCritter;
};
