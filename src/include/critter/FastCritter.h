#pragma once

#include <critter/Critter.h>

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
	static inline SDL_Texture* loadedTextureWalkUp = nullptr;
	static inline SDL_Texture* loadedTextureWalkDown = nullptr;
	static inline SDL_Texture* loadedTextureWalkSide = nullptr;
	static inline SDL_Texture* loadedTextureDeathUp = nullptr;
	static inline SDL_Texture* loadedTextureDeathDown = nullptr;
	static inline SDL_Texture* loadedTextureDeathSide = nullptr;

	static void loadTextures();

private:
	float speed = 75.0f;
	float hitPoints;
	float maxHitPoints = 10.0f;
	int reward = 5;
	static inline const std::string baseTexturePath = "critters/bee";
};