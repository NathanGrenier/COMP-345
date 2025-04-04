/**
 * @file Critter.h
 * @brief The header file for the Critter class.
 * @author Nirav Patel
 * @date 2025-02-21
 *
 * @details This file contains the declaration of the Critter class, which represents
 *          the enemies in the tower defense game. Critters follow a flow field path
 *          towards an exit and can be damaged by towers. They also have health, speed,
 *          strength, and a reward value upon death.
 */

#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <map/Map.h>
#include <util/Observable.h>
#include <ui/Texture.h>
#include <SDL3_mixer/SDL_mixer.h>

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class State {
	ALIVE,
	DYING,
	DEAD
};

/**
 * @class Critter
 * @brief Represents an enemy in the tower defense game.
 *
 * The Critter class models an enemy unit that moves across the map, follows a path to
 * the exit, takes damage from towers, and steals gold when it reaches the exit. The
 * class also handles rendering and updating the critter's position, health, and other
 * properties.
 */
class Critter : public Observable, public FlowFieldObserver {
public:
	Critter(int level, SDL_FRect start, Map* map);
	~Critter();

	// Pure virtual functions for subclass-specific attributes
	virtual std::string getType() const = 0;
	virtual float getSpeed() const = 0;
	virtual void setSpeed(float speed) = 0;
	virtual float getHitPoints() const = 0;
	virtual void setHitPoints(float hitPoints) = 0;
	virtual float getMaxHitPoints() const = 0;
	virtual int getReward() const = 0;
	virtual std::string getTexturePath() const = 0;
	virtual bool isDamaged() const = 0;
	virtual void setupAnimationFrames() = 0;

	// Common logic methods
	void onFlowFieldChanged() override;
	SDL_FRect getPosition() const;
	void move(float deltaTime, const std::vector<Critter*> critters);
	void takeDamage(float damage);
	void update(float deltaTime);
	bool isAlive() const;
	bool isDying() const;
	bool atExit() const;
	bool isClicked() const;
	void setAtExit(bool con);
	void render();
	void stealGold(int& playerGold) const;
	SDL_FRect getCurrentRenderRect() const { return currentRenderRect; }
	float getDistanceTravelled() const { return distanceTravelled; };

	// Constants
	static constexpr float CRITTER_WIDTH_SCALE = 1.0f;  /**< Width scale for the critter */
	static constexpr float CRITTER_HEIGHT_SCALE = 1.0f; /**< Height scale for the critter */
	static constexpr float SPEED_PER_CELL = 30.0f; /**< Amount of speed for the critter to take a cell (as a ratio)*/

	static constexpr float CRITTER_HEALTHBAR_HEIGHT = CRITTER_HEIGHT_SCALE * 0.15f;  /**< Height of the health bar */
	static constexpr float CRITTER_HEALTHBAR_PADDING = CRITTER_HEIGHT_SCALE * 0.15f; /**< Padding for the health bar */

	static constexpr float CRITTER_SPRITE_MODIFIER = 2.0f;

	static Mix_Chunk* critterHit;
	static Mix_Chunk* critterSpawn;
	static Mix_Chunk* critterDeath;

protected:
	int level;
	float hitPoints;
	SDL_FRect position;
	bool isAtExit;
	Map* map;
	int targetCellX;
	int targetCellY;
	SDL_FPoint targetPos;
	Texture critterTexture;
	SDL_FRect currentRenderRect;
	Direction currentDirection;
	float distanceTravelled; /**< The distanced travelled by the critter, away from the spawn point*/

	static constexpr float SPEED_SCALE_FACTOR = 0.01f;
	static constexpr float HITPOINT_SCALE_FACTOR = 0.2f;
	static constexpr float REWARD_SCALE_FACTOR = 0.01f;

	// Health bar damage effect
	bool isHurt = false;
	Uint64 damageTimer = 0;
	const Uint64 damageDuration = 500;
	float redTintAlpha = 255;
	float greenTintAlpha = 255;
	float blueTintAlpha = 255;
	const float maxRedAlpha = 255;

	// Animation textures and frames for each direction
	int currentFrame;
	float animationTimer;
	float frameTime;
	Texture textureWalkUp;
	Texture textureWalkDown;
	Texture textureWalkSide;
	std::vector<SDL_FRect> animationFramesWalkUp;
	std::vector<SDL_FRect> animationFramesWalkDown;
	std::vector<SDL_FRect> animationFramesWalkSide;

	Texture textureDeathUp;
	Texture textureDeathDown;
	Texture textureDeathSide;
	std::vector<SDL_FRect> animationFramesDeathUp, animationFramesDeathDown, animationFramesDeathSide;
	State currentState;
};
