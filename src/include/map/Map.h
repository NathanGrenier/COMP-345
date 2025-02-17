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

class Map {
private:
	static const unsigned char flowDistanceMax = 255;

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
		int flowDirectionX = 0; /**< X component of flow direction */
		int flowDirectionY = 0; /**< Y component of flow direction */
		unsigned char flowDistance = flowDistanceMax; /**< Distance to target in flow field */
	};

public:
	/** @brief Number of pixels each cell occupies on screen */
	static constexpr int PIXELS_PER_CELL = 48;

public:
	Map(SDL_Renderer* renderer, int cellCountX, int cellCountY);
	void draw(SDL_Renderer* renderer);

	bool isCellWall(int x, int y);
	void setCellWall(int x, int y, bool setWall);
	bool isTarget(int x, int y);
	void setTarget(int x, int y);
	bool isSpawner(int x, int y);
	void setSpawner(int x, int y);
	bool isValidPath();
	Vector2D getTargetPos();
	Vector2D getFlowNormal(int x, int y);

private:
	bool isInbounds(int x, int y);
	void drawCell(SDL_Renderer* renderer, const Cell& cell);
	void calculateFlowField();
	void calculateDistances();
	void calculateFlowDirections();

	std::vector<Cell> cells;              /**< Grid cells storage */
	const int cellCountX, cellCountY;     /**< Grid dimensions */

	SDL_Texture* textureCellWall;         /**< Texture for wall cells */
	SDL_Texture* textureCellTarget;       /**< Texture for target cells */
	SDL_Texture* textureCellSpawner;      /**< Texture for spawner cells */
	SDL_Texture* textureCellEmpty;        /**< Texture for empty cells */
	SDL_Texture* textureCellArrowUp;      /**< Texture for upward flow */
	SDL_Texture* textureCellArrowRight;   /**< Texture for rightward flow */
	SDL_Texture* textureCellArrowDown;    /**< Texture for downward flow */
	SDL_Texture* textureCellArrowLeft;    /**< Texture for leftward flow */
};