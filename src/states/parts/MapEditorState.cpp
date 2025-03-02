#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <states/parts/MapEditorState.h>
#include <Global.h>

/** @class MapEditorState
 *  @brief Implementation of the MapEditorState.
 *
 *
 *  @author Nirav Patel
 */

 /// Static instance of the ExitState
MapEditorState MapEditorState::sMapEditorState;
SDL_FRect mapView;

MapEditorState* MapEditorState::get() {
	return &sMapEditorState;
}
    
bool MapEditorState::enter() {
    if (Global::currentMap == nullptr) {
        Global::currentMap = new Map(15, 15, "Default");
        mMessageTexture.loadFromFile("assets/ui/MapCreation.png");
    }
    else {
        mMessageTexture.loadFromFile("assets/ui/MapEditing.png");
    }
    map = Global::currentMap;

    noOfColumnsLabel.loadFromFile("assets/ui/Columns.png");
    noOfRowsLabel.loadFromFile("assets/ui/Rows.png");
    addColumn.loadFromFile("assets/ui/AddButton.png");
    removeColumn.loadFromFile("assets/ui/RemoveButton.png");
    addRow.loadFromFile("assets/ui/AddButton.png");
    removeRow.loadFromFile("assets/ui/RemoveButton.png");

    selectStartPos.loadFromFile("assets/ui/SetStartPositionButton.png");
    selectEndPos.loadFromFile("assets/ui/SetEndPositionButton.png");
    selectWallCell.loadFromFile("assets/ui/SetWallButton.png");
    eraser.loadFromFile("assets/ui/EraseButton.png");

    saveMapButton.loadFromFile("assets/ui/SaveButton.png");
    renameButton.loadFromFile("assets/ui/RenameButton.png");
    textField.loadFromRenderedText("File Name: ", { 0, 0, 0, 255 });

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
    removeColumn.setSizeWithAspectRatio(squareButtonWidth, 0);
    addColumn.setSizeWithAspectRatio(squareButtonWidth, 0);
    removeRow.setSizeWithAspectRatio(squareButtonWidth, 0);
    addRow.setSizeWithAspectRatio(squareButtonWidth, 0);

    selectStartPos.setSizeWithAspectRatio(buttonWidth, 0);
    selectEndPos.setSizeWithAspectRatio(buttonWidth, 0);
    selectWallCell.setSizeWithAspectRatio(buttonWidth, 0);
    eraser.setSizeWithAspectRatio(buttonWidth, 0);

    saveMapButton.setSizeWithAspectRatio(Global::viewerWidth / 2 - 5, 0);
    renameButton.setSizeWithAspectRatio(Global::viewerWidth / 2 - 5, 0);

    // Set button positions first, then apply size
    removeColumn.setPosition(startX, buttonStartY + noOfColumnsLabel.getHeight());
    addColumn.setPosition(removeColumn.getPosition().x + removeColumn.kButtonWidth + textWidth + buttonSpacing * 9, removeColumn.getPosition().y);

    removeRow.setPosition(startX, removeColumn.getPosition().y + removeColumn.kButtonHeight + groupSpacing);
    addRow.setPosition(removeRow.getPosition().x + removeRow.kButtonWidth + textWidth + buttonSpacing * 9, removeRow.getPosition().y);

    selectStartPos.setPosition(buttonStartX, addRow.getPosition().y + addRow.kButtonHeight + groupSpacing);
    selectEndPos.setPosition(buttonStartX, selectStartPos.getPosition().y + selectStartPos.kButtonHeight + buttonSpacing);
    selectWallCell.setPosition(buttonStartX, selectEndPos.getPosition().y + selectEndPos.kButtonHeight + buttonSpacing);
    eraser.setPosition(buttonStartX, selectWallCell.getPosition().y + selectWallCell.kButtonHeight + buttonSpacing);

    saveMapButton.setPosition(eraser.getPosition().x, selectWallCell.getPosition().y + selectWallCell.kButtonHeight + groupSpacing + 5);
    renameButton.setPosition(saveMapButton.getPosition().x + saveMapButton.kButtonWidth + buttonSpacing, saveMapButton.getPosition().y);

    return true;
}


bool MapEditorState::exit() {
    Global::currentMap = nullptr;
    map = nullptr;

    noOfColumnsLabel.destroy();
    noOfRowsLabel.destroy();

    addColumn.destroy();
    removeColumn.destroy();
    addRow.destroy();
    removeRow.destroy();

    selectStartPos.destroy();
    selectEndPos.destroy();
    selectWallCell.destroy();
    eraser.destroy();

    saveMapButton.destroy();
    renameButton.destroy();

    mBackgroundTexture.destroy();

    mMessageTexture.destroy();
    return true;
}

void MapEditorState::handleEvent(SDL_Event& e) {
    addColumn.handleEvent(&e);
    removeColumn.handleEvent(&e);
    addRow.handleEvent(&e);
    removeRow.handleEvent(&e);

    selectStartPos.handleEvent(&e);
    selectEndPos.handleEvent(&e);
    selectWallCell.handleEvent(&e);
    eraser.handleEvent(&e);

    saveMapButton.handleEvent(&e);
    renameButton.handleEvent(&e);
}

void MapEditorState::update() {
    noOfColumnsText.loadFromRenderedText(std::to_string(map->cellCountX), { 0x00, 0x00, 0x00, 0xFF });
    noOfRowsText.loadFromRenderedText(std::to_string(map->cellCountY), { 0x00, 0x00, 0x00, 0xFF });
}

void MapEditorState::render() {
    float buttonWidth = Global::viewerWidth * 0.8f;
    float buttonSpacing = 30.0f;
    float buttonHeight = 25.0f;
    float buttonStartX = Global::kScreenWidth - Global::viewerWidth - (Global::viewerWidth - buttonWidth) / 2.0f;
    float buttonStartY = Global::headerHeight + 20;

    map->drawOnTargetRect(gRenderer, mapView);  

    mMessageTexture.render((Global::kScreenWidth - Global::kScreenWidth * 0.5) / 2, 20, nullptr, Global::kScreenWidth * 0.5, -1);

    noOfColumnsLabel.render(buttonStartX + 45, buttonStartY, nullptr, -1, buttonHeight);
    noOfColumnsText.render(buttonStartX + 100, buttonStartY + 50, nullptr, -1, buttonHeight + 10);
    addColumn.render();
    removeColumn.render();

    noOfRowsLabel.render(buttonStartX + 75, buttonStartY + noOfColumnsLabel.getHeight() + addColumn.kButtonHeight, nullptr, -1, buttonHeight);
    noOfRowsText.render(buttonStartX + 100, buttonStartY + 150, nullptr, -1, buttonHeight + 10);
    addRow.render();
    removeRow.render();

    selectStartPos.render();
    selectEndPos.render();
    selectWallCell.render();
    eraser.render();

    saveMapButton.render();
    renameButton.render();
    textField.render(50, Global::kScreenHeight - textField.getHeight() - 10, nullptr, textField.getWidth(), textField.getHeight());
}

MapEditorState::MapEditorState() {}
