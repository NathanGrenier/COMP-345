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

class Critter : public Observable, public FlowFieldObserver {
public:
	Critter(int level, float speed, int hitPoints, int strength, int reward, SDL_FRect start, Map* map);
	~Critter();

	static constexpr float CRITTER_WIDTH_SCALE = 0.65f;
	static constexpr float CRITTER_HEIGHT_SCALE = 0.65f;

	static constexpr float CRITTER_HEALTHBAR_HEIGHT = CRITTER_HEIGHT_SCALE * 0.4f;
	static constexpr float CRITTER_HEALTHBAR_PADDING = CRITTER_HEIGHT_SCALE * 0.65f;

	void onFlowFieldChanged() override;

	void move(float deltaTime, const std::vector<Critter>& critters, float spacing);  // Move critter towards exit
	void takeDamage(int damage); // Apply damage from towers
	bool isAlive() const;        // Check if critter is still alive
	void stealGold(int& playerGold);  // Steal gold if critter reaches the exit

	void render(SDL_Renderer* renderer);  // Render critter (could be a simple shape or sprite)

	int getSpeed();
	void setSpeed(int speed);

	int getReward() const { return reward; }
	int getStrength() const { return strength; }

	int getHitPoints() const { return hitPoints; }
	void setHitPoints(int hitPoints);

	bool isClicked() const;

	SDL_FRect getPosition() const;

	bool atExit();
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

	int targetCellX;
	int targetCellY;
	SDL_FPoint targetPos; // Pixel position of the target (center of the target cell)
};
