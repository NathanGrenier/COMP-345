#include <ui/ParallaxBackground.h>
#include <Global.h>

bool ParallaxBackground::loadTexture(SDL_Renderer* renderer, const std::string& filepath) {
    if (!mParallaxTexture.loadFromFile(filepath)) {
        SDL_Log("Failed to load background texture: %s\n", filepath.c_str());
        return false;
    }
    width = mParallaxTexture.getWidth();
    height = mParallaxTexture.getHeight();
    x2 = x1 + width;
    return true;
}

void ParallaxBackground::update(float deltaTime) {
    // Move both textures
    x1 -= speed * deltaTime;
    x2 -= speed * deltaTime;

    // Check if first texture has moved out of bounds (left side)
    if (x1 + width <= 0) {
        x1 = x2 + width;
    }

    // Check if second texture has moved out of bounds
    if (x2 + width <= 0) {
        x2 = x1 + width;
    }
}

void ParallaxBackground::render(SDL_Renderer* renderer) {
    SDL_FRect rect1 = { x1, 0, width, height };
    SDL_FRect rect2 = { x2, 0, width, height };

    // Calculate clipping to avoid drawing out-of-bounds regions
    SDL_FRect clipRect1 = { 0, 0, width, height };
    SDL_FRect clipRect2 = { 0, 0, width, height };

    // If rect1 is partially off-screen on the left
    if (rect1.x < 0) {
        clipRect1.x = -rect1.x; // Clip the left portion
        clipRect1.w = width - clipRect1.x;
        rect1.w = clipRect1.w;
        rect1.x = 0; // Start drawing from the leftmost visible point
    }

    // If rect2 is partially off-screen on the left
    if (rect2.x < 0) {
        clipRect2.x = -rect2.x;
        clipRect2.w = width - clipRect2.x;
        rect2.w = clipRect2.w;
        rect2.x = 0;
    }

    // If rect1 is partially off-screen on the right
    if (rect1.x + rect1.w > Global::kScreenWidth) {
        clipRect1.w = Global::kScreenWidth - rect1.x;
        rect1.w = clipRect1.w;
    }

    // If rect2 is partially off-screen on the right
    if (rect2.x + rect2.w > Global::kScreenWidth) {
        clipRect2.w = Global::kScreenWidth - rect2.x;
        rect2.w = clipRect2.w;
    }

    // Render both textures with proper clipping
    mParallaxTexture.render(rect1.x, rect1.y, &clipRect1, rect1.w, rect1.h);
    mParallaxTexture.render(rect2.x, rect2.y, &clipRect2, rect2.w, rect2.h);
}
