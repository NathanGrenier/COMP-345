/**
 * @file CritterProjectile.h
 * @brief The header file for the CritterProjectile class, used for the Part 3 Demo.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <critter/Critter.h>
#include <vector>

class CritterProjectile {
public:
    CritterProjectile(SDL_FRect startPosition, SDL_FRect target, int damage);

    void update(float deltaTime, const SDL_FRect& currentTargetPos);
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_FRect& critterPos);

    SDL_FRect getPosition() { return position; }
    int getDamage() { return damage; }
private:
    SDL_FRect position;
    SDL_FRect target;
    int damage;
    float speed;
    bool active;
};
