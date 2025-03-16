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
 * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
 * Damage is to be applied to critters, removing the same number of health from it
 * Area damage is not yet implemented
 * Uses a default ProjectileSize of 3
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(3)
{
    projectileTexture.loadFromFile(texturePath);
}

/**
 * @brief Constructor with position, damage, if projectile damages an area, and projectile size
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param damage Amount of health points to take from a critter
 * @param isArea true if projectile should deal damage in an area (not implemented)
 * @param projectileSize size of one side of a cube-shaped projectile
 * @details Constructor for Projectile with x, y position, damage, and if damage is area damage
 * Damage is to be applied to critters, removing the same number of health from it
 * Area damage is not yet implemented
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize, std::string texturePath) 
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(projectileSize)
{
    projectileTexture.loadFromFile(texturePath);
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
 * @param xSpeed Pixels to move the projectile horizontally
 * @param ySpeed Pixels to move the projectile vertically
 * @details adds the xSpeed and ySpeed values to their corresponding coordinates to move the Projectile along the map
 */
void Projectile::move(float xSpeed, float ySpeed)
{
    x += xSpeed;
    y += ySpeed;
}

/**
 * @brief Generates Projectile
 * 
 * @details Represents a Projectile with a black square
 * Draws the square using SDL 
 */
void Projectile::generateProjectile(float rotationAngle)
{
    // Define the sprite clips for each frame (horizontal sprite sheet)
    float frameWidth = projectileTexture.getWidth() / 4;
    float frameHeight = projectileTexture.getHeight();

    SDL_FRect spriteClips[] = {
        { 0.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 1.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 2.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 3.f * frameWidth, 0.f, frameWidth, frameHeight }
    };

    // Calculate target height as 50% of cell size
    float targetHeight = Global::currentMap->PIXELS_PER_CELL * 0.5f;

    // Maintain aspect ratio for width
    float aspectRatio = frameWidth / frameHeight;
    float targetWidth = targetHeight * aspectRatio;

    // Render the projectile with correct scaling and rotation
    projectileTexture.render(x, y, &spriteClips[currentFrame], targetWidth, targetHeight, rotationAngle - 90);
}

void Projectile::updateAnimation(float deltaTime)
{
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration)
    {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

void Projectile::destroy()
{
    projectileTexture.destroy();
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
bool Projectile::checkCollision(Critter* critter) {
    //float tolerance = -4.0f;
    float tolerance = 0.0f;
    int critterX = critter->getPosition().x;
    int critterY = critter->getPosition().y;
    int critterSize = critter->getPosition().w;

    return 
        // checks both horizontal sides of the projectile
        x < critterX + critterSize + tolerance &&
        x + projectileSize > critterX - tolerance &&
        
        // checks both vertical sides of the projectile
        y < critterY + critterSize + tolerance &&
        y + projectileSize > critterY - tolerance;
}

