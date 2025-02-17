#include <tower/CritterProjectile.h>
#include <cmath>

CritterProjectile::CritterProjectile(SDL_FRect startPosition, SDL_FRect target, int damage)
    : position(startPosition), target(target), damage(damage), speed(200.0f), active(true) {
}

void CritterProjectile::render(SDL_Renderer* renderer) {
    if (!active) return; // Don't render if the projectile is inactive

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for projectiles
    SDL_RenderFillRect(renderer, &position);
}

bool CritterProjectile::checkCollision(const SDL_FRect& critterPos) {
    float tolerance = -4.0f;
    return position.x < critterPos.x + critterPos.w + tolerance &&  // Allow extra space on the right
        position.x + position.w > critterPos.x - tolerance &&  // Allow extra space on the left
        position.y < critterPos.y + critterPos.h + tolerance && // Allow extra space below
        position.y + position.h > critterPos.y - tolerance;    // Allow extra space above
}


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
