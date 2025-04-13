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
#include <map/Map.h>
#include <string>
#include <ui/Texture.h>
#include <ui/LButton.h>
#include <unordered_map>
#include <Global.h>

class MapSelectState : public GameState {
public:
	MapSelectState();

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

	std::unordered_map<std::string, Map> availableMaps;

	int selectedIndex; ///< Index of the currently selected map

	void loadAvailableMaps(); ///< Loads available map names from the map directory
	std::string formatMapName(const std::string& fileName);

	//State title message
	Texture mTitle;
	Texture mHoveredMapName;

	LButton backButton;

	LButton leftArrow;
	LButton rightArrow;

	LButton createButton;
	LButton editButton;
	LButton selectButton;

	float mapWidth = Global::kScreenWidth * 0.6;
	float mapHeight = Global::kScreenHeight * 0.6;

	SDL_FRect targetRect = {
		(Global::kScreenWidth - mapWidth) / 2.0f,
		(Global::kScreenHeight - mapHeight) / 2.0f - 25,
		mapWidth,
		mapHeight
	};
};