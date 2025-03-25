#include <ui/ParallaxBackground.h>
#include <cstdlib>   // For rand()
#include <ctime>     // For time() for seeding
#include <filesystem> // For directory traversal
#include <vector>    // For storing the list of files
#include <Global.h>

namespace fs = std::filesystem;

std::vector<std::string> ParallaxBackground::backgroundImages;
std::vector<std::string> ParallaxBackground::starImages;
std::vector<std::string> ParallaxBackground::propImages;

ParallaxBackground::ParallaxBackground() {
    // Seed the random number generator
    std::srand(std::time(0));

    // Get the list of star images from assets/backgrounds/stars
    for (const auto& entry : fs::directory_iterator("assets/backgrounds/stars")) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            starImages.push_back(entry.path().string());
        }
    }

    // Get the list of prop images from assets/backgrounds/props
    for (const auto& entry : fs::directory_iterator("assets/backgrounds/props")) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            propImages.push_back(entry.path().string());
        }
    }

    // Add the background image (as before, it could be selected randomly from assets/backgrounds)
    for (const auto& entry : fs::directory_iterator("assets/backgrounds")) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            backgroundImages.push_back(entry.path().string());
        }
    }

    // Add stars with random selection
    if (!starImages.empty()) {
        int randomStarIndex = std::rand() % starImages.size();
        std::string randomStarImage = starImages[randomStarIndex];
        ParallaxLayer* newLayer = new ParallaxLayer(10.0f, 0, randomStarImage);
        layers.push_back(newLayer);
    }
    else {
        SDL_Log("No star images found in the assets/backgrounds/stars directory.\n");
    }


    if (!backgroundImages.empty()) {
        int randomIndex = std::rand() % backgroundImages.size();
        std::string randomBackground = backgroundImages[randomIndex];
        ParallaxLayer* newLayer = new ParallaxLayer(20.0f, 0, randomBackground);
        layers.push_back(newLayer);
    } else {
        SDL_Log("No background images found in the assets/backgrounds directory.\n");
    }
}

ParallaxBackground::~ParallaxBackground() {
    for (auto layer : layers) {
        delete layer;
    }
}

void ParallaxBackground::addLayer(float speed, int height) {
    int randomIndex = std::rand() % propImages.size();
    std::string randomProp = propImages[randomIndex];
    int randomY = std::rand() % height;
    ParallaxLayer* newLayer = new ParallaxLayer(speed, randomY, randomProp);
    layers.push_back(newLayer);
}

void ParallaxBackground::update(float deltaTime) {
    for (auto layer : layers) {
        layer->update(deltaTime);
    }
}

void ParallaxBackground::render() {
    for (size_t i = 0; i < layers.size(); ++i) {
        layers[i]->render();
    }
}
