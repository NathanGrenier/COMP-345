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
#include <vector>
#include <iostream>
#include <string>

class FlowFieldObserver {
public:
	virtual void onFlowFieldChanged() = 0;
};

class Map {
public:
	Map(int cellCountX, int cellCountY, std::string name);
	Map();

	std::string name;               /**< Name of the map */
	std::string filePath;               /**< Name of the map */

	/** @brief Number of pixels each cell occupies on screen */
	int PIXELS_PER_CELL;
	int cellCountX, cellCountY;     /**< Grid dimensions */
	bool isFlowFieldVisible = false;

	bool isCellWall(int x, int y);
	void setCellWall(int x, int y, bool setWall);
	bool isTarget(int x, int y);
	void setTarget(int x, int y);
	bool isSpawner(int x, int y);
	void setSpawner(int x, int y);
	bool isValidPath();
	SDL_FRect getTargetPos(const SDL_FRect& targetRect);
	SDL_FRect getSpawnerPos(const SDL_FRect& targetRect);
	Vector2D getFlowNormal(int x, int y);
	void setFlowFieldVisibility(bool value);
	void toggleFlowFieldVisibility();

	bool loadFromJson(const std::string& filePath);
	bool saveToJson(const std::string& filePath);
	void calculatePixelsPerCell();
	void drawOnTargetRect(SDL_Renderer* renderer, const SDL_FRect& targetRect);
	SDL_FRect getCurrentRenderRect();
	void setCurrentRenderRect(SDL_FRect newTargetRect);

	SDL_FRect getPixelPerCell();
	void updateMapDimensions(int newCellCountX, int newCellCountY);

	std::pair<int, int> getCellFromPosition(float x, float y, const SDL_FRect& targetRect) const;
	SDL_FPoint getCellCenter(int x, int y, const SDL_FRect& targetRect) const;

	void subscribe(FlowFieldObserver* observer);
	void unsubscribe(FlowFieldObserver* observer);
	void notifyObservers();

	void setName(std::string newName);
	std::string getName();
	void setPath(std::string newPath);
	std::string getPath();

	/**
	 * @struct Cell
	 * @brief Represents a single cell in the map grid
	 *
	 * Contains position, type, and flow field information for pathfinding
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
		unsigned char flowDistance = flowDistanceMax; /**< Distance to target in flow field */

		bool operator==(const Cell& other) const {
			return x == other.x && y == other.y;
		}
	};
private:
	static const unsigned char flowDistanceMax = 255;
	SDL_FRect currentRenderRect;

	bool isInbounds(int x, int y);
	void drawCell(SDL_Renderer* renderer, const Cell& cell, const SDL_FRect& rect);
	void calculateFlowField();
	void calculateDistances();
	void calculateFlowDirections();
	std::vector<Cell> walkPath();

	std::vector<FlowFieldObserver*> observers; /**< List of subscribers */

	SDL_Texture* textureCellWall;         /**< Texture for wall cells */
	SDL_Texture* textureCellTarget;       /**< Texture for target cells */
	SDL_Texture* textureCellSpawner;      /**< Texture for spawner cells */
	SDL_Texture* textureCellEmpty;        /**< Texture for empty cells */
	SDL_Texture* textureCellArrowUp;      /**< Texture for upward flow */
	SDL_Texture* textureCellArrowRight;   /**< Texture for rightward flow */
	SDL_Texture* textureCellArrowDown;    /**< Texture for downward flow */
	SDL_Texture* textureCellArrowLeft;    /**< Texture for leftward flow */

public:
	std::vector<Cell> cells;              /**< Grid cells storage */

	SDL_FRect scaleCellRect(const Cell& cell, const SDL_FRect& targetRect) const;
};

/**
 * @namespace std
 * @brief Specialization of the std namespace for hashing Map::Cell.
 *
 * Provides a custom hash function for the Map::Cell struct to be used in hash-based containers.
 */
namespace std {
	/**
	 * @struct hash<Map::Cell>
	 * @brief Custom hash function for Map::Cell.
	 *
	 * Specializes the std::hash template for Map::Cell to enable its use in unordered containers.
	 */
	template <>
	struct hash<Map::Cell> {
		/**
		 * @brief Computes the hash value for a Map::Cell.
		 *
		 * @param c The Map::Cell to hash.
		 * @return size_t The computed hash value.
		 * @details Combines the hashes of the x and y coordinates of the cell.
		 */
		size_t operator()(const Map::Cell& c) const {
			// Combine the hashes of x and y (a simple example)
			return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
		}
	};
}