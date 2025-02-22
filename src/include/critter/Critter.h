/**
 * @file Critter.h
 * @brief The header file for the Critter class.
 * @author Nirav Patel
 * @date 2025-02-21
 */
#pragma once

#include <SDL3/SDL.h>
#include <vector>

class Critter {
public:
    Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, SDL_FRect end);

    void move(float deltaTime, const std::vector<Critter>& critters, float spacing);  // Move critter towards exit
    void takeDamage(int damage); // Apply damage from towers
    bool isAlive() const;        // Check if critter is still alive
    void stealGold(int& playerGold);  // Steal gold if critter reaches the exit

    void render(SDL_Renderer* renderer);  // Render critter (could be a simple shape or sprite)

    int getReward() const { return reward; }
    int getStrength() const { return strength; }

    int getHitPoints() const { return hitPoints; }
    void setHitPoints(int hitPoints);

    void setEndPosition(SDL_FRect newEndPosition);
    SDL_FRect getEndPosition();

    SDL_FRect getPosition();

    bool atExit();
    void setAtExit(bool con);
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
    float healthBarWidth;
    bool healthBarVisible;
    float healthBarTime;
    int maxHitPoints;
};
