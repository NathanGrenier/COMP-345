#include <critter/TankCritter.h>
#include <Global.h>

TankCritter::TankCritter(int level, SDL_FRect start, Map* map)
	: Critter(level, start, map) {
	speed += level * SPEED_SCALE_AMOUNT;
	maxHitPoints += level * HITPOINT_SCALE_AMOUNT;
	hitPoints = maxHitPoints;
	reward += static_cast<int>(level * REWARD_SCALE_AMOUNT);

	setupAnimationFrames();
	frameTime = (1.0f / animationFramesWalkSide.size()) * (1.1f * (1 + Critter::SPEED_SCALE_FACTOR));
	//std::cout << std::fixed << "Tank Critter Frame Time: " << frameTime << std::endl;

}

void TankCritter::loadTextures() {
	textureWalkUp.loadFromFile(baseTexturePath + "/U_Walk.png");
	textureWalkDown.loadFromFile(baseTexturePath + "/D_Walk.png");
	textureWalkSide.loadFromFile(baseTexturePath + "/S_Walk.png");

	textureDeathUp.loadFromFile(baseTexturePath + "/U_Death.png");
	textureDeathDown.loadFromFile(baseTexturePath + "/D_Death.png");
	textureDeathSide.loadFromFile(baseTexturePath + "/S_Death.png");
}

void TankCritter::setupAnimationFrames() {
	loadTextures();

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

std::string TankCritter::getType() const { return "Ogre"; }
float TankCritter::getSpeed() const { return speed; }
void TankCritter::setSpeed(float speed) {
	this->speed = speed;
}
float TankCritter::getHitPoints() const { return hitPoints; }
void TankCritter::setHitPoints(float hitPoints) {
	this->hitPoints = hitPoints;
}
float TankCritter::getMaxHitPoints() const { return maxHitPoints; }
int TankCritter::getReward() const { return reward; }
std::string TankCritter::getTexturePath() const { return baseTexturePath; }
bool TankCritter::isDamaged() const {
	return this->isHurt;
}