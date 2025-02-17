#pragma once

#include <tower/CritterTower.h>
#include <vector>
#include <SDL3/SDL.h>
#include <tower/CritterProjectile.h>
#include <critter/Critter.h>
#include <ui/LTexture.h>

class CritterTower {
public:
    CritterTower(int cost, int damage, float range, float fireRate, SDL_FRect position);

    void update(float deltaTime, std::vector<Critter>& critters);  // Attack critters in range
    void render(SDL_Renderer* renderer);  // Render tower
    bool isDestroyed() const;  // Check if the tower is destroyed
    SDL_FRect getPosition() { return position; }
    int getCost() { return cost; }

    // Buying & Selling
    static bool canBuy(int playerCoins, int cost);
    static void buyTower(int& playerCoins, int cost);
    static void sellTower(int& playerCoins, int sellValue);

    void shootProjectile(Critter& target);
    void takeDamage(int amount);  // Reduce tower health
    bool isInRange(Critter& critter);

private:
    int cost;
    int damage;
    float range;
    float fireRate;  // How often the tower shoots
    SDL_FRect position;
    int health;
    float timeSinceLastShot;
    std::vector<CritterProjectile> projectiles;
};
