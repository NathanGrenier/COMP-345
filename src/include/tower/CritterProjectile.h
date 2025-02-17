#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <critter/Critter.h>
#include <vector>

class CritterProjectile {
public:
    CritterProjectile(SDL_FRect startPosition, SDL_FRect target, int damage);

    void update(float deltaTime, const SDL_FRect& currentTargetPos);  // Update projectile position
    void render(SDL_Renderer* renderer);  // Render the projectile
    bool checkCollision(const SDL_FRect& critterPos);  // Check if the projectile hits a critter

    SDL_FRect getPosition() { return position; }
    int getDamage() { return damage; }
private:
    SDL_FRect position;
    SDL_FRect target;
    int damage;
    float speed;
    bool active;
};
