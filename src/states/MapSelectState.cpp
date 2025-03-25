/// @class MapSelectState
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

	bg = new ParallaxBackground();
	std::srand(std::time(0));

	for (int i = 0; i < Global::numberOfProps; ++i) {
		float randomSpeed = 5.0f + std::rand() % 11;
		bg->addLayer(randomSpeed, Global::kScreenHeight);
	}

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
	const float maxButtonWidth = (Global::kScreenWidth - (buttonSpacing * (buttonCount + 1))) / buttonCount;

	float reducedHeight = mTitle.getHeight() * 0.75f;

	backButton.setSizeWithAspectRatio(reducedHeight, 0);
	createButton.setSizeWithAspectRatio(maxButtonWidth, 0);
	editButton.setSizeWithAspectRatio(maxButtonWidth, 0);
	selectButton.setSizeWithAspectRatio(maxButtonWidth, 0);

	leftArrow.setSizeWithAspectRatio(50, 0);
	rightArrow.setSizeWithAspectRatio(50, 0);

	// Get max button height (assuming they have the same aspect ratio)
	float maxButtonHeight = std::max({ createButton.kButtonHeight, editButton.kButtonHeight, selectButton.kButtonHeight });

	// Calculate start positions
	float startX = static_cast<float>((Global::kScreenWidth - (buttonCount * maxButtonWidth + (buttonCount - 1) * buttonSpacing))) / 2;
	float startY = static_cast<float>(Global::kScreenHeight - maxButtonHeight - buttonSpacing);

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
	float totalArrowsWidth = leftArrow.kButtonWidth + rightArrow.kButtonWidth + distanceBetweenArrows;

	// Calculate the X position for the first arrow (leftArrow) to center them
	float centerX = (Global::kScreenWidth - totalArrowsWidth) / 2;

	// Set the positions for the arrows
	leftArrow.setPosition(centerX, ((Global::kScreenHeight) - mHoveredMapName.getHeight()) / 2 - 75);
	rightArrow.setPosition(centerX + leftArrow.kButtonWidth + distanceBetweenArrows, ((Global::kScreenHeight) - mHoveredMapName.getHeight()) / 2 - 75);

	for (auto& [mapName, map] : availableMaps) {
		map.setCurrentRenderRect(targetRect);
	}

	return true;
}

bool MapSelectState::exit() {
	backButton.destroy();
	createButton.destroy();
	editButton.destroy();
	selectButton.destroy();
	mTitle.destroy();
	mHoveredMapName.destroy();
	delete bg;
	bg = nullptr;

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
			if (selectedIndex > 0) {
				selectedIndex--;
			}
			else {
				selectedIndex = static_cast<int>(availableMaps.size()) - 1;
			}
		}
		else if (rightArrow.isClicked()) {
			selectedIndex = (selectedIndex + 1) % static_cast<int>(availableMaps.size());
		}

	}

	if (e.type == SDL_EVENT_KEY_DOWN) {
		switch (e.key.key) {
		case SDLK_LEFT:
			if (selectedIndex > 0) {
				selectedIndex--;
			}
			else {
				selectedIndex = static_cast<int>(availableMaps.size()) - 1;
			}
			break;
		case SDLK_RIGHT:
			selectedIndex = (selectedIndex + 1) % static_cast<int>(availableMaps.size());
			break;
		}
	}

	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		std::string selectedMapPath = "assets/mapPresets/" + selectedMapFilePath;

		if (createButton.isClicked()) {
			Global::currentMap = nullptr;  // Assign a new empty map (default constructor)
			setNextState(MapEditorState::get());
		}
		else if (editButton.isClicked()) {
			Global::currentMap = &availableMaps[selectedMapFilePath];
			setNextState(MapEditorState::get());
		}
		else if (selectButton.isClicked()) {
			Global::currentMap = &availableMaps[selectedMapFilePath];
			setNextState(MainGameState::get());
		}
		else if (backButton.isClicked()) {
			setNextState(TitleState::get());
		}
	}
}


void MapSelectState::update() {
	bg->update(0.016f);

	if (!availableMaps.empty()) {
		auto it = std::next(availableMaps.begin(), selectedIndex);
		selectedMapFilePath = it->first;
	}

	if (!selectedMapFilePath.empty() && availableMaps.find(selectedMapFilePath) != availableMaps.end()) {
		mHoveredMapName.loadFromRenderedText(availableMaps[selectedMapFilePath].name, { 255, 255, 255, 255 });
	}
}

void MapSelectState::render() {
	bg->render();

	float kScreenWidth = Global::kScreenWidth;
	float kScreenHeight = Global::kScreenHeight;

	// Render title below the map
	mTitle.render(
		(kScreenWidth - kScreenWidth * 0.5f) / 2,
		10.0f,
		nullptr,
		kScreenWidth * 0.5f,
		-1
	);

	// Render selected map
	if (!selectedMapFilePath.empty()) {
		auto mapIter = availableMaps.find(selectedMapFilePath);
		if (mapIter != availableMaps.end()) {
			mapIter->second.setCurrentRenderRect(targetRect);
			mapIter->second.drawOnTargetRect(targetRect);
		}
		else {
			std::cerr << "Error: Map '" << selectedMapFilePath << "' not found in available maps." << std::endl;
		}
	}
	else {
		std::cerr << "Error: selectedMapName is empty." << std::endl;
	}

	// Render hovered map name
	mHoveredMapName.render(
		(kScreenWidth - mHoveredMapName.getWidth()) / 2,
		targetRect.y + targetRect.h + 30
	);

	// Button rendering
	backButton.render();
	createButton.render();
	editButton.render();
	selectButton.render();

	int arrowOffset = 10;
	leftArrow.setPosition(targetRect.x - leftArrow.kButtonWidth - arrowOffset, kScreenHeight / 2 - leftArrow.kButtonHeight / 2 - 30);
	rightArrow.setPosition(targetRect.x + targetRect.w + arrowOffset, kScreenHeight / 2 - rightArrow.kButtonHeight / 2 - 30);

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
