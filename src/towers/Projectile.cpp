/**
 * @class Projectile
 * @brief Implementation of Projectile class, no implementation for area damage
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/Projectile.h>
#include <iostream>
#include <critter/Critter.h>

 /**
  * @brief Constructor with position, damage, and if projectile damages an area
  *
  * @param x Horizontal position using pixels
  * @param y Vertical position using pixels
  * @param damage Amount of health points to take from a critter
  * @param isArea true if projectile should deal damage in an area (not implemented)
  * @param speed speed of the Projectile
  * @param targettedCritter Critter that the Projectile will travel to
  * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
  * Damage is to be applied to critters, removing the same number of health from it
  * Area damage is not yet implemented
  * Uses a default ProjectileSize of 3
  */
Projectile::Projectile(float x, float y, int damage, bool isArea, int speed)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(3)
{
    targettedCritter = nullptr;
}

/**
 * @brief Constructor with position, damage, if projectile damages an area, and projectile size
 *
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param damage Amount of health points to take from a critter
 * @param isArea true if projectile should deal damage in an area (not implemented)
 * @param speed speed of the Projectile
 * @param projectileSize size of one side of a cube-shaped projectile
 * @param targettedCritter Critter that the Projectile will travel to
 * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
 * Damage is to be applied to critters, removing the same number of health from it
 * Area damage is not yet implemented
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int speed, int projectileSize)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(projectileSize), targettedCritter(targettedCritter)
{
    targettedCritter = nullptr;
}

/**
 * @brief Constructor with position, damage, and if projectile damages an area
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param damage Amount of health points to take from a critter
 * @param isArea true if projectile should deal damage in an area (not implemented)
 * @param speed speed of the Projectile
 * @param targettedCritter Critter that the Projectile will travel to
 * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
 * Damage is to be applied to critters, removing the same number of health from it
 * Area damage is not yet implemented
 * Uses a default ProjectileSize of 3
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int speed, const Critter* targettedCritter)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(3), targettedCritter(targettedCritter)
{
    
}

/**
 * @brief Constructor with position, damage, if projectile damages an area, and projectile size
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param damage Amount of health points to take from a critter
 * @param isArea true if projectile should deal damage in an area (not implemented)
 * @param speed speed of the Projectile
 * @param projectileSize size of one side of a cube-shaped projectile
 * @param targettedCritter Critter that the Projectile will travel to
 * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
 * Damage is to be applied to critters, removing the same number of health from it
 * Area damage is not yet implemented
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int speed, int projectileSize, const Critter* targettedCritter)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(projectileSize), targettedCritter(targettedCritter)
{
    
}

/**
 * @brief Accessor for damage
 * 
 * @return The damage the Projectile will do to a critter
 */
int Projectile::getDamage() 
{
    return damage;
}

/**
 * @brief Moves Projectile according to changes in x and y
 * 
 * @details adds the xSpeed and ySpeed values to their corresponding coordinates to move the Projectile along the map
 */
void Projectile::move()
{
    float unitDistanceX = 0;
    float unitDistanceY = 0;

    if (targettedCritter != nullptr && targettedCritter->isAlive())
    {
        SDL_FRect currentCellSize = Global::currentMap->getPixelPerCell();

        // projectile position with offset
        float halfProjectileSize = static_cast<float>(projectileSize) / 2;
        float posX = x + halfProjectileSize;
        float posY = y + halfProjectileSize;

        // critter position with offset
        float critterPosX = targettedCritter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize.w / 2;
        float critterPosY = targettedCritter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize.h / 2;

        // differences in position from tower to cannon
        float differenceX = critterPosX - posX;
        float differenceY = critterPosY - posY;

        float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

        // distance for projectile as a unit vector
        unitDistanceX = differenceX / distance;
        unitDistanceY = differenceY / distance;
    }
    
    x += speed * unitDistanceX;
    y += speed * unitDistanceY;
}

/**
 * @brief Generates Projectile
 * 
 * @details Represents a Projectile with a black square
 * Draws the square using SDL 
 */
void Projectile::generateProjectile() 
{
    SDL_FRect fillRect = { x, y, projectileSize, projectileSize };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

/**
 * @brief Checks if projectile is outside the map area
 * 
 * @details Check if projectile is either horizontally or vertically out of bounds
 * Checks both lower and upper bounds
 * Lower bounds being 0,
 * Upper bounds being the screen size in pixels
 * @return true if the Projectile is out of bounds
 * @return false if the Projectile is positioned correctly within the map 
 */
bool Projectile::isOutside()
{
    return ((x < 0 || x > Global::kScreenWidth) || (y < 0 || y > Global::kScreenHeight));
}

/**
 * @brief Checks if projectile is colliding with a critter
 * 
 * @param critterX Horizontal position of the critter 
 * @param critterY Vertical position of the critter
 * @details Check if projectile is colliding with the critter
 * Uses the size of the critter and an additional tolerance to account for positioning
 * As position of critter is offset to the top left corner
 * @return true if the Projectile is colliding with a critter 
 * @return false if the Projectile has not collided with critter
 */
bool Projectile::checkCollision() {
    //float tolerance = -4.0f;
    float tolerance = 0.0f;
    int critterX = targettedCritter->getPosition().x;
    int critterY = targettedCritter->getPosition().y;
    int critterSize = targettedCritter->getPosition().w;

    return 
        // checks both horizontal sides of the projectile
        x < critterX + critterSize + tolerance &&
        x + projectileSize > critterX - tolerance &&
        
        // checks both vertical sides of the projectile
        y < critterY + critterSize + tolerance &&
        y + projectileSize > critterY - tolerance;
}

Critter* Projectile::getTargettedCritter()
{
    return const_cast<Critter*>(targettedCritter);
}
