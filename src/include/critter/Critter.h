/**
 * @file Critter.h
 * @brief The header file for the Critter class.
 * @author Nirav Patel
 * @date 2025-02-21
 */
#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <map/Map.h>
#include <util/Observable.h>
#include <ui/LTexture.h>

class Critter : public Observable, public FlowFieldObserver {
public:
	Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, Map* map);
	~Critter();

	static constexpr float CRITTER_WIDTH_SCALE = 1.00f;
	static constexpr float CRITTER_HEIGHT_SCALE = 1.00f;

	static constexpr float CRITTER_HEALTHBAR_HEIGHT = CRITTER_HEIGHT_SCALE * 0.3f;
	static constexpr float CRITTER_HEALTHBAR_PADDING = CRITTER_HEIGHT_SCALE * 0.5f;

	void onFlowFieldChanged() override;

	void move(float deltaTime, const std::vector<Critter*> critters, float spacing);  // Move critter towards exit
	void takeDamage(); // Apply damage from towers
	bool isAlive() const;        // Check if critter is still alive
	void stealGold(int& playerGold) const;  // Steal gold if critter reaches the exit

	void render();  // Render critter (could be a simple shape or sprite)
	void update();

	float getSpeed() const;
	void setSpeed(float speed);

	int getReward() const { return reward; }
	int getStrength() const { return strength; }

	int getHitPoints() const { return hitPoints; }
	void setHitPoints(int hitPoints);

	bool isClicked() const;

	SDL_FRect getPosition() const;
	SDL_FRect getCurrentRenderRect() const { return currentRenderRect; };

	bool atExit() const;
	void setAtExit(bool con);
private:
	int level;
	float speed;
	int hitPoints;
	int strength;
	int reward;
	SDL_FRect position;
	bool isAtExit;
	int maxHitPoints;
	Map* map;
	SDL_FRect currentRenderRect;
	LTexture critterTexture;
	
	bool isDamaged = false;
	Uint64 damageTimer = 0; 
	float redTintAlpha = 0.0f;
	float greenTintAlpha = 0.0f;
	float blueTintAlpha = 0.0f;
	const float maxRedAlpha = 255.0f;
	const Uint32 damageDuration = 100;

	int targetCellX;
	int targetCellY;
	SDL_FPoint targetPos; // Pixel position of the target (center of the target cell)
};
