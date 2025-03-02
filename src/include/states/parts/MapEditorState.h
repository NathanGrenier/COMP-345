/**
 * @file MainGameState.h
 * @brief The header file for the MainGameState class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include "../GameState.h"
#include <ui/LTexture.h>
#include <map/Map.h>
#include <ui/LButton.h>
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

	//Private constructor
	MapEditorState();

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;

	LTexture noOfColumnsLabel;
	LTexture noOfColumnsText;
	LButton addColumn;
	LButton removeColumn;

	LTexture noOfRowsLabel;
	LTexture noOfRowsText;
	LButton addRow;
	LButton removeRow;

	LButton selectStartPos;
	LButton selectEndPos;
	LButton selectWallCell;
	LButton eraser;

	LButton saveMapButton;
	LButton renameButton;
	LTexture textField;

	/** @brief Pointer to the map being edited
	 *  @details nullptr if no map is currently loaded
	 */
	Map* map = nullptr;
};
