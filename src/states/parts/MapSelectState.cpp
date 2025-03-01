/// @file MapSelectState.cpp
/// @brief Implementation of the MapSelectState class.

#include <iostream>
#include <states/parts/MapSelectState.h>
#include <Global.h>
#include <filesystem>
#include <states/parts/MapEditorState.h>
#include <states/parts/MainGameState.h>
#include <ui/LButton.h>
#include <ui/LTexture.h>
#include <map/Map.h>

namespace fs = std::filesystem;

std::string selectedMapName;

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

    // Load button textures
    createButton.loadFromFile("assets/ui/CreateMap.png");
    editButton.loadFromFile("assets/ui/EditMap.png");
    selectButton.loadFromFile("assets/ui/SelectMap.png");

    // Set sizes dynamically based on screen width
    constexpr int buttonCount = 3;
    const int buttonSpacing = 20;
    const int maxButtonWidth = (Global::kScreenWidth - (buttonSpacing * (buttonCount + 1))) / buttonCount;

    createButton.setSizeWithAspectRatio(maxButtonWidth, 0);
    editButton.setSizeWithAspectRatio(maxButtonWidth, 0);
    selectButton.setSizeWithAspectRatio(maxButtonWidth, 0);

    // Get max button height (assuming they have the same aspect ratio)
    int maxButtonHeight = std::max({ createButton.kButtonHeight, editButton.kButtonHeight, selectButton.kButtonHeight });

    // Calculate start positions
    int startX = (Global::kScreenWidth - (buttonCount * maxButtonWidth + (buttonCount - 1) * buttonSpacing)) / 2;
    int startY = Global::kScreenHeight - maxButtonHeight - buttonSpacing;

    // Set positions dynamically
    createButton.setPosition(startX, startY);
    editButton.setPosition(startX + maxButtonWidth + buttonSpacing, startY);
    selectButton.setPosition(startX + 2 * (maxButtonWidth + buttonSpacing), startY);

    mTitle.loadFromFile("assets/ui/MapSelectionMessage.png");

    return true;
}

bool MapSelectState::exit() {
    return true;
}

void MapSelectState::handleEvent(SDL_Event& e) {
    createButton.handleEvent(&e);
    editButton.handleEvent(&e);
    selectButton.handleEvent(&e);

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
        std::string selectedMapPath = "assets/mapPresets/" + selectedMapName;

        if (createButton.isClicked()) {
            setNextState(MapEditorState::get());
        }
        else if (editButton.isClicked()) {
            // Edit the selected map
            currentMap = selectedMapPath;
            setNextState(MapEditorState::get());
        }
        else if (selectButton.isClicked()) {
            // Select the map to start the game
            currentMap = selectedMapPath;
            setNextState(MainGameState::get());
        }
    }
}


void MapSelectState::update() {
    if (!availableMaps.empty()) {
        auto it = std::next(availableMaps.begin(), selectedIndex);
        selectedMapName = it->first;
    }

    if (!selectedMapName.empty() && availableMaps.find(selectedMapName) != availableMaps.end()) {
        std::string displayedMap = formatMapName(selectedMapName);
        mHoveredMapName.loadFromRenderedText(">  " + displayedMap + "  <", { 0, 0, 0 });  // Render map name at center
    }
}

void MapSelectState::render() {
    int kScreenWidth = Global::kScreenWidth;
    int kScreenHeight = Global::kScreenHeight;

    mTitle.render((kScreenWidth - kScreenWidth * 0.5) / 2, 20, nullptr, kScreenWidth * 0.5, -1);

    // Debug output for checking selectedMapName and map existence
    if (!selectedMapName.empty()) {
        auto mapIter = availableMaps.find(selectedMapName);
        if (mapIter != availableMaps.end()) {
            // Map found, render it
            SDL_FRect targetRect = { (kScreenWidth - 300) / 2.0f, (kScreenHeight - 300) / 2.0f - 50, 300, 300 };
        
            // Debug output to check map object and rendering process
            std::cout << "Rendering map: " << selectedMapName << std::endl;
        
            // Ensure that the map can be drawn
            mapIter->second.drawOnTargetRect(gRenderer, targetRect);
        } else {
            // Map not found
            std::cerr << "Error: Map '" << selectedMapName << "' not found in available maps." << std::endl;
        }
    } else {
        std::cerr << "Error: selectedMapName is empty." << std::endl;
    }

    mHoveredMapName.render((kScreenWidth - mHoveredMapName.getWidth()) / 2, (kScreenHeight - mHoveredMapName.getHeight()) / 2 + 150);

    createButton.render();
    editButton.render();
    selectButton.render();
}


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
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading maps: " << e.what() << std::endl;
    }

    if (availableMaps.empty()) {
        std::cout << "No maps found in " << mapsDirectory << std::endl;
    }
    else {
        // Set the first map as the selected one
        selectedMapName = availableMaps.begin()->first;
        std::cout << "First map set as selected: " << availableMaps.begin()->first << std::endl;
    }
}


std::string MapSelectState::formatMapName(const std::string& fileName) {
    std::string formattedName = fileName;
    // Remove extension and convert to sentence case
    formattedName = formattedName.substr(0, formattedName.find_last_of('.'));
    formattedName[0] = toupper(formattedName[0]);
    for (size_t i = 1; i < formattedName.size(); ++i) {
        if (formattedName[i - 1] == '_') {
            formattedName[i] = toupper(formattedName[i]);
        }
        else {
            formattedName[i] = tolower(formattedName[i]);
        }
    }
    return formattedName;
}
