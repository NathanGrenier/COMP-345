#include <critter/NormalCritter.h>
#include <util/TextureLoader.h>
#include <Global.h>

NormalCritter::NormalCritter(int level, SDL_FRect start, Map* map)
	: Critter(level, start, map) {
	speed = 50.0f;
	hitPoints = 20.0f + level * 2.0f;
	maxHitPoints = hitPoints;
	strength = level * 2;
	reward = level * 10;

	setupAnimationFrames();
	frameTime = (1.0f / animationFramesWalkSide.size()) * 0.8;
	//std::cout << std::fixed << "Normal Critter Frame Time: " << frameTime << std::endl;
}

void NormalCritter::loadTextures() {
	loadedTextureWalkUp = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/U_Walk.png");
	loadedTextureWalkDown = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/D_Walk.png");
	loadedTextureWalkSide = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/S_Walk.png");

	loadedTextureDeathUp = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/U_Death.png");
	loadedTextureDeathDown = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/D_Death.png");
	loadedTextureDeathSide = TextureLoader::loadTexture(gRenderer, NormalCritter::baseTexturePath + "/S_Death.png");
}

void NormalCritter::setupAnimationFrames() {
	textureWalkUp = LTexture(NormalCritter::loadedTextureWalkUp, false);
	textureWalkDown = LTexture(NormalCritter::loadedTextureWalkDown, false);
	textureWalkSide = LTexture(NormalCritter::loadedTextureWalkSide, false);

	textureDeathUp = LTexture(NormalCritter::loadedTextureDeathUp, false);
	textureDeathDown = LTexture(NormalCritter::loadedTextureDeathDown, false);
	textureDeathSide = LTexture(NormalCritter::loadedTextureDeathSide, false);

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
std::string NormalCritter::getTexturePath() const { return baseTexturePath; }
bool NormalCritter::isDamaged() const {
	return this->isHurt;
}