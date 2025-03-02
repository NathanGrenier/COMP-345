/**
 * @file Map.cpp
 * @brief Implementation of the Map class
 */

#include <map/Map.h>
#include <util/TextureLoader.h>
#include <queue>
#include <SDL3/SDL_log.h>
#include <iostream>
#include <fstream>
#include <Global.h>
#include <nlohmann/json.hpp>

 /**
  * @brief Default Map constructor.
  */
Map::Map() {
	cellCountX = 15;
	cellCountY = 15;

	calculatePixelsPerCell();

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
 * @brief Constructs a new Map object.
 *
 * @param renderer The SDL renderer used for loading textures.
 * @param setCellCountX The number of cells along the X-axis.
 * @param setCellCountY The number of cells along the Y-axis.
 * @details Initializes cell textures, creates a grid of cells, sets the default target at the center,
 *          and calculates the initial flow field.
 */
Map::Map(int setCellCountX, int setCellCountY, std::string name) :
	cellCountX(setCellCountX), cellCountY(setCellCountY), name(name) {
	calculatePixelsPerCell();

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
 * @brief Draws an individual cell.
 *
 * @param renderer The SDL renderer used for drawing.
 * @param cell The cell to be drawn.
 * @details Selects the appropriate texture based on cell properties.
 */
 /**
  * @brief Draws an individual cell.
  *
  * @param renderer The SDL renderer used for drawing.
  * @param cell The cell to be drawn.
  * @param rect The rectangle where the cell should be rendered.
  * @details Selects the appropriate texture based on cell properties and draws it within the specified rectangle.
  */
void Map::drawCell(SDL_Renderer* renderer, const Cell& cell, const SDL_FRect& rect) {
	// Default texture is the empty cell.
	SDL_Texture* textureSelected = textureCellEmpty;

	// Choose texture based on the cell’s properties.
	if (cell.isSpawner) {
		// Spawner cell
		textureSelected = textureCellSpawner;
	} else if (cell.isTarget) {
		// Target cell
		textureSelected = textureCellTarget;
	} else if (cell.isWall) {
		// Wall cell
		textureSelected = textureCellWall;
	} else if (cell.flowDirectionX != 0 || cell.flowDirectionY != 0) {
		if (isFlowFieldVisible) {
			// Cell with a flow direction (arrow)
			if (cell.flowDirectionX == 0 && cell.flowDirectionY == -1) {
				// Up
				textureSelected = textureCellArrowUp;
			} else if (cell.flowDirectionX == 1 && cell.flowDirectionY == 0) {
				// Right
				textureSelected = textureCellArrowRight;
			} else if (cell.flowDirectionX == 0 && cell.flowDirectionY == 1) {
				// Down
				textureSelected = textureCellArrowDown;
			} else if (cell.flowDirectionX == -1 && cell.flowDirectionY == 0) {
				// Left
				textureSelected = textureCellArrowLeft;
			}
		} else if (cell.isOnPath) {
			// Cell with a flow direction (arrow)
			if (cell.flowDirectionX == 0 && cell.flowDirectionY == -1) {
				// Up
				textureSelected = textureCellArrowUp;
			} else if (cell.flowDirectionX == 1 && cell.flowDirectionY == 0) {
				// Right
				textureSelected = textureCellArrowRight;
			} else if (cell.flowDirectionX == 0 && cell.flowDirectionY == 1) {
				// Down
				textureSelected = textureCellArrowDown;
			} else if (cell.flowDirectionX == -1 && cell.flowDirectionY == 0) {
				// Left
				textureSelected = textureCellArrowLeft;
			}
		}
	}

	// Draw the selected texture within the specified rectangle
	if (textureSelected != nullptr) {
		SDL_RenderTexture(renderer, textureSelected, nullptr, &rect);
	}
}

std::pair<int, int> Map::getCellFromPosition(float x, float y, const SDL_FRect& targetRect) const {
	float cellW = targetRect.w / cellCountX;
	float cellH = targetRect.h / cellCountY;
	int cellX = static_cast<int>((x - targetRect.x) / cellW);
	int cellY = static_cast<int>((y - targetRect.y) / cellH);
	if (cellX >= 0 && cellX < cellCountX && cellY >= 0 && cellY < cellCountY) {
		return { cellX, cellY };
	}
	return { -1, -1 }; // Out of bounds
}

SDL_FPoint Map::getCellCenter(int x, int y, const SDL_FRect& targetRect) const {
	SDL_FRect cellRect = scaleCellRect(cells[x + y * cellCountX], targetRect);
	return { cellRect.x + cellRect.w / 2.0f, cellRect.y + cellRect.h / 2.0f };
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
 * @param targetRect The rectangle where the map is being rendered.
 * @return SDL_FRect Position of the target in scaled coordinates.
 * @details Falls back to center coordinates if no target found and sets one there.
 */
SDL_FRect Map::getTargetPos(const SDL_FRect& targetRect) {
	for (const auto& cell : cells) {
		if (cell.isTarget) {
			return scaleCellRect(cell, targetRect);
		}
	}

	// Fallback if no target found (shouldn't occur with default setup)
	setTarget(cellCountX / 2, cellCountY / 2);
	return scaleCellRect(cells[cellCountX / 2 + cellCountY / 2 * cellCountX], targetRect);
}

/**
 * @brief Gets the position of the first spawner cell found.
 *
 * @param targetRect The rectangle where the map is being rendered.
 * @return SDL_FRect Position of the spawner in scaled coordinates.
 * @details Returns a default position if no spawner found.
 */
SDL_FRect Map::getSpawnerPos(const SDL_FRect& targetRect) {
	for (const auto& cell : cells) {
		if (cell.isSpawner) {
			return scaleCellRect(cell, targetRect);
		}
	}

	// Default if no spawner
	return SDL_FRect{
		targetRect.x + (targetRect.w - cellCountX * PIXELS_PER_CELL * std::min(targetRect.w / (cellCountX * PIXELS_PER_CELL), targetRect.h / (cellCountY * PIXELS_PER_CELL))) / 2.0f,
		targetRect.y + (targetRect.h - cellCountY * PIXELS_PER_CELL * std::min(targetRect.w / (cellCountX * PIXELS_PER_CELL), targetRect.h / (cellCountY * PIXELS_PER_CELL))) / 2.0f,
		static_cast<float>(PIXELS_PER_CELL) * std::min(targetRect.w / (cellCountX * PIXELS_PER_CELL), targetRect.h / (cellCountY * PIXELS_PER_CELL)),
		static_cast<float>(PIXELS_PER_CELL) * std::min(targetRect.w / (cellCountX * PIXELS_PER_CELL), targetRect.h / (cellCountY * PIXELS_PER_CELL))
	};
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
	walkPath();
	notifyObservers();
}

void Map::toggleFlowFieldVisibility() {
	isFlowFieldVisible = !isFlowFieldVisible;
}

void Map::setFlowFieldVisibility(bool value) {
	isFlowFieldVisible = value;
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

nlohmann::json loadMapData(const std::string& filePath) {
	// Open the file
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filePath << std::endl;
		return {};  // Return an empty json object on failure
	}

	// Attempt to parse the file into a json object
	try {
		nlohmann::json mapData;
		file >> mapData;

		// Ensure the file content is valid JSON
		if (file.fail()) {
			std::cerr << "Error: Failed to parse the JSON from file " << filePath << std::endl;
			return {};  // Return an empty json object if parsing fails
		}

		return mapData;
	} catch (const nlohmann::json::parse_error& e) {
		std::cerr << "Error: Parsing failed. Exception: " << e.what() << std::endl;
		return {};  // Return an empty json object if an exception occurs
	}
}

bool Map::saveToJson(const std::string& filePath) {
	// Create a JSON object to store map data
	nlohmann::json mapData;

	// Set map metadata
	mapData["name"] = this->name;
	mapData["width"] = this->cellCountX;
	mapData["height"] = this->cellCountY;

	// Set walls
	for (const auto& cell : cells) {
		if (cell.isWall) {
			nlohmann::json wall;
			wall["x"] = cell.x;
			wall["y"] = cell.y;
			mapData["walls"].push_back(wall);
		} else if (cell.isSpawner) {
			mapData["start"]["x"] = cell.x;
			mapData["start"]["y"] = cell.y;
		} else if (cell.isTarget) {
			mapData["end"]["x"] = cell.x;
			mapData["end"]["y"] = cell.y;
		}
	}

	// Open the file for writing
	std::ofstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filePath << std::endl;
		return false; // Return false if the file could not be opened
	}

	// Write the JSON object to the file
	try {
		file << mapData.dump(4);  // Indented with 4 spaces for readability
	} catch (const std::exception& e) {
		std::cerr << "Error: Failed to write to file " << filePath << ". Exception: " << e.what() << std::endl;
		return false; // Return false if an error occurs during writing
	}

	// Successfully saved the map
	return true;
}


/**
 * @brief Loads and initializes the map from a JSON file.
 *
 * @param filePath The path to the JSON file.
 * @return true If the map was successfully loaded and initialized.
 * @return false If the file could not be opened or parsed.
 */
bool Map::loadFromJson(const std::string& filePath) {
	// Load the JSON file
	nlohmann::json mapData = loadMapData(filePath);
	if (mapData.empty()) {
		return false; // Failed to load or parse the JSON file
	}

	// Set map metadata
	this->name = mapData["name"];
	this->filePath = filePath;

	// Set map dimensions
	cellCountX = mapData["width"];
	cellCountY = mapData["height"];

	// Clear existing cells and reserve space for new ones
	cells.clear();
	cells.reserve(cellCountX * cellCountY);

	// Initialize the grid of cells
	for (int y = 0; y < cellCountY; y++) {
		for (int x = 0; x < cellCountX; x++) {
			Cell cell;
			cell.x = x;
			cell.y = y;
			cell.isWall = false;
			cell.isSpawner = false;
			cell.isTarget = false;
			cells.push_back(cell);
		}
	}

	// Set the spawner (start position)
	int spawnX = mapData["start"]["x"];
	int spawnY = mapData["start"]["y"];
	setSpawner(spawnX, spawnY);

	// Set the target (end position)
	int targetX = mapData["end"]["x"];
	int targetY = mapData["end"]["y"];
	setTarget(targetX, targetY);

	// Set walls
	for (const auto& wall : mapData["walls"]) {
		int wallX = wall["x"];
		int wallY = wall["y"];
		setCellWall(wallX, wallY, true);
	}

	// Recalculate the flow field
	calculateFlowField();

	return true;
}

SDL_FRect Map::getCurrentRenderRect() {
	return currentRenderRect;
}

void Map::setCurrentRenderRect(SDL_FRect newTargetRect) {
	currentRenderRect = newTargetRect;
}

SDL_FRect Map::getPixelPerCell() {
	SDL_FRect currentMapRect = getCurrentRenderRect();
	return SDL_FRect{ 0, 0, currentMapRect.w / cellCountX, currentMapRect.h / cellCountY };
}

void Map::calculatePixelsPerCell() {
	// Perform the calculation based on Global's static parameters
	PIXELS_PER_CELL = (Global::kScreenWidth - Global::viewerWidth) / cellCountX;

	textureCellWall = TextureLoader::loadTexture(gRenderer, "map/cell-wall.bmp");
	textureCellTarget = TextureLoader::loadTexture(gRenderer, "map/cell-target.bmp");
	textureCellSpawner = TextureLoader::loadTexture(gRenderer, "map/cell-spawner.bmp");

	textureCellEmpty = TextureLoader::loadTexture(gRenderer, "map/cell-empty.bmp");
	textureCellArrowUp = TextureLoader::loadTexture(gRenderer, "map/cell-arrow-up.bmp");
	textureCellArrowDown = TextureLoader::loadTexture(gRenderer, "map/cell-arrow-down.bmp");
	textureCellArrowLeft = TextureLoader::loadTexture(gRenderer, "map/cell-arrow-left.bmp");
	textureCellArrowRight = TextureLoader::loadTexture(gRenderer, "map/cell-arrow-right.bmp");
}

/**
 * @brief Draws the map scaled to fit within a target rectangle.
 *
 * @param renderer The SDL renderer used for drawing.
 * @param targetRect The rectangle where the map should be rendered.
 * @details Scales the map to fit within the target rectangle while maintaining aspect ratio.
 */
void Map::drawOnTargetRect(SDL_Renderer* renderer, const SDL_FRect& targetRect) {
	calculatePixelsPerCell();

	// Calculate the scaling factor to fit the map within the target rectangle
	float mapWidth = cellCountX * PIXELS_PER_CELL;
	float mapHeight = cellCountY * PIXELS_PER_CELL;

	float scaleX = targetRect.w / mapWidth;
	float scaleY = targetRect.h / mapHeight;
	float scale = std::min(scaleX, scaleY); // Maintain aspect ratio

	// Calculate the offset to center the map within the target rectangle
	float offsetX = targetRect.x + (targetRect.w - mapWidth * scale) / 2.0f;
	float offsetY = targetRect.y + (targetRect.h - mapHeight * scale) / 2.0f;

	// Update the currentRenderRect to track where the map is being drawn
	currentRenderRect = {
		offsetX,
		offsetY,
		mapWidth * scale,
		mapHeight * scale
	};

	// Draw each cell, scaled to fit the target rectangle
	for (const auto& cell : cells) {
		SDL_FRect cellRect = {
			offsetX + cell.x * PIXELS_PER_CELL * scale,
			offsetY + cell.y * PIXELS_PER_CELL * scale,
			PIXELS_PER_CELL * scale,
			PIXELS_PER_CELL * scale
		};

		// Draw the cell using the existing drawCell method
		drawCell(renderer, cell, cellRect);
	}
}

/**
 * @brief Updates the map dimensions and adjusts the cells accordingly.
 *
 * @param newCellCountX The new number of cells along the X-axis.
 * @param newCellCountY The new number of cells along the Y-axis.
 * @details Updates the `cells` member by resizing it to the new dimensions and adjusting the positions of existing cells.
 */
void Map::updateMapDimensions(int newCellCountX, int newCellCountY) {
	// Keep the current dimensions until we finish modifying cells
	int oldCellCountX = cellCountX;
	int oldCellCountY = cellCountY;

	// Update the new dimensions
	cellCountX = newCellCountX;
	cellCountY = newCellCountY;

	PIXELS_PER_CELL = (Global::kScreenWidth - Global::viewerWidth) / cellCountX;

	// Create a new vector for the cells with the new size
	std::vector<Cell> newCells(cellCountX * cellCountY);

	// Copy data from the old cells to the new cells within the valid bounds
	for (int y = 0; y < std::min(oldCellCountY, newCellCountY); ++y) {
		for (int x = 0; x < std::min(oldCellCountX, newCellCountX); ++x) {
			int oldIndex = x + y * oldCellCountX;
			int newIndex = x + y * cellCountX;
			newCells[newIndex] = cells[oldIndex];
		}
	}

	// Initialize any new cells in the newly added area (bottom and right)
	for (int y = 0; y < newCellCountY; ++y) {
		for (int x = 0; x < newCellCountX; ++x) {
			int index = x + y * cellCountX;
			// Ensure that we initialize only newly added cells (outside old bounds)
			if (x >= oldCellCountX || y >= oldCellCountY) {
				newCells[index].x = x;
				newCells[index].y = y;
				newCells[index].isWall = false;
				newCells[index].isSpawner = false;
				newCells[index].isTarget = false;
			}
		}
	}

	// Replace the old cells vector with the newly created one
	cells = std::move(newCells);

	// Recalculate flow field based on the new grid dimensions
	calculateFlowField();
}


/**
 * @brief Scales the cell rectangle based on the target rendering rectangle.
 *
 * @param cell The cell to be scaled.
 * @param targetRect The rectangle where the map is being rendered.
 * @return SDL_FRect Scaled rectangle of the cell.
 */
SDL_FRect Map::scaleCellRect(const Cell& cell, const SDL_FRect& targetRect) const {
	float mapWidth = cellCountX * PIXELS_PER_CELL;
	float mapHeight = cellCountY * PIXELS_PER_CELL;

	float scaleX = targetRect.w / mapWidth;
	float scaleY = targetRect.h / mapHeight;
	float scale = std::min(scaleX, scaleY); // Maintain aspect ratio

	float offsetX = targetRect.x + (targetRect.w - mapWidth * scale) / 2.0f;
	float offsetY = targetRect.y + (targetRect.h - mapHeight * scale) / 2.0f;

	return SDL_FRect{
		offsetX + static_cast<float>(cell.x) * PIXELS_PER_CELL * scale,
		offsetY + static_cast<float>(cell.y) * PIXELS_PER_CELL * scale,
		static_cast<float>(PIXELS_PER_CELL) * scale,
		static_cast<float>(PIXELS_PER_CELL) * scale
	};
}

std::vector<Map::Cell> Map::walkPath() {
	// Reset all cells
	for (auto& cell : cells) {
		cell.isOnPath = false;
	}

	std::vector<Cell> path;

	// Find the spawner cell
	int spawnerIndex = -1;
	for (int i = 0; i < static_cast<int>(cells.size()); ++i) {
		if (cells[i].isSpawner) {
			spawnerIndex = i;
			break;
		}
	}

	if (spawnerIndex == -1) {
		//std::cerr << "Error: No spawner cell found." << std::endl;
		return path;
	}

	int currentIndex = spawnerIndex;
	while (true) {
		Cell& currentCell = cells[currentIndex];

		currentCell.isOnPath = true;
		path.push_back(currentCell);

		if (currentCell.isTarget) {
			break;
		}

		// Move to the next cell in the flow direction
		int nextX = currentCell.x + currentCell.flowDirectionX;
		int nextY = currentCell.y + currentCell.flowDirectionY;
		int nextIndex = nextX + nextY * cellCountX;

		if (!isInbounds(nextX, nextY) || cells[nextIndex].flowDistance == flowDistanceMax) {
			//std::cerr << "Error: Path is broken or leads out of bounds." << std::endl;
			break;
		}

		currentIndex = nextIndex;
	}

	return path;
}

void Map::subscribe(FlowFieldObserver* observer) {
	observers.push_back(observer);
}

void Map::unsubscribe(FlowFieldObserver* observer) {
	auto it = std::find(observers.begin(), observers.end(), observer);
	if (it != observers.end()) {
		observers.erase(it);
	}
}

void Map::notifyObservers() {
	for (auto observer : observers) {
		observer->onFlowFieldChanged();
	}
}

void Map::setName(std::string newName) {
	name = newName;
}

std::string Map::getName() {
	return name;
}

void Map::setPath(std::string newPath) {
	filePath = newPath;
}

std::string Map::getPath() {
	return filePath;
}