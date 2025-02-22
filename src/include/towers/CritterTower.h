/**
 * @file CritterTower.h
 * @brief The header file for the CritterTower class, used for the Part 3 Demo.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <towers/CritterProjectile.h>
#include <critter/Critter.h>
#include <ui/LTexture.h>

class CritterTower {
public:
    CritterTower(int cost, int damage, float range, float fireRate, SDL_FRect position);

    void update(float deltaTime, std::vector<Critter>& critters);
    void render(SDL_Renderer* renderer);
    bool isDestroyed() const;
    SDL_FRect getPosition() { return position; }
    int getCost() { return cost; }

    // Buying & Selling
    static bool canBuy(int playerCoins, int cost);
    static void buyTower(int& playerCoins, int cost);
    static void sellTower(int& playerCoins, int sellValue);

    void shootProjectile(Critter& target);
    void takeDamage(int amount);
    bool isInRange(Critter& critter);

private:
    int cost;
    int damage;
    float range;
    float fireRate;
    SDL_FRect position;
    int health;
    float timeSinceLastShot;
    std::vector<CritterProjectile> projectiles;
};
