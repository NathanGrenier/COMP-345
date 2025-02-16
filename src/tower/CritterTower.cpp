#include <tower/CritterTower.h>
#include <critter/Critter.h>
#include <cmath>
#include <ui/LTexture.h>

CritterTower::CritterTower(int cost, int damage, float range, float fireRate, SDL_FRect position)
    : cost(cost), damage(damage), range(range), fireRate(fireRate), position(position), health(100), timeSinceLastShot(0.0f) {}

void CritterTower::shootProjectile(Critter& target) {
    // Define the size of the projectile
    float projectileWidth = 10.0f;
    float projectileHeight = 10.0f;

    // Center the projectile at the tower's position
    SDL_FRect projectilePos;
    projectilePos.w = projectileWidth;
    projectilePos.h = projectileHeight;
    projectilePos.x = position.x + (position.w - projectileWidth) / 2;
    projectilePos.y = position.y + (position.h - projectileHeight) / 2;

    // Create and add the projectile
    CritterProjectile newProjectile(projectilePos, target.getPosition(), damage);
    projectiles.push_back(newProjectile);
}

void CritterTower::update(float deltaTime, std::vector<Critter>& critters) {
    if (timeSinceLastShot >= fireRate) {
        // Find nearest critter within range
        Critter* target = nullptr;
        for (auto& critter : critters) {
            if (isInRange(critter)) {
                target = &critter;
                break;
            }
        }

        if (target) {
            // Shoot a projectile at the target critter
            shootProjectile(*target);
            timeSinceLastShot = 0.0f;  // Reset the cooldown
        }
    }

    // Update projectiles (pass the current target position dynamically)
    for (auto& proj : projectiles) {
        // Find the closest critter to the projectile
        Critter* closestCritter = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (auto& critter : critters) {
            float dx = critter.getPosition().x - proj.getPosition().x;
            float dy = critter.getPosition().y - proj.getPosition().y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < closestDistance) {
                closestDistance = distance;
                closestCritter = &critter;
            }
        }

        if (closestCritter) {
            proj.update(deltaTime, closestCritter->getPosition());  // Update projectile's movement
        }
    }

    // Check for collisions with critters
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        bool hit = false;

        for (auto& critter : critters) {
            if (it->checkCollision(critter.getPosition())) {
                critter.takeDamage(it->getDamage());  // Deal damage to the critter
                hit = true;
                break;
            }
        }

        if (hit) {
            it = projectiles.erase(it);  // Remove the projectile if it hit a critter
        }
        else {
            ++it;
        }
    }

    timeSinceLastShot += deltaTime;
}


bool CritterTower::isDestroyed() const {
    return health <= 0;
}

void CritterTower::takeDamage(int amount) {
    health -= amount;
}

bool CritterTower::canBuy(int playerCoins, int cost) {
    return playerCoins >= cost;
}

void CritterTower::buyTower(int& playerCoins, int cost) {
    if (canBuy(playerCoins, cost)) {
        playerCoins -= cost;
    }
}

void CritterTower::sellTower(int& playerCoins, int sellValue) {
    playerCoins += sellValue;
}

void CritterTower::render(SDL_Renderer* renderer) {
    // Render tower
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &position);

    // Load and render bold "T"
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color for the text
    LTexture textTexture;
    if (textTexture.loadFromRenderedText("T", textColor)) {
        // Center the text over the tower
        int textX = position.x + (position.w - textTexture.getWidth()) / 2;
        int textY = position.y + (position.h - textTexture.getHeight()) / 2;
        textTexture.render(textX, textY);
    }

    // Render projectiles
    for (auto& proj : projectiles) {
        proj.render(renderer);
    }
}


bool CritterTower::isInRange(Critter& critter) {
    float dx = critter.getPosition().x - position.x;
    float dy = critter.getPosition().y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}
