#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "ParallaxLayer.h"  // Forward declaration of ParallaxLayer class

class ParallaxBackground {
public:
    // Constructor to initialize the background with layers
    ParallaxBackground();

    // Destructor
    ~ParallaxBackground();

    // Add a layer to the background
    void addLayer(float speed, int height);

    // Update all layers
    void update(float deltaTime);

    // Render all layers
    void render();

    static std::vector<std::string> backgroundImages;
    static std::vector<std::string> starImages;
    static std::vector<std::string> propImages;
    static std::unordered_set<int> propXPositions;
private:
    std::vector<ParallaxLayer*> layers;  // Vector to store all the layers
    LTexture backgroundLayer;
    LTexture starsLayer;
};

