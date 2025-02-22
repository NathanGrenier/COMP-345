#include <towers/CritterProjectile.h>
#include <cmath>

/**
 * @class CritterProjectile
 * @brief Represents a projectile fired from a tower towards a target critter.
 *
 * The CritterProjectile class handles the movement of the projectile, its collision detection
 * with critters, and rendering. It also ensures that projectiles are updated to track the critter's
 * current position dynamically during the game loop.
 *
 * @author Nirav Patel
 */
CritterProjectile::CritterProjectile(SDL_FRect startPosition, SDL_FRect target, int damage)
    : position(startPosition), target(target), damage(damage), speed(200.0f), active(true) {
}

/**
 * @brief Render the projectile to the screen.
 *
 * This method draws the projectile to the screen using SDL's rendering system.
 * The projectile is rendered as a green rectangle.
 *
 * @param renderer The SDL renderer used to draw the projectile.
 */
void CritterProjectile::render(SDL_Renderer* renderer) {
    if (!active) return; // Don't render if the projectile is inactive

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for projectiles
    SDL_RenderFillRect(renderer, &position);
}

/**
 * @brief Check if the projectile collides with a critter.
 *
 * This method checks if the projectile has collided with a critter by comparing the
 * bounding boxes of the projectile and the critter. A tolerance is allowed to account for small
 * variations in positioning.
 *
 * @param critterPos The position of the critter to check for collision.
 * @return True if the projectile collides with the critter, false otherwise.
 */
bool CritterProjectile::checkCollision(const SDL_FRect& critterPos) {
    float tolerance = -4.0f;  // Tolerance value to allow for a small margin of error
    return position.x < critterPos.x + critterPos.w + tolerance &&  // Allow extra space on the right
        position.x + position.w > critterPos.x - tolerance &&  // Allow extra space on the left
        position.y < critterPos.y + critterPos.h + tolerance && // Allow extra space below
        position.y + position.h > critterPos.y - tolerance;    // Allow extra space above
}

/**
 * @brief Update the position of the projectile towards the current target.
 *
 * This method updates the projectile's position to move it towards the target. It dynamically
 * adjusts the target position in each update and recalculates the direction and speed to ensure
 * the projectile is always moving towards the critter's current position.
 *
 * @param deltaTime The time elapsed since the last frame, used for movement calculation.
 * @param currentTargetPos The current position of the target critter.
 */
void CritterProjectile::update(float deltaTime, const SDL_FRect& currentTargetPos) {
    // Update target position dynamically
    target = currentTargetPos;

    // Calculate new direction
    float dx = target.x - position.x;
    float dy = target.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Avoid division by zero
    if (distance > 0.0f) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        // Move projectile towards the updated target position
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;
    }
}
