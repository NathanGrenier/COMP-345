/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "GameState.h"
#include <ui/Texture.h>
#include <map/Map.h>
#include <ui/LButton.h>

class MapEditorState : public GameState {
public:
	//Static accessor
	static MapEditorState* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

private:
	//Static instance
	static MapEditorState sMapEditorState;

	std::string currentSelection = "Start";
	std::string originalName;

	LButton backButton;

	//Intro background
	Texture mMessageTexture;

	Texture noOfColumnsLabel;
	Texture noOfColumnsText;
	LButton addColumn;
	LButton removeColumn;

	Texture noOfRowsLabel;
	Texture noOfRowsText;
	LButton addRow;
	LButton removeRow;

	LButton selectStartPos;
	LButton selectEndPos;
	LButton selectWallCell;
	LButton toggleFlowFieldVisibility;

	LButton saveMapButton;
	LButton renameButton;
	Texture textField;

	Texture currentMessage;

	int mouseDownStatus = 0;
	int keyDownStatus = 0;

	SDL_FRect mapView = {};

	/** @brief Pointer to the map being edited
	 *  @details nullptr if no map is currently loaded
	 */
	Map* map = nullptr;
};
