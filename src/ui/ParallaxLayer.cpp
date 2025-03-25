#include <ui/ParallaxLayer.h>
#include <ui/ParallaxBackground.h>
#include <Global.h>


ParallaxLayer::ParallaxLayer(float speed, float y, const std::string& filepath)
    : speed(speed), x1(0), x2(0), y(y), width(0), height(0) {
    // Load the texture from file
    if (!texture.loadFromFile(filepath)) {
        SDL_Log("Failed to load texture: %s\n", filepath.c_str());
    }

    isProp = (filepath.find("props") != std::string::npos);

    width = texture.getWidth();
    height = texture.getHeight();

    // Calculate random scale factor for size variation
    float scaleFactor = 1.0f + (std::rand() % 51) / 100.0f;
    width = static_cast<int>(width * scaleFactor);
    height = static_cast<int>(height * scaleFactor);
    rotationAngle = 0;

    // Ensure prop doesn't overlap existing props by finding a suitable x1 position
    if (isProp) {
        // Try several random positions to ensure no overlap
        bool overlapDetected = true;
        int maxAttempts = 100; // Maximum number of attempts before giving up
        while (overlapDetected && maxAttempts > 0) {
            x1 = std::rand() % static_cast<int>(Global::kScreenWidth);
            overlapDetected = false;

            // Check if this x1 position overlaps with any previously spawned props
            for (int existingX : ParallaxBackground::propXPositions) {
                if (std::abs(existingX - x1) < width) {  // Ensure no overlap (distance >= width)
                    overlapDetected = true;
                    break;
                }
            }

            maxAttempts--;
        }

        // If after maxAttempts no valid position was found, spawn at a random position anyway
        if (maxAttempts == 0) {
            x1 = std::rand() % static_cast<int>(Global::kScreenWidth);  // Fallback to random position
        }

        // Add the new x1 position to the set of tracked positions
        ParallaxBackground::propXPositions.insert(x1);

        rotationAngle = std::rand() % 360; // Random rotation for props
    }

    x2 = x1 + width;
}

ParallaxLayer::~ParallaxLayer() {
    if (isProp) {
        ParallaxBackground::propXPositions.erase(x1);
    }
    texture.destroy();
}

void ParallaxLayer::update(float deltaTime) {
    if (isProp) {
        // Move prop textures and check if they are out of bounds
        x1 -= speed * deltaTime;
        if (x1 + width <= 0) {
            // Prop is out of bounds, reload a new texture and spawn it at a random y position
            int randomIndex = std::rand() % ParallaxBackground::propImages.size(); // assuming a list of prop images
            std::string randomProp = ParallaxBackground::propImages[randomIndex];
            if (!texture.loadFromFile(randomProp)) {
                SDL_Log("Failed to reload prop texture: %s\n", randomProp.c_str());
            }

            width = texture.getWidth();
            height = texture.getHeight();
            float scaleFactor = 1.0f + (std::rand() % 51) / 100.0f;  // Random value between 1 and 1.5
            width = static_cast<int>(width * scaleFactor);
            height = static_cast<int>(height * scaleFactor);
            rotationAngle = std::rand() % 360;

            // Ensure the new prop doesn't overlap with any other props
            bool overlapDetected = true;
            int maxAttempts = 100; // Maximum number of attempts before giving up
            while (overlapDetected && maxAttempts > 0) {
                x1 = Global::kScreenWidth; // Spawn at the right edge
                overlapDetected = false;

                // Check if this x1 position overlaps with any previously spawned props
                for (int existingX : ParallaxBackground::propXPositions) {
                    if (std::abs(existingX - x1) < width) {
                        overlapDetected = true;
                        break;
                    }
                }

                maxAttempts--;
            }

            // Add the new x1 position to the set of tracked positions
            ParallaxBackground::propXPositions.insert(x1);

            // Set new y position when prop is out of bounds
            y = std::rand() % static_cast<int>(Global::kScreenHeight);
        }
    }
    else {
        // Move both textures for background
        x1 -= speed * deltaTime;
        x2 -= speed * deltaTime;

        // Wrap around textures if they go off-screen with 1-pixel overlap
        if (x1 + width <= 0) {
            x1 = x2 + width;
        }

        if (x2 + width <= 0) {
            x2 = x1 + width;
        }
    }
}

void ParallaxLayer::render() {
    // Define the rectangle for rendering the first texture
    SDL_FRect rect1 = { x1, y, width + 2, height };

    // Define the rectangle for rendering the second texture
    SDL_FRect rect2 = { x2, y, width + 2, height };

    // Render the first texture
    texture.render(rect1.x, rect1.y, nullptr, rect1.w, rect1.h, rotationAngle);

    // Render the second texture
    if (!isProp) texture.render(rect2.x, rect2.y, nullptr, rect2.w, rect2.h);
}
