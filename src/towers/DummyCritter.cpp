/**
 * @class DummyCritter
 * @brief Implementation of DummyCritter class
 * @author Denmar Ermitano
 * @date 2025-02-22
 */

#include <Global.h>
#include <towers/DummyCritter.h>

/**
 * @brief Constructor with position and health
 * 
 * @param x Horizontal position using pixels
 * @param y Vertical position using pixels
 * @param health Health points for DummyCritter
 * @details Constructor for DummyCritter with x, y position and health
 * Uses health as damage points, with DummyCritter "dying" should it reach 0 health
 */
DummyCritter::DummyCritter(float x, float y, int health)
    : x(x), y(y), health(health)
{

}

/**
 * @brief Damages a DummyCritter
 * 
 * @param damage the amount of health to remove from the DummyCritter
 * @details Decrements the health for the DummyCritter
 * @return true if DummyCritter reaches 0 health
 * @return false if DummyCritter still has health remaining
 */
bool DummyCritter::damageCritter(int damage)
{
    health -= damage;
    return (health <= 0);
}

/**
 * @brief Accessor for health
 * 
 * @return The remaining health of the DummyCritter
 */
int DummyCritter::getHealth() 
{
    return health;
}

/**
 * @brief Accessor for x position
 * 
 * @return The horizontal position for the DummyCritter in pixels
 */
float DummyCritter::getX()
{
    return x;
}

/**
 * @brief Accessor for y position
 * 
 * @return The vertical position for the DummyCritter in pixels
 */
float DummyCritter::getY()
{
    return y;
}

/**
 * @brief Generates DummyCritter
 * 
 * @details Represents a DummyCritter with a green square
 * Draws the square using SDL 
 */
void DummyCritter::generateCritter()
{
    SDL_FRect fillRect = { x, y, CRITTER_SIZE, CRITTER_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 0x1B, 0x82, 0x4D, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}
