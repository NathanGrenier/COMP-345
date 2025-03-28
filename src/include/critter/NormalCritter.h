#pragma once

#include <critter/Critter.h>
#include <ui/Texture.h>

class NormalCritter : public Critter {
public:
	NormalCritter(int level, SDL_FRect start, Map* map);
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
private:
	void loadTextures();

	float speed = 50.0f;
	float maxHitPoints = 20.0f;
	int reward = 10;
	static inline const std::string baseTexturePath = "critters/slime";
};