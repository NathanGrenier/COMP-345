/// @file MapSelectState.h
/// @brief The header file for the MapSelectState class.
/// 
/// This class represents the state where the player selects a map before starting the game.
/// It provides UI rendering and input handling to allow the player to choose from available maps.
///
/// @author Nirav Patel
/// @date 2025-02-27

#pragma once

#include <states/GameState.h>
#include <vector>
#include <string>
#include <ui/LTexture.h>
#include <ui/LButton.h>

class MapSelectState : public GameState {
public:
    MapSelectState();
    ~MapSelectState() override;

    //Static accessor
    static MapSelectState* get();

    bool enter() override;
    bool exit() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    //Static instance
    static MapSelectState sMapSelectState;

    std::vector<std::string> availableMaps; ///< List of available maps
    int selectedIndex; ///< Index of the currently selected map

    void loadAvailableMaps(); ///< Loads available map names from the map directory
    std::string formatMapName(const std::string& fileName);

    //State title message
    LTexture mTitle;
    LTexture mHoveredMapName;

    LButton createButton;
    LButton editButton;
    LButton selectButton;
};