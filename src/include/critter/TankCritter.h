#pragma once

#include <critter/Critter.h>

class TankCritter : public Critter {
public:
	TankCritter(int level, SDL_FRect start, Map* map);
	std::string getType() const override;
	float getSpeed() const override;
	void setSpeed(float speed) override;
	float getHitPoints() const override;
	void setHitPoints(float hitPoints) override;
	float getMaxHitPoints() const override;
	int getStrength() const override;
	int getReward() const override;
	std::string getTexturePath() const override;
	bool isDamaged() const override;
	void setupAnimationFrames() override;

private:
	float speed = 25.0f;
	float hitPoints = 30.0f;
	float maxHitPoints = 30.0f;
	int strength = 2;
	int reward = 10;
	std::string texturePath = "assets/critters/ogre/D_Walk.png";
};