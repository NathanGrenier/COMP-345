#pragma once

#include <critter/Critter.h>

class NormalCritter : public Critter {
public:
	NormalCritter(int level, SDL_FRect start, Map* map);
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
	float speed = 50.0f;
	float maxHitPoints = 20.0f;
	int strength = 2;
	int reward = 10;
	std::string texturePath = "assets/critters/slime/D_Walk.png";
};