#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <ui/Texture.h>
#include "ParallaxLayer.h"  // Forward declaration of ParallaxLayer class

class ParallaxBackground {
public:
	// Constructor to initialize the background with layers
	ParallaxBackground();

	// Destructor
	~ParallaxBackground();

	// Add a layer to the background
	void addLayer(float speed, float height);

	// Update all layers
	void update(float deltaTime);

	// Render all layers
	void render();

	void populateAssetPaths(const std::string& directory, std::vector<std::string>& array);

	static std::vector<std::string> backgroundImages;
	static std::vector<std::string> starImages;
	static std::vector<std::string> propImages;
	static std::unordered_set<float> propXPositions;

	static bool isMoving;
private:
	std::vector<ParallaxLayer*> layers;  // Vector to store all the layers
	std::string basePath = "assets/";
};

