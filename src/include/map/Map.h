/**
 * @file Map.h
 * @brief Responsible for generating the tower defense map.
 * @author Nathan Grenier
 * @date 2025-02-15
 *
 * @details A class that manages a grid-based map for a tower defense game.
 * The map consists of cells that can be walls, spawners, or targets.
 * It also implements a flow field pathfinding system to guide enemies from
 * spawners to targets.
 */

#pragma once

#include <SDL3/SDL_render.h>
#include <util/Vector2D.h>
#include <ui/Texture.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

 /**
  * @struct Cell
  * @brief Represents a single cell in the map grid
  *
  * Contains position, type, and flow field information for pathfinding.
  */
struct Cell {
	int x = 0;              /**< X coordinate in the grid */
	int y = 0;              /**< Y coordinate in the grid */
	bool isWall = false;    /**< Whether this cell is a wall */
	bool isSpawner = false; /**< Whether this cell is an enemy spawner */
	bool isTarget = false;  /**< Whether this cell is a target */
	bool isOnPath = false;  /**< Whether this cell is on the path */
	int flowDirectionX = 0; /**< X component of flow direction */
	int flowDirectionY = 0; /**< Y component of flow direction */
	unsigned char flowDistance = 255; /**< Distance to target in flow field */

	/**
	 * @brief Equality operator for Cell.
	 *
	 * Compares two cells based on their x and y coordinates.
	 *
	 * @param other The other Cell to compare with.
	 * @return true if the cells are at the same coordinates, false otherwise.
	 */
	bool operator==(const Cell& other) const {
		return x == other.x && y == other.y;
	}
};

/**
 * @namespace std
 * @brief Specialization of the std namespace for hashing Cell.
 *
 * Provides a custom hash function for the Cell struct to be used in hash-based containers.
 */
namespace std {
	template <>
	struct hash<Cell> {
		/**
		 * @brief Custom hash function for Cell.
		 *
		 * Generates a hash value for a given Cell using its x and y coordinates.
		 *
		 * @param c The Cell to hash.
		 * @return A hash value for the Cell.
		 */
		size_t operator()(const Cell& c) const {
			return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
		}
	};
}

/**
 * @class FlowFieldObserver
 * @brief Interface for objects that observe flow field changes.
 *
 * This class is intended to be used by objects that need to react to changes in the flow field.
 */
class FlowFieldObserver {
public:
	/**
	 * @brief Called when the flow field changes.
	 */
	virtual void onFlowFieldChanged() = 0;
};

/**
 * @class Map
 * @brief Responsible for generating the tower defense map.
 *
 * This class manages the grid-based map for the tower defense game. It includes functionalities for setting up and modifying
 * the map grid, calculating flow field information for pathfinding, and rendering the map.
 */
class Map {
public:
	/**
	 * @brief Constructs a Map object with specified grid dimensions and name.
	 *
	 * Initializes a Map with a specific size (cellCountX and cellCountY) and a name.
	 *
	 * @param cellCountX Number of cells in the X direction.
	 * @param cellCountY Number of cells in the Y direction.
	 * @param name The name of the map.
	 */
	Map(int cellCountX, int cellCountY, std::string name);

	/**
	 * @brief Default constructor for the Map.
	 *
	 * Initializes an empty map with no cells.
	 */
	Map();

	std::string name;               /**< Name of the map */
	std::string filePath;           /**< File path of the map */

	int cellCountX, cellCountY;     /**< Grid dimensions */
	bool isFlowFieldVisible = false;

	/**
	 * @brief Checks if the specified cell is a wall.
	 *
	 * Determines if the cell at the given position is a wall.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @return true if the cell is a wall, false otherwise.
	 */
	bool isCellWall(int x, int y);

	/**
	 * @brief Sets the specified cell as a wall or not.
	 *
	 * Sets the cell at the given position to be a wall or removes it as a wall.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @param setWall true to set as a wall, false to remove the wall.
	 */
	void setCellWall(int x, int y, bool setWall);

	/**
	 * @brief Checks if the specified cell is a target.
	 *
	 * Determines if the cell at the given position is a target.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @return true if the cell is a target, false otherwise.
	 */
	bool isTarget(int x, int y);

	/**
	 * @brief Sets the specified cell as a target.
	 *
	 * Sets the cell at the given position to be a target.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 */
	void setTarget(int x, int y);

	/**
	 * @brief Checks if the specified cell is a spawner.
	 *
	 * Determines if the cell at the given position is an enemy spawner.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @return true if the cell is a spawner, false otherwise.
	 */
	bool isSpawner(int x, int y);

	/**
	 * @brief Sets the specified cell as a spawner.
	 *
	 * Sets the cell at the given position to be an enemy spawner.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 */
	void setSpawner(int x, int y);

	/**
	 * @brief Validates if there is a valid path on the map.
	 *
	 * Checks if the map contains a valid path from spawner to target.
	 *
	 * @return true if a valid path exists, false otherwise.
	 */
	bool isValidPath();

	/**
	 * @brief Retrieves the target position based on the target rectangle.
	 *
	 * @param targetRect The target rectangle to calculate the position from.
	 * @return The position of the target.
	 */
	SDL_FRect getTargetPos(const SDL_FRect& targetRect);

	/**
	 * @brief Retrieves the spawner position based on the spawner rectangle.
	 *
	 * @param targetRect The spawner rectangle to calculate the position from.
	 * @return The position of the spawner.
	 */
	SDL_FRect getSpawnerPos(const SDL_FRect& targetRect);

	/**
	 * @brief Retrieves the flow direction vector at a specific cell.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @return A Vector2D representing the flow direction.
	 */
	Vector2D getFlowNormal(int x, int y);

	/**
	 * @brief Sets the visibility of the flow field.
	 *
	 * Toggles the visibility of the flow field.
	 *
	 * @param value true to show the flow field, false to hide it.
	 */
	void setFlowFieldVisibility(bool value);

	/**
	 * @brief Toggles the visibility of the flow field.
	 */
	void toggleFlowFieldVisibility();

	/**
	 * @brief Checks if the map is empty.
	 *
	 * @return true if the map has no cells, false otherwise.
	 */
	bool isEmpty() const { return cellCountX == 0; };

	/**
	 * @brief Loads a map from a JSON file.
	 *
	 * Loads the map configuration and data from a JSON file.
	 *
	 * @param filePath The file path to the JSON map file.
	 * @return true if loading is successful, false otherwise.
	 */
	bool loadFromJson(const std::string& filePath);

	/**
	 * @brief Saves the map to a JSON file.
	 *
	 * Saves the current map state to a JSON file.
	 *
	 * @param filePath The file path where the map will be saved.
	 * @return true if saving is successful, false otherwise.
	 */
	bool saveToJson(const std::string& filePath);

	/**
	 * @brief Loads the cell textures.
	 */
	void loadCellTextures();

	/**
	 * @brief Draws the map onto a specific target rectangle.
	 *
	 * @param targetRect The target rectangle to render the map on.
	 */
	void drawOnTargetRect(const SDL_FRect& targetRect);

	/**
	 * @brief Retrieves the current render rectangle.
	 *
	 * @return The current render rectangle.
	 */
	SDL_FRect getCurrentRenderRect() const;

	/**
	 * @brief Sets a new render rectangle.
	 *
	 * @param newTargetRect The new render rectangle.
	 */
	void setCurrentRenderRect(SDL_FRect newTargetRect);

	/**
	 * @brief Retrieves the pixels per cell value.
	 *
	 * @return The pixels per cell value.
	 */
	float getPixelPerCell() const;

	/**
	 * @brief Updates the map dimensions.
	 *
	 * @param newCellCountX New number of cells in the X direction.
	 * @param newCellCountY New number of cells in the Y direction.
	 */
	void updateMapDimensions(int newCellCountX, int newCellCountY);

	/**
	 * @brief Converts a position in pixels to the corresponding grid cell.
	 *
	 * @param x X position in pixels.
	 * @param y Y position in pixels.
	 * @param targetRect The target rectangle to calculate the cell.
	 * @return A pair of integers representing the grid coordinates.
	 */
	std::pair<int, int> getCellFromPosition(float x, float y, const SDL_FRect& targetRect) const;

	/**
	 * @brief Retrieves the center position of a specific grid cell.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @param targetRect The target rectangle to calculate the center.
	 * @return The center point of the cell in pixels.
	 */
	SDL_FPoint getCellCenter(int x, int y, const SDL_FRect& targetRect);

	/**
	 * @brief Subscribes an observer to receive flow field change notifications.
	 *
	 * @param observer The observer to subscribe.
	 */
	void subscribe(FlowFieldObserver* observer);

	/**
	 * @brief Unsubscribes an observer from receiving flow field change notifications.
	 *
	 * @param observer The observer to unsubscribe.
	 */
	void unsubscribe(FlowFieldObserver* observer);

	/**
	 * @brief Notifies all subscribed observers of a flow field change.
	 */
	void notifyObservers();

	/**
	 * @brief Sets a new name for the map.
	 *
	 * @param newName The new name of the map.
	 */
	void setName(std::string newName);

	/**
	 * @brief Retrieves the current map name.
	 *
	 * @return The current name of the map.
	 */
	std::string getName() const;

	/**
	 * @brief Sets a new file path for the map.
	 *
	 * @param newPath The new file path of the map.
	 */
	void setPath(std::string newPath);

	/**
	 * @brief Retrieves the current map file path.
	 *
	 * @return The current file path of the map.
	 */
	std::string getPath() const;

	std::unordered_map<Cell, bool> wallCellDict; /**< Dictionary for wall cells */
	std::vector<Cell> cells;              /**< Grid cells storage */

	/**
	 * @brief Scales a cell rectangle based on the target rectangle.
	 *
	 * @param cell The cell to scale.
	 * @param targetRect The target rectangle to use for scaling.
	 * @return The scaled rectangle for the cell.
	 */
	SDL_FRect scaleCellRect(const Cell& cell, const SDL_FRect& targetRect) const;

private:
	static const unsigned char flowDistanceMax = 255; /**< Maximum flow distance */
	SDL_FRect currentRenderRect; /**< Current render rectangle */

	/**
	 * @brief Checks if the given coordinates are within bounds.
	 *
	 * @param x X coordinate of the cell.
	 * @param y Y coordinate of the cell.
	 * @return true if the coordinates are within bounds, false otherwise.
	 */
	bool isInbounds(int x, int y) const;

	/**
	 * @brief Draws a cell on the screen.
	 *
	 * @param cell The cell to draw.
	 * @param rect The rectangle to draw the cell in.
	 */
	void drawCell(const Cell& cell, const SDL_FRect& rect);

	/**
	 * @brief Calculates the flow field for the map.
	 */
	void calculateFlowField();

	/**
	 * @brief Calculates the flow field distances for pathfinding.
	 */
	void calculateDistances();

	/**
	 * @brief Calculates the flow directions for pathfinding.
	 */
	void calculateFlowDirections();

	/**
	 * @brief Walks through the path and returns the cells on the path.
	 *
	 * @return A vector of cells representing the path.
	 */
	std::vector<Cell> walkPath();

	std::vector<FlowFieldObserver*> observers; /**< List of subscribers */

	Texture textureCellWall;         /**< Texture for wall cells */
	Texture textureCellTarget;       /**< Texture for target cells */
	Texture textureCellSpawner;      /**< Texture for spawner cells */
	Texture textureCellEmpty;        /**< Texture for empty cells */
	Texture textureCellArrowUp;      /**< Texture for upward flow */
	Texture textureCellArrowRight;   /**< Texture for rightward flow */
	Texture textureCellArrowDown;    /**< Texture for downward flow */
	Texture textureCellArrowLeft;    /**< Texture for leftward flow */
};
