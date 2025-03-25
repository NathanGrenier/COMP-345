#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <ui/LTexture.h>
#include <unordered_set>

class ParallaxLayer {
public:
    // Constructor to initialize the layer with speed and texture filepath
    ParallaxLayer(float speed, float y, const std::string& filepath);

    // Destructor
    virtual ~ParallaxLayer();

    // Update the layer position based on delta time
    virtual void update(float deltaTime);

    // Render the layer
    virtual void render();

protected:
    LTexture texture;  // Texture for the layer
    float speed;      // Speed of scrolling
    float x1, x2;     // X positions for the two textures (for scrolling)
    float y;
    int width, height;  // Dimensions of the texture
    bool isProp;
    float rotationAngle;
};
