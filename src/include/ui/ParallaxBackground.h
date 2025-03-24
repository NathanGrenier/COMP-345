#pragma once

#include "LTexture.h"
#include <SDL3/SDL_log.h>

class ParallaxBackground {
public:
    LTexture mParallaxTexture;
    float x1, x2;
    float speed;
    int width, height;

    ParallaxBackground() : x1(0), x2(0), speed(25.0f) {}

    bool loadTexture(SDL_Renderer* renderer, const std::string& filepath);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer);
};
