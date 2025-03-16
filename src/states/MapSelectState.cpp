/// @file MapSelectState.cpp
/// @brief Implementation of the MapSelectState class.

#include <iostream>
#include <states/MapSelectState.h>
#include <Global.h>
#include <filesystem>
#include <states/MapEditorState.h>
#include <states/MainGameState.h>
#include <ui/LButton.h>
#include <ui/LTexture.h>
#include <map/Map.h>
#include <states/TitleState.h>

namespace fs = std::filesystem;

std::string selectedMapFilePath;

MapSelectState::MapSelectState() : selectedIndex(0) {}

MapSelectState::~MapSelectState() {}

/** @brief Singleton instance of the MapSelectState */
MapSelectState MapSelectState::sMapSelectState;

/**
 * @brief Get the singleton instance of MapSelectState
 * @return Pointer to the MapSelectState singleton
 */
MapSelectState* MapSelectState::get() {
	return &sMapSelectState;
}

bool MapSelectState::enter() {
	loadAvailableMaps();

	mTitle.loadFromFile("assets/ui/MapSelectionMessage.png");

	// Load button textures
	backButton.loadFromFile("assets/ui/LeftArrow.png");
	createButton.loadFromFile("assets/ui/CreateMap.png");
	editButton.loadFromFile("assets/ui/EditMap.png");
	selectButton.loadFromFile("assets/ui/SelectMap.png");
	leftArrow.loadFromFile("assets/ui/LeftArrow.png");
	rightArrow.loadFromFile("assets/ui/RightArrow.png");

	// Set sizes dynamically based on screen width
	constexpr int buttonCount = 3;
	const int buttonSpacing = 20;
	const int maxButtonWidth = (Global::kScreenWidth - (buttonSpacing * (buttonCount + 1))) / buttonCount;

	backButton.setSizeWithAspectRatio(mTitle.getHeight() * 0.75, 0);
	createButton.setSizeWithAspectRatio(maxButtonWidth, 0);
	editButton.setSizeWithAspectRatio(maxButtonWidth, 0);
	selectButton.setSizeWithAspectRatio(maxButtonWidth, 0);

	leftArrow.setSizeWithAspectRatio(50, 0);
	rightArrow.setSizeWithAspectRatio(50, 0);

	// Get max button height (assuming they have the same aspect ratio)
	int maxButtonHeight = std::max({ createButton.kButtonHeight, editButton.kButtonHeight, selectButton.kButtonHeight });

	// Calculate start positions
	int startX = (Global::kScreenWidth - (buttonCount * maxButtonWidth + (buttonCount - 1) * buttonSpacing)) / 2;
	int startY = Global::kScreenHeight - maxButtonHeight - buttonSpacing;

	// Set positions dynamically
	float renderedWidth = Global::kScreenWidth * 0.5f;
	float renderedHeight = (static_cast<float>(mTitle.getHeight()) / mTitle.getWidth()) * renderedWidth;
	float backButtonY = titleDistanceFromTop + (renderedHeight - backButton.kButtonHeight) / 2.0f;
	backButton.setPosition(backButtonDistanceFromLeft, backButtonY);
	createButton.setPosition(startX, startY);
	editButton.setPosition(startX + (maxButtonWidth + buttonSpacing), startY);
	selectButton.setPosition(startX + (2 * (maxButtonWidth + buttonSpacing)), startY);

	// Define the distance you want between the arrows
	const int distanceBetweenArrows = 320; // Adjust this value as needed

	// Calculate the total width that the arrows will occupy (including the space between them)
	int totalArrowsWidth = leftArrow.kButtonWidth + rightArrow.kButtonWidth + distanceBetweenArrows;

	// Calculate the X position for the first arrow (leftArrow) to center them
	int centerX = (Global::kScreenWidth - totalArrowsWidth) / 2;

	// Set the positions for the arrows
	leftArrow.setPosition(centerX, (Global::kScreenHeight - mHoveredMapName.getHeight()) / 2 - 75);
	rightArrow.setPosition(centerX + leftArrow.kButtonWidth + distanceBetweenArrows, (Global::kScreenHeight - mHoveredMapName.getHeight()) / 2 - 75);

	return true;
}

bool MapSelectState::exit() {
	backButton.destroy();
	createButton.destroy();
	editButton.destroy();
	selectButton.destroy();
	mTitle.destroy();
	mHoveredMapName.destroy();
	return true;
}

void MapSelectState::handleEvent(SDL_Event& e) {
	backButton.handleEvent(&e);
	createButton.handleEvent(&e);
	editButton.handleEvent(&e);
	selectButton.handleEvent(&e);

	leftArrow.handleEvent(&e);
	rightArrow.handleEvent(&e);

	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		if (leftArrow.isClicked()) {
			selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : availableMaps.size() - 1;
		} else if (rightArrow.isClicked()) {
			selectedIndex = (selectedIndex + 1) % availableMaps.size();
		}
	}

	if (e.type == SDL_EVENT_KEY_DOWN) {
		switch (e.key.key) {
			case SDLK_LEFT:
				selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : availableMaps.size() - 1;
				break;
			case SDLK_RIGHT:
				selectedIndex = (selectedIndex + 1) % availableMaps.size();
				break;
		}
	}

	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		std::string selectedMapPath = "assets/mapPresets/" + selectedMapFilePath;

		if (createButton.isClicked()) {
			setNextState(MapEditorState::get());
		} else if (editButton.isClicked()) {
			// Edit the selected map
			Global::currentMap = &availableMaps[selectedMapFilePath];
			setNextState(MapEditorState::get());
		} else if (selectButton.isClicked()) {
			// Select the map to start the game
			Global::currentMap = &availableMaps[selectedMapFilePath];
			setNextState(MainGameState::get());
		} else if (backButton.isClicked()) {
			setNextState(TitleState::get());
		}
	}
}


void MapSelectState::update() {
	if (!availableMaps.empty()) {
		auto it = std::next(availableMaps.begin(), selectedIndex);
		selectedMapFilePath = it->first;
	}

	if (!selectedMapFilePath.empty() && availableMaps.find(selectedMapFilePath) != availableMaps.end()) {
		mHoveredMapName.loadFromRenderedText(availableMaps[selectedMapFilePath].name, { 0, 0, 0, 255 });
	}
}

void MapSelectState::render() {
	int kScreenWidth = Global::kScreenWidth;
	int kScreenHeight = Global::kScreenHeight;

	mTitle.render((kScreenWidth - kScreenWidth * 0.5) / 2, titleDistanceFromTop, nullptr, kScreenWidth * 0.5, -1);

	if (!selectedMapFilePath.empty()) {
		auto mapIter = availableMaps.find(selectedMapFilePath);
		if (mapIter != availableMaps.end()) {
			// Map found, render it
			SDL_FRect targetRect = { (kScreenWidth - 300) / 2.0f, (kScreenHeight - 300) / 2.0f - 50, 300, 300 };

			// Ensure that the map can be drawn
			mapIter->second.drawOnTargetRect(targetRect);
		} else {
			// Map not found
			std::cerr << "Error: Map '" << selectedMapFilePath << "' not found in available maps." << std::endl;
		}
	} else {
		std::cerr << "Error: selectedMapName is empty." << std::endl;
	}

	mHoveredMapName.render((kScreenWidth - mHoveredMapName.getWidth()) / 2, (kScreenHeight - mHoveredMapName.getHeight()) / 2 + 125);

	backButton.render();
	createButton.render();
	editButton.render();
	selectButton.render();

	leftArrow.render();
	rightArrow.render();
}

/**
 * @brief Loads the available maps from the specified directory.
 *
 * This function scans the "assets/mapPresets" directory for JSON files,
 * attempts to load each map, and stores the successfully loaded maps
 * in the `availableMaps` dictionary. It also sets the flow field visibility
 * to false for each loaded map. If no maps are found, it logs a message.
 * If the directory does not exist or is not a directory, it logs an error.
 *
 * @note The first map found is set as the selected map.
 */
void MapSelectState::loadAvailableMaps() {
	availableMaps.clear();  // Clear existing list

	std::string mapsDirectory = "assets/mapPresets";  // Folder containing map files

	try {
		if (!fs::exists(mapsDirectory) || !fs::is_directory(mapsDirectory)) {
			std::cerr << "Error: Directory " << mapsDirectory << " does not exist or is not a directory." << std::endl;
			return;
		}

		for (const auto& entry : fs::directory_iterator(mapsDirectory)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				std::string mapFileName = entry.path().filename().string();

				// Load the map from the JSON file
				Map map;
				if (!map.loadFromJson(entry.path().string())) {
					std::cerr << "Error: Failed to load map: " << mapFileName << std::endl;
					continue;  // Skip if loading fails
				}

				// Store the loaded Map object in the dictionary
				availableMaps[mapFileName] = std::move(map);
				availableMaps[mapFileName].setFlowFieldVisibility(false);
			}
		}
	} catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error loading maps: " << e.what() << std::endl;
	}

	if (!availableMaps.empty()) {
		selectedMapFilePath = availableMaps.begin()->first;
	}
}

/**
 * @brief Formats the map file name by removing the extension and converting it to sentence case.
 *
 * This function takes a file name, removes its extension, and converts it to
 * sentence case. It capitalizes the first letter and any letter following an
 * underscore, while converting all other letters to lowercase.
 *
 * @param fileName The name of the file to format.
 * @return std::string The formatted map name.
 */
std::string MapSelectState::formatMapName(const std::string& fileName) {
	std::string formattedName = fileName;
	// Remove extension and convert to sentence case
	formattedName = formattedName.substr(0, formattedName.find_last_of('.'));
	formattedName[0] = toupper(formattedName[0]);
	for (size_t i = 1; i < formattedName.size(); ++i) {
		if (formattedName[i - 1] == '_') {
			formattedName[i] = toupper(formattedName[i]);
		} else {
			formattedName[i] = tolower(formattedName[i]);
		}
	}
	return formattedName;
}
