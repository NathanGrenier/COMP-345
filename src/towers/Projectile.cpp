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
 * Initializes the projectile's position, damage, speed, rotation angle,
 * and texture for rendering. This version does not take size into account.
 *
 * @param x The x position of the projectile.
 * @param y The y position of the projectile.
 * @param damage The amount of damage the projectile does to critters.
 * @param isArea Whether the projectile has an area of effect (not yet implemented).
 * @param rotationAngle The rotation angle of the projectile.
 * @param xSpeed The horizontal speed of the projectile.
 * @param ySpeed The vertical speed of the projectile.
 * @param texturePath The file path to the texture for the projectile.
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, float rotationAngle, float xSpeed, float ySpeed, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(3), rotationAngle(rotationAngle), xSpeed(xSpeed), ySpeed(ySpeed), targettedCritter(nullptr)
{   
    currentRenderRect = {};
    projectileTexture.loadFromFile(texturePath);
}

/**
 * @brief Constructor with position, damage, if projectile damages an area, and projectile size for a non-homing projectile
 * 
 * @param x The x position of the projectile.
 * @param y The y position of the projectile.
 * @param damage The amount of damage the projectile does to critters.
 * @param isArea Whether the projectile has an area of effect (not yet implemented).
 * @param projectileSize The size of the projectile.
 * @param rotationAngle The rotation angle of the projectile.
 * @param xSpeed The horizontal speed of the projectile.
 * @param ySpeed The vertical speed of the projectile.
 * @param texturePath The file path to the texture for the projectile.
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize, float rotationAngle, float xSpeed, float ySpeed, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(projectileSize), rotationAngle(rotationAngle), xSpeed(xSpeed), ySpeed(ySpeed), targettedCritter(nullptr)
{
    currentRenderRect = {};
    projectileTexture.loadFromFile(texturePath);
}

/**
 * @brief Constructor with position, damage, and if projectile damages an area for a homing projectile
 *
 * Initializes the projectile's position, damage, speed, rotation angle,
 * and texture for rendering. This version does not take size into account.
 *
 * @param x The x position of the projectile.
 * @param y The y position of the projectile.
 * @param damage The amount of damage the projectile does to critters.
 * @param isArea Whether the projectile has an area of effect (not yet implemented).
 * @param rotationAngle The rotation angle of the projectile.
 * @param speed The speed of the projectile.
 * @param texturePath The file path to the texture for the projectile.
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize, float rotationAngle, float speed, Critter* targettedCritter, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(3), rotationAngle(rotationAngle), targettedCritter(targettedCritter)
{
    projectileTexture.loadFromFile(texturePath);
}

/**
 * @brief Constructor with position, damage, if projectile damages an area, and projectile size for a homing projectile
 *
 * Initializes the projectile's position, damage, speed, size, rotation angle,
 * and texture for rendering. This version supports custom projectile sizes.
 *
 * @param x The x position of the projectile.
 * @param y The y position of the projectile.
 * @param damage The amount of damage the projectile does to critters.
 * @param isArea Whether the projectile has an area of effect (not yet implemented).
 * @param projectileSize The size of the projectile.
 * @param rotationAngle The rotation angle of the projectile.
 * @param speed The speed of the projectile.
 * @param texturePath The file path to the texture for the projectile.
 */
Projectile::Projectile(float x, float y, int damage, bool isArea, float rotationAngle, float speed, Critter* targettedCritter, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(projectileSize), rotationAngle(rotationAngle), targettedCritter(targettedCritter)
{
    projectileTexture.loadFromFile(texturePath);
}

/**
 * @brief Accessor for damage
 * 
 * @return The damage the Projectile will do to a critter
 */
int Projectile::getDamage() const 
{
    return damage;
}

/**
 * @brief Moves the projectile based on its speed.
 *
 * This function updates the position of the projectile based on its speed,
 * multiplying it by a given multiplier (usually a frame delta).
 *
 * @param multiplier The multiplier that adjusts the movement speed.
 */
void Projectile::move(float multiplier)
{
    if (targettedCritter == nullptr)
    {
        if (isOutside()) {
            destroy();
        }
        x += xSpeed * multiplier;
        y += ySpeed * multiplier;
    }

    // need to add code for homing projectiles, will be done for homing Tower
}

/**
 * @brief Generates the projectile by loading the texture and preparing it for rendering.
 */
void Projectile::generateProjectile()
{
    // Define the sprite clips for each frame (horizontal sprite sheet)
    float frameWidth = projectileTexture.getWidth() / static_cast<float>(4);
    float frameHeight = projectileTexture.getHeight();

    SDL_FRect spriteClips[] = {
        { 0.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 1.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 2.f * frameWidth, 0.f, frameWidth, frameHeight },
        { 3.f * frameWidth, 0.f, frameWidth, frameHeight }
    };

    float targetHeight = Global::currentMap->getPixelPerCell() * 0.8f;

    // Maintain aspect ratio for width
    float aspectRatio = frameWidth / frameHeight;
    float targetWidth = targetHeight * aspectRatio;

    // Offset x and y so that projectile is centered at its position
    float renderX = x - targetWidth / 2.0f;
    float renderY = y - targetHeight / 2.0f;

    // Save the rect for collision detection
    currentRenderRect = { renderX, renderY, targetWidth, targetHeight };

    // Render the projectile using the saved rect
    projectileTexture.render(currentRenderRect.x, currentRenderRect.y, &spriteClips[currentFrame], currentRenderRect.w, currentRenderRect.h, rotationAngle - 90);
}

/**
 * @brief Updates the projectile's animation frame based on delta time.
 *
 * This method is responsible for updating the sprite animation of the projectile.
 * It cycles through frames at a set frame rate.
 *
 * @param deltaTime The time difference between frames, used to update animation.
 */
void Projectile::updateAnimation(float deltaTime)
{
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration)
    {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

/**
 * @brief Destroys the projectile, cleaning up resources.
 *
 * This method releases any resources held by the projectile, including textures.
 */
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
bool Projectile::isOutside() const
{
    return ((x < 0 || x > Global::mapViewRect.w) || (y < Global::headerHeight || y > Global::kScreenHeight));
}

/**
 * @brief Checks if the projectile collides with a critter.
 *
 * This method detects whether the projectile intersects with a critter,
 * and if so, applies damage to the critter.
 *
 * @param critter The critter to check for collision with.
 * @return A boolean indicating whether a collision occurred.
 */
bool Projectile::checkCollision(Critter* critter) const {
    // Midpoint collision tolerance (you can adjust this if you want a small margin of error)
    float tolerance = 0.0f;

    // Get critter's position and size
    float critterX = critter->getPosition().x;
    float critterY = critter->getPosition().y;
    float critterSize = critter->getPosition().w; // Assuming square critter, w == h

    // Calculate critter's midpoint
    float critterMidX = critterX + critterSize / 2.0f;
    float critterMidY = critterY + critterSize / 2.0f;

    // Projectile's bounding box
    float projectileLeft = currentRenderRect.x;
    float projectileTop = currentRenderRect.y;
    float projectileRight = currentRenderRect.x + currentRenderRect.w;
    float projectileBottom = currentRenderRect.y + currentRenderRect.h;

    // Check if critter's midpoint is inside projectile's bounding box (with optional tolerance)
    if (critterMidX >= projectileLeft - tolerance &&
        critterMidX <= projectileRight + tolerance &&
        critterMidY >= projectileTop - tolerance &&
        critterMidY <= projectileBottom + tolerance) {
        
        return true; // Collision detected
    }

    return false;
}

Critter* Projectile::getTargettedCritter()
{
    return const_cast<Critter*>(targettedCritter);
}
