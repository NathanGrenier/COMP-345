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
Projectile::Projectile(float x, float y, int damage, bool isArea, int rotationAngle, float xSpeed, float ySpeed, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(3), rotationAngle(rotationAngle), xSpeed(xSpeed), ySpeed(ySpeed)
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
Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize, int rotationAngle, float xSpeed, float ySpeed, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(projectileSize), rotationAngle(rotationAngle), xSpeed(xSpeed), ySpeed(ySpeed)
{
    projectileTexture.loadFromFile(texturePath);
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
Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize, int rotationAngle, int speed, const Critter* targettedCritter, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(3), rotationAngle(rotationAngle)
{
    targettedCritter = nullptr;
    projectileTexture.loadFromFile(texturePath);
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
Projectile::Projectile(float x, float y, int damage, bool isArea, int rotationAngle, int speed, const Critter* targettedCritter, std::string texturePath)
    : x(x), y(y), damage(damage), isArea(isArea), speed(speed), projectileSize(projectileSize), rotationAngle(rotationAngle), targettedCritter(targettedCritter)
{
    targettedCritter = nullptr;
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
 * @param multiplier speed multiplier
 * @details adds the xSpeed and ySpeed values to their corresponding coordinates to move the Projectile along the map
 */
void Projectile::move(float multiplier)
{
    float unitDistanceX = 0;
    float unitDistanceY = 0;

    //if (targettedCritter != nullptr && targettedCritter->isAlive())
    //{
    //    SDL_FRect currentCellSize = Global::currentMap->getPixelPerCell();

    //    // projectile position with offset
    //    float halfProjectileSize = static_cast<float>(projectileSize) / 2;
    //    float posX = x + halfProjectileSize;
    //    float posY = y + halfProjectileSize;

    //    // critter position with offset
    //    float critterPosX = targettedCritter->getPosition().x + Critter::CRITTER_WIDTH_SCALE * currentCellSize.w / 2;
    //    float critterPosY = targettedCritter->getPosition().y + Critter::CRITTER_HEIGHT_SCALE * currentCellSize.h / 2;

    //    // differences in position from tower to cannon
    //    float differenceX = critterPosX - posX;
    //    float differenceY = critterPosY - posY;

    //    float distance = sqrt(pow(differenceX, 2) + pow(differenceY, 2));

    //    // distance for projectile as a unit vector
    //    unitDistanceX = differenceX / distance;
    //    unitDistanceY = differenceY / distance;
    //}
    
    //x += speed * unitDistanceX;
    //y += speed * unitDistanceY;
    x += xSpeed * multiplier;
    y += ySpeed * multiplier;
}

/**
 * @brief Generates Projectile
 * 
 * @details Represents a Projectile with a black square
 * Draws the square using SDL 
 */
void Projectile::generateProjectile()
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

    float targetHeight = Global::currentMap.getPixelPerCell() * 0.8f;

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
    return ((x < 0 || x > Global::mapViewRect.w - 20) || (y < Global::headerHeight || y > Global::kScreenHeight));
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
    // Midpoint collision tolerance (you can adjust this if you want a small margin of error)
    float tolerance = 0.0f;

    // Get critter's position and size
    int critterX = critter->getPosition().x;
    int critterY = critter->getPosition().y;
    int critterSize = critter->getPosition().w; // Assuming square critter, w == h

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
        
        
        // removed damage in projectile collision
        //critter->takeDamage(damage);
        //critter->notify();




        return true; // Collision detected
    }

    return false;
}

Critter* Projectile::getTargettedCritter()
{
    return const_cast<Critter*>(targettedCritter);
}
