#pragma once

#include <SDL3/SDL.h>
#include <vector>

class Critter {
public:
    Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, SDL_FRect end);

    void move(float deltaTime, const std::vector<Critter>& critters, float spacing);  // Move critter towards exit
    void takeDamage(int damage); // Apply damage from towers
    bool isAlive() const;        // Check if critter is still alive
    void stealCoins(int& playerCoins);  // Steal coins if critter reaches the exit

    void render(SDL_Renderer* renderer);  // Render critter (could be a simple shape or sprite)

    int getReward() const { return reward; }
    int getStrength() const { return strength; }
    int getHitPoints() const { return hitPoints; }
    void setEndPosition(SDL_FRect newEndPosition);
private:
    int level;
    float speed;
    int hitPoints;
    int strength;
    int reward;
    SDL_FRect position;
    SDL_FRect startPosition;
    SDL_FRect exitPosition;
    bool isAtExit;
};
