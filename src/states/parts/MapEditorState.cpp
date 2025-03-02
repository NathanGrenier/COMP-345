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

int Global::cellWidth = 15;
int Global::cellHeight = 15;

MapEditorState* MapEditorState::get() {
	return &sMapEditorState;
}

bool MapEditorState::enter() {
	if (Global::currentMap == nullptr || Global::currentMap->getName().empty()) {
		std::cerr << "Global::currentMap was null" << std::endl;
		return false;
	}

	map = new Map(*Global::currentMap);

	map->setFlowFieldVisibility(true);

	return true;
}

bool MapEditorState::exit() {
	delete map;
	map = nullptr;
	return true;
}

void MapEditorState::handleEvent(SDL_Event& e) {
	// No event handling required
}

void MapEditorState::update() {
	// No update logic needed
}

void MapEditorState::render() {}

MapEditorState::MapEditorState() {}
