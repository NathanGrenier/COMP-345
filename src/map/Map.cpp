/**
 * @file Map.cpp
 * @brief Implementation of the Map class
 */

#include <map/Map.h>
#include <util/TextureLoader.h>
#include <queue>
#include <SDL3/SDL_log.h>
#include <iostream>
#include <Global.h>

int Map::PIXELS_PER_CELL = 24;

void Map::calculatePixelsPerCell() {
	// Perform the calculation based on Global's static parameters
	PIXELS_PER_CELL = (Global::kScreenWidth - Global::viewerWidth) / Global::cellWidth;

	// You may want to handle edge cases, like division by zero, or unexpected values.
	if (Global::cellWidth == 0) {
		// Handle error (e.g., set PIXELS_PER_CELL to a default value or throw an exception)
		PIXELS_PER_CELL = 0; // Or some default fallback value
	}
}
 /**
  * @brief Constructs a new Map object.
  *
  * @param renderer The SDL renderer used for loading textures.
  * @param setCellCountX The number of cells along the X-axis.
  * @param setCellCountY The number of cells along the Y-axis.
  * @details Initializes cell textures, creates a grid of cells, sets the default target at the center,
  *          and calculates the initial flow field.
  */
Map::Map(SDL_Renderer* renderer, int setCellCountX, int setCellCountY) :
	cellCountX(setCellCountX), cellCountY(setCellCountY) {
	calculatePixelsPerCell();
	textureCellWall = TextureLoader::loadTexture(renderer, "map/cell-wall.bmp");
	textureCellTarget = TextureLoader::loadTexture(renderer, "map/cell-target.bmp");
	textureCellSpawner = TextureLoader::loadTexture(renderer, "map/cell-spawner.bmp");

	textureCellEmpty = TextureLoader::loadTexture(renderer, "map/cell-empty.bmp");
	textureCellArrowUp = TextureLoader::loadTexture(renderer, "map/cell-arrow-up.bmp");
	textureCellArrowDown = TextureLoader::loadTexture(renderer, "map/cell-arrow-down.bmp");
	textureCellArrowLeft = TextureLoader::loadTexture(renderer, "map/cell-arrow-left.bmp");
	textureCellArrowRight = TextureLoader::loadTexture(renderer, "map/cell-arrow-right.bmp");

	cells.clear();
	cells.reserve(cellCountX * cellCountY);
	for (int y = 0; y < cellCountY; y++) {
		for (int x = 0; x < cellCountX; x++) {
			Cell cell;
			cell.x = x;
			cell.y = y;
			cells.push_back(cell);
		}
	}

	// Set default target at the center
	int centerX = cellCountX / 2;
	int centerY = cellCountY / 2;
	setTarget(centerX, centerY);

	calculateFlowField();
}

/**
 * @brief Draws all cells in the map.
 *
 * @param renderer The SDL renderer used for drawing.
 * @details Iterates through all cells and draws them using their respective textures based on type and flow direction.
 */
void Map::draw(SDL_Renderer* renderer) {
	// Draw the arrow and empty cells.
	for (const auto& cell : cells) {
		drawCell(renderer, cell);
	}
}

/**
 * @brief Draws an individual cell.
 *
 * @param renderer The SDL renderer used for drawing.
 * @param cell The cell to be drawn.
 * @details Selects the appropriate texture based on cell properties.
 */
void Map::drawCell(SDL_Renderer* renderer, const Cell& cell) {
	// Default texture is the empty cell.
	SDL_Texture* textureSelected = textureCellEmpty;

	// Choose arrow texture based on the cell’s flow direction.
	if (cell.isSpawner) {	// Spawner still has a flow direction, but is drawn as a different texture.
		textureSelected = textureCellSpawner;
	} else if (cell.flowDirectionX == 0 && cell.flowDirectionY == -1) { // Up
		textureSelected = textureCellArrowUp;
	} else if (cell.flowDirectionX == 1 && cell.flowDirectionY == 0) { // Right
		textureSelected = textureCellArrowRight;
	} else if (cell.flowDirectionX == 0 && cell.flowDirectionY == 1) { // Down
		textureSelected = textureCellArrowDown;
	} else if (cell.flowDirectionX == -1 && cell.flowDirectionY == 0) { // Left
		textureSelected = textureCellArrowLeft;
	} else if (cell.isTarget) {
		textureSelected = textureCellTarget;
	} else if (cell.isWall) {
		textureSelected = textureCellWall;
	}

	if (textureSelected != nullptr) {
		SDL_FRect rect = { cell.x * PIXELS_PER_CELL, cell.y * PIXELS_PER_CELL, PIXELS_PER_CELL, PIXELS_PER_CELL };
		SDL_RenderTexture(renderer, textureSelected, nullptr, &rect);
	}
}

/**
 * @brief Checks if a coordinate is within map boundaries.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @return true If the coordinates are within valid bounds.
 * @return false If the coordinates are out of bounds.
 */
bool Map::isInbounds(int x, int y) {
	int index = x + y * cellCountX;
	return (index >= 0 && index < static_cast<int>(cells.size()) &&
			x >= 0 && x < cellCountX &&
			y >= 0 && y < cellCountY);
}

/**
 * @brief Checks if a cell is a wall.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @return true If the cell is a wall.
 * @return false If the cell is not a wall or coordinates are invalid.
 */
bool Map::isCellWall(int x, int y) {
	int index = x + y * cellCountX;
	if (isInbounds(x, y)) {
		return cells[index].isWall;
	}
	return false;
}

/**
 * @brief Sets a cell's wall status.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @param setWall true to make the cell a wall, false to clear.
 * @details Does nothing if coordinates are invalid or cell is target/spawner. Recalculates flow field after change.
 */
void Map::setCellWall(int x, int y, bool setWall) {
	int index = x + y * cellCountX;
	// Prevent setting a wall where a cell is marked as target or spawn.
	if (isInbounds(x, y) && !isTarget(x, y) && !isSpawner(x, y)) {
		cells[index].isWall = setWall;
		calculateFlowField();
	}
}

/**
 * @brief Checks if a cell is a target.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @return true If the cell is a target.
 * @return false If the cell is not a target or coordinates are invalid.
 */
bool Map::isTarget(int x, int y) {
	int index = x + y * cellCountX;
	if (isInbounds(x, y)) {
		return cells[index].isTarget;
	}
	return false;
}

/**
 * @brief Sets a cell as the new target.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @details Clears existing targets, sets new target, marks it non-wall/non-spawner, and recalculates flow field.
 */
void Map::setTarget(int x, int y) {
	if (!isInbounds(x, y))
		return;

	int index = x + y * cellCountX;

	// Clear all existing targets
	for (auto& cell : cells) {
		cell.isTarget = false;
	}
	// Set the new target
	cells[index].isTarget = true;
	cells[index].isWall = false;
	cells[index].isSpawner = false;
	calculateFlowField();
}

/**
 * @brief Checks if a cell is a spawner.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @return true If the cell is a spawner.
 * @return false If the cell is not a spawner or coordinates are invalid.
 */
bool Map::isSpawner(int x, int y) {
	int index = x + y * cellCountX;
	if (isInbounds(x, y)) {
		return cells[index].isSpawner;
	}
	return false;
}

/**
 * @brief Sets a cell as the new spawner.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @details Clears existing spawners, sets new spawner, marks it non-wall, and recalculates flow field.
 *          Does nothing if coordinates are invalid or cell is a target.
 */
void Map::setSpawner(int x, int y) {
	if (!isInbounds(x, y) || isTarget(x, y))
		return;

	int index = x + y * cellCountX;
	// Clear all existing spawns
	for (auto& cell : cells) {
		cell.isSpawner = false;
	}
	cells[index].isSpawner = true;
	cells[index].isWall = false;
	calculateFlowField();
}

/**
 * @brief Gets the position of the first target cell found.
 *
 * @return Vector2D Position of the target in cell coordinates.
 * @details Falls back to center coordinates if no target found and sets one there.
 */
Vector2D Map::getTargetPos() {
	// Return the first target cell found (default is center)
	for (const auto& cell : cells) {
		if (cell.isTarget) {
			return Vector2D(static_cast<float>(cell.x), static_cast<float>(cell.y));
		}
	}
	// Fallback if no target found (shouldn't occur with default setup)
	setTarget(cellCountX / 2, cellCountY / 2);
	return Vector2D(static_cast<float>(cellCountX / 2), static_cast<float>(cellCountY / 2));
}

/**
 * @brief Recalculates the flow field for pathfinding.
 *
 * @details Resets all flow data, calculates distances from targets using BFS,
 *          then determines optimal flow directions for each cell.
 */
void Map::calculateFlowField() {
	// Reset flow data for all cells
	for (auto& cell : cells) {
		cell.flowDirectionX = 0;
		cell.flowDirectionY = 0;
		cell.flowDistance = flowDistanceMax;
	}
	calculateDistances();
	calculateFlowDirections();
}

/**
 * @brief Calculates distance values from target cells using BFS.
 *
 * @details Initializes queue with target cell (distance 0), propagates distances
 *          to all reachable non-wall cells. Unreachable cells keep flowDistanceMax.
 */
void Map::calculateDistances() {
	std::queue<int> indicesToCheck;

	// Initialize queue with all target cells
	for (int i = 0; i < static_cast<int>(cells.size()); ++i) {
		if (cells[i].isTarget) {
			cells[i].flowDistance = 0;
			indicesToCheck.push(i);
		}
	}

	const int neighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

	while (!indicesToCheck.empty()) {
		int indexCurrent = indicesToCheck.front();
		indicesToCheck.pop();

		for (int count = 0; count < 4; count++) {
			int neighborX = neighbors[count][0] + cells[indexCurrent].x;
			int neighborY = neighbors[count][1] + cells[indexCurrent].y;
			int indexNeighbor = neighborX + neighborY * cellCountX;

			if (isInbounds(neighborX, neighborY) && !cells[indexNeighbor].isWall) {
				if (cells[indexNeighbor].flowDistance == flowDistanceMax) {
					cells[indexNeighbor].flowDistance = cells[indexCurrent].flowDistance + 1;
					indicesToCheck.push(indexNeighbor);
				}
			}
		}
	}
}

/**
 * @brief Determines flow directions based on calculated distances.
 *
 * @details For each cell, sets flow direction towards the neighbor with the smallest distance value.
 */
void Map::calculateFlowDirections() {
	const int neighbors[][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

	for (int indexCurrent = 0; indexCurrent < static_cast<int>(cells.size()); indexCurrent++) {
		if (cells[indexCurrent].flowDistance != flowDistanceMax) {
			unsigned char flowFieldBest = cells[indexCurrent].flowDistance;

			for (int count = 0; count < 4; count++) {
				int offsetX = neighbors[count][0];
				int offsetY = neighbors[count][1];

				int neighborX = offsetX + (indexCurrent % cellCountX);
				int neighborY = offsetY + (indexCurrent / cellCountX);
				int indexNeighbor = neighborX + neighborY * cellCountX;

				if (isInbounds(neighborX, neighborY)) {
					if (cells[indexNeighbor].flowDistance < flowFieldBest) {
						flowFieldBest = cells[indexNeighbor].flowDistance;
						cells[indexCurrent].flowDirectionX = offsetX;
						cells[indexCurrent].flowDirectionY = offsetY;
					}
				}
			}
		}
	}
}

/**
 * @brief Gets the normalized flow direction vector for a cell.
 *
 * @param x X-coordinate of the cell.
 * @param y Y-coordinate of the cell.
 * @return Vector2D Normalized flow direction vector (zero vector if invalid).
 */
Vector2D Map::getFlowNormal(int x, int y) {
	int index = x + y * cellCountX;
	if (index >= 0 && index < static_cast<int>(cells.size()) && isInbounds(x, y))
		return Vector2D(static_cast<float>(cells[index].flowDirectionX),
						static_cast<float>(cells[index].flowDirectionY)).normalize();
	return Vector2D();
}

/**
 * @brief Checks if a valid path exists from spawner to target.
 *
 * @return true If spawner exists and is reachable (valid flow path to target).
 * @return false If no spawner or spawner is unreachable.
 */
bool Map::isValidPath() {
	// Find the spawn cell
	int spawnIndex = -1;
	for (int i = 0; i < static_cast<int>(cells.size()); ++i) {
		if (cells[i].isSpawner) {
			spawnIndex = i;
			break;
		}
	}

	// If no spawn exists or spawn is unreachable, return false
	if (spawnIndex == -1 || cells[spawnIndex].flowDistance == flowDistanceMax)
		return false;

	return true;
}