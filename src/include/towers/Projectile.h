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

class Projectile
{
    public:
        Projectile(float x, float y, int damage, bool isArea);
        Projectile(float x, float y, int damage, bool isArea, int projectileSize);
        int getDamage();
        int getIsArea();

        void move(float xSpeed, float ySpeed);
        void generateProjectile();
        bool isOutside();

        bool checkCollision(float critterX, float critterY);

    private:
        float x; /** x position for projectile */
        float y; /** y position for projectile */
        int damage;
        int projectileSize;
        bool isArea; /** if projectile has area of effect, to be implemented */
};
