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
std::unordered_set<int> ParallaxBackground::propXPositions;

bool ParallaxBackground::isMoving = true;

ParallaxBackground::ParallaxBackground() {
	// Seed the random number generator
	std::srand(std::time(0));


	// Get the list of star images from assets/backgrounds/stars
	populateAssetPaths("assets/backgrounds/stars", starImages);

	// Get the list of prop images from assets/backgrounds/props
	populateAssetPaths("assets/backgrounds/props", propImages);

	// Add the background image (as before, it could be selected randomly from assets/backgrounds)
	populateAssetPaths("assets/backgrounds", backgroundImages);

	// Add stars with random selection
	//if (!starImages.empty()) {
	//	int randomStarIndex = std::rand() % starImages.size();
	//	std::string randomStarImage = starImages[randomStarIndex];
	//	ParallaxLayer* newLayer = new ParallaxLayer(10.0f, 0, randomStarImage);
	//	layers.push_back(newLayer);
	//} else {
	//	SDL_Log("No star images found in the assets/backgrounds/stars directory.\n");
	//}

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
	if (!isMoving) return;

	for (auto layer : layers) {
		layer->update(deltaTime);
	}
}

void ParallaxBackground::render() {
	for (size_t i = 0; i < layers.size(); ++i) {
		layers[i]->render();
	}
}

void ParallaxBackground::populateAssetPaths(const std::string& directory, std::vector<std::string>& array) {
	if (array.empty()) {
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_regular_file() && entry.path().extension() == ".png") {
				std::string relativePath = fs::relative(entry.path(), basePath).string();
				array.push_back(relativePath);
			}
		}
	}
}