#include <critter/FastCritter.h>

FastCritter::FastCritter(int level, SDL_FRect start, Map* map)
	: Critter(level, start, map) {
	speed = 75.0f + level * 5.0f;
	hitPoints = 10.0f + level * 1.0f;
	maxHitPoints = hitPoints;
	strength = level * 2;
	reward = level * 10;

	setupAnimationFrames();
	frameTime = (1.0f / animationFramesWalkSide.size()) * 0.5;
	//std::cout << std::fixed << "Fast Critter Frame Time: " << frameTime << std::endl;

}

void FastCritter::setupAnimationFrames() {
	// Load walk animations
	textureWalkUp.loadFromFile("assets/critters/bee/U_Walk.png");
	textureWalkDown.loadFromFile("assets/critters/bee/D_Walk.png");
	textureWalkSide.loadFromFile("assets/critters/bee/S_Walk.png");

	// Load death animations
	textureDeathUp.loadFromFile("assets/critters/bee/U_Death.png");
	textureDeathDown.loadFromFile("assets/critters/bee/D_Death.png");
	textureDeathSide.loadFromFile("assets/critters/bee/S_Death.png");

	const int animFrames = 6;

	// Setup Up frames
	float textureWidth = textureWalkUp.getWidth();
	float textureHeight = textureWalkUp.getHeight();
	float frameWidth = textureWidth / animFrames;
	animationFramesWalkUp.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkUp[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Down frames
	textureWidth = textureWalkDown.getWidth();
	textureHeight = textureWalkDown.getHeight();
	frameWidth = textureWidth / animFrames;
	animationFramesWalkDown.resize(animFrames);
	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkDown[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}

	// Setup Side frames
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

std::string FastCritter::getType() const { return "Bee"; }
float FastCritter::getSpeed() const { return speed; }
void FastCritter::setSpeed(float speed) {
	this->speed = speed;
}
float FastCritter::getHitPoints() const { return hitPoints; }
void FastCritter::setHitPoints(float hitPoints) {
	this->hitPoints = hitPoints;
}
float FastCritter::getMaxHitPoints() const { return maxHitPoints; }
int FastCritter::getStrength() const { return strength; }
int FastCritter::getReward() const { return reward; }
std::string FastCritter::getTexturePath() const { return texturePath; }
bool FastCritter::isDamaged() const {
	return this->isHurt;
}