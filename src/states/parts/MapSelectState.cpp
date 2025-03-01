/// @file MapSelectState.cpp
/// @brief Implementation of the MapSelectState class.

#include <iostream>
#include <states/parts/MapSelectState.h>
#include <Global.h>
#include <filesystem>
#include <states/parts/MapEditorState.h>
#include <states/parts/MainGameState.h>
#include <ui/LButton.h>

namespace fs = std::filesystem;

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

    createButton.loadFromFile("assets/ui/CreateMap.png");
    editButton.setText("Edit Map", { 0, 0, 0 });
    selectButton.setText("Select Map", { 0, 0, 0 });

    mTitle.loadFromRenderedText("Map Selection", { 0, 0, 0 });

    const int buttonSpacing = 20;

    int totalButtonsWidth = createButton.kButtonWidth + editButton.kButtonWidth + selectButton.kButtonWidth + 2 * buttonSpacing;

    int startX = (Global::kScreenWidth - totalButtonsWidth) / 2;
    int startY = Global::kScreenHeight - createButton.kButtonHeight - buttonSpacing;

    createButton.setPosition(startX, startY);
    editButton.setPosition(startX + editButton.kButtonWidth + buttonSpacing, startY);
    selectButton.setPosition(startX + (editButton.kButtonWidth + selectButton.kButtonWidth + buttonSpacing), startY);
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
        std::string selectedMapPath = "assets/customMaps/" + availableMaps[selectedIndex];

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
    // Render the list of maps (carousel style)
    std::string displayedMap = formatMapName(availableMaps[selectedIndex]);

    mHoveredMapName.loadFromRenderedText(">  " + displayedMap + "  <", { 0, 0, 0 });  // Render map name at center
}

void MapSelectState::render() {
    int kScreenWidth = Global::kScreenWidth;
    int kScreenHeight = Global::kScreenHeight;

    mTitle.render((kScreenWidth - mTitle.getWidth()) / 2, 20);

    mHoveredMapName.render((kScreenWidth - mHoveredMapName.getWidth()) / 2, (kScreenHeight - mHoveredMapName.getHeight()) / 2);

    createButton.render();
    editButton.render();
    selectButton.render();
}

void MapSelectState::loadAvailableMaps() {
    availableMaps.clear();  // Clear existing list

    std::string mapsDirectory = "assets/customMaps";  // Folder containing map files

    try {
        if (!fs::exists(mapsDirectory) || !fs::is_directory(mapsDirectory)) {
            std::cerr << "Error: Directory " << mapsDirectory << " does not exist or is not a directory." << std::endl;
            return;
        }

        for (const auto& entry : fs::directory_iterator(mapsDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                availableMaps.push_back(entry.path().filename().string());  // Store only the file name
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