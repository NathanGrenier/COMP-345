/** @class GameOverState
 *  @brief Implementation of the GameOverState.
 *
 *
 *  @author Denmar Ermitano
 */

#include <states/GameOverState.h>
#include <states/EndScreenState.h>
#include <critter/FastCritter.h>
#include <critter/TankCritter.h>
#include <critter/NormalCritter.h>
#include <Global.h>

 /// Static instance of the GameOverState
GameOverState GameOverState::sGameOverState;


const std::string GameOverState::FAST_CRITTER_TYPE = "Bee";
const std::string GameOverState::TANK_CRITTER_TYPE = "Ogre";
const std::string GameOverState::NORMAL_CRITTER_TYPE = "Slime";

/**
 * @brief Gets the singleton instance of GameOverState.
 *
 * @return Pointer to the GameOverState instance.
 */
GameOverState* GameOverState::get() {
	return &sGameOverState;
}

/**
 * @brief Enters the GameOverState, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool GameOverState::enter() {
	bool success = true;

	// generate critter sprite
	generateCritterSprite();

	bool endScreenSuccess = EndScreenState::enter();

	return success && endScreenSuccess;
}

/**
 * @brief Exits the title state and frees resources.
 *
 * Cleans up textures before transitioning to another state.
 *
 * @return Always returns true.
 */
bool GameOverState::exit()
{
	EndScreenState::exit();

	return true;
}

/**
 * @brief Updates the GameOverState logic.
 *
 */
void GameOverState::update() {
	// animate critter
	animateCritterSprite(0.016f);
	
	EndScreenState::update();
}

/**
 * @brief Renders the GameOverState screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void GameOverState::render() {
	// render critter
	SDL_FRect* clip = &(animationFramesWalkDown)[currentFrame];

	EndScreenState::render();
	
	float critterAnimationHeight = (1.0f / 13.0f) * Global::kScreenHeight;
	
	textureWalkDown.render((Global::kScreenWidth - CRITTER_ANIMATION_WIDTH) / 2.0f, critterAnimationHeight, clip, CRITTER_ANIMATION_WIDTH, CRITTER_ANIMATION_WIDTH);
}

/**
 * @brief Loads the sprite of the critter to represent the critter that killed the player.
 */
void GameOverState::generateCritterSprite()
{
	std::string subpath = killedBy;
	subpath[0] = char(tolower(subpath[0]));
	textureWalkDown.loadFromFile("critters/" + subpath + "/D_Walk.png");

	const int animFrames = 6;

	float textureWidth = textureWalkDown.getWidth();
	float textureHeight = textureWalkDown.getHeight();
	float frameWidth = textureWidth / animFrames;
	animationFramesWalkDown.resize(animFrames);

	for (int i = 0; i < animFrames; ++i) {
		animationFramesWalkDown[i] = { i * frameWidth, 0, frameWidth, textureHeight };
	}
}

/**
 * @brief Animes spprite of the critter that killed the player
 * 
 * @param deltaTime change in time
 */
void GameOverState::animateCritterSprite(float deltaTime)
{
	float frameTime = 0.1f;

	animationTimer += deltaTime;
	if (animationTimer >= frameTime)
	{
		currentFrame = (currentFrame + 1) % animationFramesWalkDown.size();

		animationTimer = 0.0f;
	}
}
