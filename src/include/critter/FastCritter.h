#pragma once

#include <critter/Critter.h>
#include <ui/Texture.h>

class FastCritter : public Critter {
public:
	FastCritter(int level, SDL_FRect start, Map* map);
	std::string getType() const override;
	float getSpeed() const override;
	void setSpeed(float speed) override;
	float getHitPoints() const override;
	void setHitPoints(float hitPoints) override;
	float getMaxHitPoints() const override;
	int getReward() const override;
	std::string getTexturePath() const override;
	bool isDamaged() const override;
	void setupAnimationFrames() override;

	// Static texture members
	static inline Texture loadedTextureWalkUp;
	static inline Texture loadedTextureWalkDown;
	static inline Texture loadedTextureWalkSide;
	static inline Texture loadedTextureDeathUp;
	static inline Texture loadedTextureDeathDown;
	static inline Texture loadedTextureDeathSide;

	void loadTextures();

private:
	static constexpr float INITIAL_SPEED = 75.0f;
	static constexpr float INITIAL_HITPOINTS = 5.0f;
	static constexpr int INITIAL_REWARD = 7;
	float speed = INITIAL_SPEED;
	float maxHitPoints = INITIAL_HITPOINTS;
	int reward = INITIAL_REWARD;

	static constexpr float SPEED_SCALE_AMOUNT = INITIAL_SPEED * Critter::SPEED_SCALE_FACTOR;
	static constexpr float HITPOINT_SCALE_AMOUNT = INITIAL_HITPOINTS * Critter::HITPOINT_SCALE_FACTOR;
	static constexpr float REWARD_SCALE_AMOUNT = INITIAL_REWARD * Critter::REWARD_SCALE_FACTOR;

	static inline const std::string baseTexturePath = "critters/bee";
};