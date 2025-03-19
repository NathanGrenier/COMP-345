#include <towers/powerups/Powerup.h>

Powerup::Powerup(SDL_FRect position, std::string texturePath)
    : position(position), currentFrame(0), frameTime(0), frameDuration(1.0f / 60.0f),
    bobbingSpeed(1.0f), bobbingHeight(0.05f), bobbingOffset(0.0f) {
    powerupTexture.loadFromFile(texturePath);
}

Powerup::~Powerup() {}

bool Powerup::isClicked(float mouseX, float mouseY) const {
    return (mouseX >= position.x && mouseX <= position.x + position.w &&
        mouseY >= position.y && mouseY <= position.y + position.h);
}

void Powerup::update(float deltaTime) {
    // Update the frame time for frame cycling
    frameTime += deltaTime;
    if (frameTime >= frameDuration) {
        frameTime = 0.0f;
        currentFrame = (currentFrame + 1) % 4;  // Cycle through frames 0 to 3
    }

    // Update bobbing effect: Sinusoidal movement for vertical bobbing
    bobbingOffset = sin(SDL_GetTicks() / 1000.0f * bobbingSpeed) * bobbingHeight;
    if (!isDragged) {
        position.y += bobbingOffset;
    }
}

void Powerup::render() {
    int textureWidth = powerupTexture.getWidth() / 4;  // Original width of the texture
    int textureHeight = powerupTexture.getHeight();  // Original height of the texture

    // Calculate the aspect ratio of the texture
    float aspectRatio = static_cast<float>(textureWidth) / static_cast<float>(textureHeight);

    // Calculate the new width and height to fit the powerup within the position's dimensions, preserving the aspect ratio
    float newWidth = position.w;
    float newHeight = position.w / aspectRatio;

    // If the new height is greater than the available height, adjust based on the height instead
    if (newHeight > position.h) {
        newHeight = position.h;
        newWidth = position.h * aspectRatio;
    }

    SDL_FRect srcRect = { currentFrame * textureWidth, 0, textureWidth, textureHeight };
    SDL_FRect destRect = { position.x, position.y, newWidth, newHeight };

    powerupTexture.render(destRect.x, destRect.y, &srcRect, destRect.w, destRect.h);
}

