#include <critter/NormalCritter.h>

NormalCritter::NormalCritter(int level, SDL_FRect start, Map* map)
	: Critter(level, start, map) {
	hitPoints = 20.0f + level * 2.0f;
	maxHitPoints = hitPoints;
	strength = level * 2;
	reward = level * 10;

	//critterTexture.loadFromFile(getTexturePath());

	setupAnimationFrames();
	frameTime = (1.0f / animationFramesWalkSide.size()) * 0.8;
	//std::cout << std::fixed << "Normal Critter Frame Time: " << frameTime << std::endl;
}

void NormalCritter::setupAnimationFrames() {
	// Load walking animations
	textureWalkUp.loadFromFile("assets/critters/slime/U_Walk.png");
	textureWalkDown.loadFromFile("assets/critters/slime/D_Walk.png");
	textureWalkSide.loadFromFile("assets/critters/slime/S_Walk.png");

	// Load death animations
	textureDeathUp.loadFromFile("assets/critters/slime/U_Death.png");
	textureDeathDown.loadFromFile("assets/critters/slime/D_Death.png");
	textureDeathSide.loadFromFile("assets/critters/slime/S_Death.png");

	const int animFrames = 6;

	// Setup Walk Up frames
	float textureWidth = textureWalkUp.getWidth();
	float textureHeight = textureWalkUp.getHeight();
	float frameWidth = textureWidth / animFrames;
	animationFramesWalkUp.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkUp[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Walk Down frames
	textureWidth = textureWalkDown.getWidth();
	textureHeight = textureWalkDown.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesWalkDown.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkDown[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Walk Side frames
	textureWidth = textureWalkSide.getWidth();
	textureHeight = textureWalkSide.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesWalkSide.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkSide[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Death Up frames
	textureWidth = textureDeathUp.getWidth();
	textureHeight = textureDeathUp.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesDeathUp.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesDeathUp[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Death Down frames
	textureWidth = textureDeathDown.getWidth();
	textureHeight = textureDeathDown.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesDeathDown.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesDeathDown[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Death Side frames
	textureWidth = textureDeathSide.getWidth();
	textureHeight = textureDeathSide.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesDeathSide.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesDeathSide[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}
}

std::string NormalCritter::getType() const { return "Slime"; }
float NormalCritter::getSpeed() const { return speed; }
void NormalCritter::setSpeed(float speed) {
	this->speed = speed;
}
float NormalCritter::getHitPoints() const { return hitPoints; }
void NormalCritter::setHitPoints(float hitPoints) {
	this->hitPoints = hitPoints;
}
float NormalCritter::getMaxHitPoints() const { return maxHitPoints; }
int NormalCritter::getStrength() const { return strength; }
int NormalCritter::getReward() const { return reward; }
std::string NormalCritter::getTexturePath() const { return texturePath; }
bool NormalCritter::isDamaged() const {
	return this->isHurt;
}