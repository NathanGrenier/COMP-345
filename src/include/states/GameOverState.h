/**
 * @file GameOverState.h
 * @brief The header file for the GameOverState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include <states/EndScreenState.h>
#include <UI/Texture.h>
#include <string>

class GameOverState : public EndScreenState {
public:
    const static std::string FAST_CRITTER_TYPE;
    const static std::string TANK_CRITTER_TYPE;
    const static std::string NORMAL_CRITTER_TYPE;
    const static int CRITTER_ANIMATION_WIDTH = 320;

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void update() override;
    void render() override;

    //Static accessor
    static GameOverState* get();

private:
    //Static instance
    static GameOverState sGameOverState;

    //Critter animation
    Texture textureWalkDown;
    std::vector<SDL_FRect> animationFramesWalkDown;

    void generateCritterSprite();
    void animateCritterSprite(float deltaTime);

    //Critter animation timings
    float animationTimer = 0.0f;
    int currentFrame = 0;
};
