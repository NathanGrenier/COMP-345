#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <states/MapEditorState.h>
#include <Global.h>
#include <states/MapSelectState.h>

/** @class MapEditorState
 *  @brief Implementation of the MapEditorState.
 *
 *
 *  @author Nirav Patel
 */

 /// Static instance of the ExitState
MapEditorState MapEditorState::sMapEditorState;

MapEditorState* MapEditorState::get() {
	return &sMapEditorState;
}

bool MapEditorState::enter() {
	if (Global::currentMap == nullptr) {
		map = new Map(15, 15, "Default");
		mMessageTexture.loadFromFile("ui/MapCreation.png");
	} else {
		map = Global::currentMap;
		mMessageTexture.loadFromFile("ui/MapEditing.png");
	}
	originalName = map->getName();
	map->setFlowFieldVisibility(true);
	map->setCurrentRenderRect(Global::mapViewRect);

	backButton.loadFromFile("ui/LeftArrow.png");

	noOfColumnsLabel.loadFromFile("ui/Columns.png");
	noOfRowsLabel.loadFromFile("ui/Rows.png");
	addColumn.loadFromFile("ui/AddButton.png");
	removeColumn.loadFromFile("ui/RemoveButton.png");
	addRow.loadFromFile("ui/AddButton.png");
	removeRow.loadFromFile("ui/RemoveButton.png");

	selectStartPos.loadFromFile("ui/SetStartPositionButton.png");
	selectEndPos.loadFromFile("ui/SetEndPositionButton.png");
	selectWallCell.loadFromFile("ui/EditWallButton.png");
	toggleFlowFieldVisibility.loadFromFile("ui/ToggleFlowVisibilityButton.png");

	saveMapButton.loadFromFile("ui/SaveButton.png");
	renameButton.loadFromFile("ui/RenameButton.png");
	textField.loadFromRenderedText("File Name: " + map->getName(), { 255, 255, 255, 255 });

	// Map view calculations
	float availableWidth = Global::kScreenWidth - Global::viewerWidth;
	float availableHeight = Global::kScreenHeight - Global::headerHeight;

	float mapWidth = 0.8f * availableWidth;
	float mapHeight = 0.8f * availableHeight;

	float mapX = (availableWidth - mapWidth) / 2.0f;
	float mapY = Global::headerHeight + (availableHeight - mapHeight) / 2.0f;

	mapView = { mapX, mapY, mapWidth, mapHeight };

	// BUTTON SIZING AND POSITIONING
	float buttonWidth = Global::viewerWidth; // Individual button width (30% of viewerWidth)
	float squareButtonWidth = Global::viewerWidth * 0.2f;
	float labelWidth = Global::viewerWidth * 0.8f;  // Column/Row labels width (80% of viewerWidth)
	float textWidth = Global::viewerWidth * 0.2f;   // Grid size text width

	float buttonSpacing = 10.0f;
	float groupSpacing = 50.0f;

	float startX = Global::kScreenWidth - Global::viewerWidth + (Global::viewerWidth - labelWidth) / 2.0f - 50;
	float buttonStartX = startX - 5;
	float buttonStartY = Global::headerHeight;

	// Apply aspect ratio scaling
	backButton.setSizeWithAspectRatio(mMessageTexture.getHeight() * 0.75f, 0.f);
	removeColumn.setSizeWithAspectRatio(squareButtonWidth, 0.f);
	addColumn.setSizeWithAspectRatio(squareButtonWidth, 0.f);
	removeRow.setSizeWithAspectRatio(squareButtonWidth, 0.f);
	addRow.setSizeWithAspectRatio(squareButtonWidth, 0.f);

	selectStartPos.setSizeWithAspectRatio(buttonWidth, 0.f);
	selectEndPos.setSizeWithAspectRatio(buttonWidth, 0.f);
	selectWallCell.setSizeWithAspectRatio(buttonWidth, 0.f);
	toggleFlowFieldVisibility.setSizeWithAspectRatio(buttonWidth, 0.f);

	saveMapButton.setSizeWithAspectRatio(Global::viewerWidth / 2 - 5, 0.f);
	renameButton.setSizeWithAspectRatio(Global::viewerWidth / 2 - 5, 0.f);

	// Set button positions first, then apply size
	float renderedWidth = Global::kScreenWidth * 0.5f;
	float renderedHeight = (static_cast<float>(mMessageTexture.getHeight()) / mMessageTexture.getWidth()) * renderedWidth;
	float backButtonY = titleDistanceFromTop + (renderedHeight - backButton.kButtonHeight) / 2.0f;
	backButton.setPosition(backButtonDistanceFromLeft, backButtonY);

	removeColumn.setPosition(startX, buttonStartY + noOfColumnsLabel.getHeight());
	addColumn.setPosition(removeColumn.getPosition().x + removeColumn.kButtonWidth + textWidth + buttonSpacing * 9, removeColumn.getPosition().y);

	removeRow.setPosition(startX, removeColumn.getPosition().y + removeColumn.kButtonHeight + groupSpacing);
	addRow.setPosition(removeRow.getPosition().x + removeRow.kButtonWidth + textWidth + buttonSpacing * 9, removeRow.getPosition().y);

	currentMessage.loadFromRenderedText("Selected: Start", { 255, 255, 255, 255 });

	selectStartPos.setPosition(buttonStartX, addRow.getPosition().y + addRow.kButtonHeight + groupSpacing);
	selectEndPos.setPosition(buttonStartX, selectStartPos.getPosition().y + selectStartPos.kButtonHeight + buttonSpacing);
	selectWallCell.setPosition(buttonStartX, selectEndPos.getPosition().y + selectEndPos.kButtonHeight + buttonSpacing);
	toggleFlowFieldVisibility.setPosition(buttonStartX, selectWallCell.getPosition().y + selectWallCell.kButtonHeight + buttonSpacing);

	saveMapButton.setPosition(toggleFlowFieldVisibility.getPosition().x, selectWallCell.getPosition().y + selectWallCell.kButtonHeight + groupSpacing + 5);
	renameButton.setPosition(saveMapButton.getPosition().x + saveMapButton.kButtonWidth + buttonSpacing, saveMapButton.getPosition().y);

	return true;
}


bool MapEditorState::exit() {
	map = nullptr;

	return true;
}

void MapEditorState::handleEvent(SDL_Event& e) {
	if (map == nullptr) return;

	backButton.handleEvent(&e);

	addColumn.handleEvent(&e);
	removeColumn.handleEvent(&e);
	addRow.handleEvent(&e);
	removeRow.handleEvent(&e);

	selectStartPos.handleEvent(&e);
	selectEndPos.handleEvent(&e);
	selectWallCell.handleEvent(&e);
	toggleFlowFieldVisibility.handleEvent(&e);

	saveMapButton.handleEvent(&e);
	renameButton.handleEvent(&e);

	// Process selection buttons
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		if (backButton.isClicked()) {
			setNextState(MapSelectState::get());
		} else if (selectStartPos.isClicked()) {
			currentSelection = "Start";
			currentMessage.loadFromRenderedText("Selected: Start", { 255, 255, 255, 255 });
		} else if (selectEndPos.isClicked()) {
			currentSelection = "End";
			currentMessage.loadFromRenderedText("Selected: End", { 255, 255, 255, 255 });
		} else if (selectWallCell.isClicked()) {
			currentSelection = "Wall";
			currentMessage.loadFromRenderedText("Selected: Edit Wall", { 255, 255, 255, 255 });
		} else if (toggleFlowFieldVisibility.isClicked()) {
			std::string textToDisplay = "Flow Visibility: " + std::string(map->isFlowFieldVisible ? "On" : "Off");
			currentMessage.loadFromRenderedText(textToDisplay, { 255, 255, 255, 255 });
			map->toggleFlowFieldVisibility();
		} else if (renameButton.isClicked()) {
			currentSelection = "Rename";
			currentMessage.loadFromRenderedText("Selected: Rename", { 255, 255, 255, 255 });
		} else if (addColumn.isClicked() && map->cellCountX < 20) {
			map->updateMapDimensions(map->cellCountX + 1, map->cellCountY);
		} else if (removeColumn.isClicked() && map->cellCountX > 8) {
			map->updateMapDimensions(map->cellCountX - 1, map->cellCountY);
		} else if (addRow.isClicked() && map->cellCountY < 20) {
			map->updateMapDimensions(map->cellCountX, map->cellCountY + 1);
		} else if (removeRow.isClicked() && map->cellCountY > 8) {
			map->updateMapDimensions(map->cellCountX, map->cellCountY - 1);
		} else if (saveMapButton.isClicked()) {
			if (!map->isValidPath())
			{
				currentMessage.loadFromRenderedText("Invalid Path!", { 255, 255, 255, 255 });
				return;
			}
			// Get the original and current map names
			std::string originalMapPath = "assets/mapPresets/" + originalName + ".json";
			std::string newMapPath = "assets/mapPresets/" + map->getName() + ".json";

			// Attempt to delete the original map file
			std::remove(originalMapPath.c_str());

			// Attempt to delete the current map file (if it exists)
			if (originalMapPath != newMapPath && std::remove(newMapPath.c_str()) != 0) {
				std::cerr << "Error: Failed to delete the current map file: " << newMapPath << std::endl;
			}

			// Save the map as a new JSON file
			if (map->saveToJson(newMapPath)) {
				currentMessage.loadFromRenderedText("Save Success!", { 255, 255, 255, 255 });
			} else {
				std::cerr << "Failed to save the map." << std::endl;
				currentMessage.loadFromRenderedText("Save Failure!", { 255, 255, 255, 255 });
			}
		}
	}

	// Track mouse button state
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				mouseDownStatus = SDL_BUTTON_LEFT;
				break;
			case SDL_BUTTON_RIGHT:
				mouseDownStatus = SDL_BUTTON_RIGHT;
				break;
		}
	} else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT)) {
		mouseDownStatus = 0; // Reset flag
	}

	SDL_FRect currentRenderRect = map->getCurrentRenderRect();

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Calculate scaling factor
	float mapWidth = map->cellCountX * map->getPixelPerCell();
	float mapHeight = map->cellCountY * map->getPixelPerCell();
	float scaleX = currentRenderRect.w / mapWidth;
	float scaleY = currentRenderRect.h / mapHeight;
	float scale = std::min(scaleX, scaleY);

	// Convert mouse position
	Vector2D mousePosition(
		(mouseX - currentRenderRect.x) / scale / map->getPixelPerCell(),
		(mouseY - currentRenderRect.y) / scale / map->getPixelPerCell()
	);

	if (map != nullptr) {
		// Continuous placement while holding left click
		if (mouseDownStatus == SDL_BUTTON_LEFT) {
			if (currentSelection == "Start") {
				map->setSpawner((int)mousePosition.x, (int)mousePosition.y);
			} else if (currentSelection == "End") {
				map->setTarget((int)mousePosition.x, (int)mousePosition.y);
			} else if (currentSelection == "Wall") {
				map->setCellWall((int)mousePosition.x, (int)mousePosition.y, true);
			}
		} else if (mouseDownStatus == SDL_BUTTON_RIGHT) {
			if (currentSelection == "Wall") {
				map->setCellWall((int)mousePosition.x, (int)mousePosition.y, false);
			}
		}
	}

	// Handle keyboard input for renaming map
	if (currentSelection == "Rename") {
		if (e.type == SDL_EVENT_KEY_DOWN) {
			// Check for Backspace
			if (e.key.key == SDLK_BACKSPACE && !map->getName().empty()) {
				// Remove last character from the map name
				map->setName(map->getName().substr(0, map->getName().size() - 1));
			}
			// Handle "Enter" key to confirm rename
			else if (e.key.key == SDLK_RETURN) {
				currentSelection = ""; // Exit rename mode after pressing enter
				currentMessage.loadFromRenderedText("Rename confirmed", { 255, 255, 255, 255 });
			}
			// Check for printable characters (alphanumeric and others) and limit to 16 characters
			else if (e.key.key >= SDLK_SPACE && e.key.key <= SDLK_Z) {
				// Only append if the name is less than 16 characters
				if (map->getName().size() < 16) {
					char keyChar = static_cast<char>(e.key.key);
					// Check if Shift key is pressed
					if (e.key.mod & (SDL_KMOD_SHIFT)) {
						keyChar = std::toupper(keyChar);
					}
					map->setName(map->getName() + keyChar);
				}
			}
		}

		// Update textField to show the new name
		textField.loadFromRenderedText("File Name: " + map->getName(), { 255, 255, 255, 255 });
	}
}


void MapEditorState::update() {
	// Set color to black and fill the map view
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);  // Black (R=0, G=0, B=0, A=255)
	SDL_RenderFillRect(gRenderer, &mapView);

	noOfColumnsText.loadFromRenderedText(std::to_string(map->cellCountY), { 255, 255, 255, 0xFF });
	noOfRowsText.loadFromRenderedText(std::to_string(map->cellCountX), { 255, 255, 255, 0xFF });
}

void MapEditorState::render() {
	float buttonWidth = Global::viewerWidth * 0.8f;
	float buttonSpacing = 30.0f;
	float buttonHeight = 25.0f;
	float buttonStartX = Global::kScreenWidth - Global::viewerWidth - (Global::viewerWidth - buttonWidth) / 2.0f;
	float buttonStartY = Global::headerHeight + 20;

	map->drawOnTargetRect(mapView);

	mMessageTexture.render((Global::kScreenWidth - Global::kScreenWidth * 0.5) / 2, titleDistanceFromTop, nullptr, Global::kScreenWidth * 0.5, -1);

	noOfColumnsLabel.render(buttonStartX + 45, buttonStartY, nullptr, -1, buttonHeight);
	noOfRowsText.render(buttonStartX + 100, buttonStartY + 50, nullptr, -1, buttonHeight + 10);
	addColumn.render();
	removeColumn.render();

	noOfRowsLabel.render(buttonStartX + 75, buttonStartY + noOfColumnsLabel.getHeight() + addColumn.kButtonHeight, nullptr, -1, buttonHeight);
	noOfColumnsText.render(buttonStartX + 100, buttonStartY + 150, nullptr, -1, buttonHeight + 10);
	addRow.render();
	removeRow.render();

	currentMessage.render(selectStartPos.getPosition().x, addRow.getPosition().y + addRow.kButtonHeight + buttonSpacing - 15, nullptr, buttonWidth, buttonHeight);

	backButton.render();

	selectStartPos.render();
	selectEndPos.render();
	selectWallCell.render();
	toggleFlowFieldVisibility.render();

	saveMapButton.render();
	renameButton.render();
	textField.render(50, Global::kScreenHeight - textField.getHeight() - 10, nullptr, textField.getWidth(), textField.getHeight());
}
