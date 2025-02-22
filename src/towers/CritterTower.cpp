#include <towers/CritterTower.h>
#include <critter/Critter.h>
#include <cmath>
#include <ui/LTexture.h>

/**
 * @class CritterTower
 * @brief Represents a tower that shoots projectiles at critters.
 *
 * The CritterTower class handles the mechanics of a tower, including shooting projectiles,
 * checking for collisions with critters, and managing health. It also includes the logic for
 * buying and selling the tower, as well as rendering the tower and projectiles.
 * 
 * @author Nirav Patel
 */
CritterTower::CritterTower(int cost, int damage, float range, float fireRate, SDL_FRect position)
    : cost(cost), damage(damage), range(range), fireRate(fireRate), position(position), health(100), timeSinceLastShot(0.0f) {}

/**
 * @brief Shoot a projectile at a target critter.
 *
 * This method creates a new projectile and adds it to the list of projectiles to be rendered
 * and updated during the game loop.
 *
 * @param target The critter that the projectile will be shot at.
 */
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

/**
 * @brief Update the state of the tower and its projectiles.
 *
 * This method checks whether the tower can shoot based on the cooldown time and the range to
 * the nearest critter. It also updates the projectiles, checks for collisions, and removes
 * projectiles that hit critters.
 *
 * @param deltaTime The time elapsed since the last frame, used for updating projectiles.
 * @param critters A list of critters in the game, used to determine if a critter is within range.
 */
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

    if (critters.empty()) {
        projectiles.clear();
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

/**
 * @brief Check if the tower has been destroyed.
 *
 * This method checks whether the tower's health has dropped to zero or below.
 *
 * @return True if the tower is destroyed, false otherwise.
 */
bool CritterTower::isDestroyed() const {
    return health <= 0;
}

/**
 * @brief Apply damage to the tower.
 *
 * This method reduces the tower's health by the specified amount.
 *
 * @param amount The amount of damage to apply to the tower.
 */
void CritterTower::takeDamage(int amount) {
    health -= amount;
}

/**
 * @brief Check if the player can afford to buy the tower.
 *
 * This method checks whether the player has enough coins to purchase the tower.
 *
 * @param playerCoins The current amount of coins the player has.
 * @param cost The cost of the tower.
 * @return True if the player can afford the tower, false otherwise.
 */
bool CritterTower::canBuy(int playerCoins, int cost) {
    return playerCoins >= cost;
}

/**
 * @brief Buy the tower and deduct its cost from the player's coins.
 *
 * This method deducts the cost of the tower from the player's coins if they can afford it.
 *
 * @param playerCoins The player's current coin amount.
 * @param cost The cost of the tower.
 */
void CritterTower::buyTower(int& playerCoins, int cost) {
    if (canBuy(playerCoins, cost)) {
        playerCoins -= cost;
    }
}

/**
 * @brief Sell the tower and add its sell value to the player's coins.
 *
 * This method adds a portion of the tower's cost back to the player's coins when selling the tower.
 *
 * @param playerCoins The player's current coin amount.
 * @param sellValue The value received for selling the tower.
 */
void CritterTower::sellTower(int& playerCoins, int sellValue) {
    playerCoins += sellValue;
}

/**
 * @brief Render the tower and its projectiles.
 *
 * This method renders the tower as well as all the projectiles currently active in the game.
 *
 * @param renderer The SDL renderer used to draw the tower and projectiles.
 */
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

/**
 * @brief Check if a critter is within the tower's range.
 *
 * This method calculates the distance between the tower and the critter and checks if it's within range.
 *
 * @param critter The critter to check.
 * @return True if the critter is within range, false otherwise.
 */
bool CritterTower::isInRange(Critter& critter) {
    float dx = critter.getPosition().x - position.x;
    float dy = critter.getPosition().y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}
