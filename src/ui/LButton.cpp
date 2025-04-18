#include <ui/LButton.h>
#include <Global.h>
#include <util/AudioManager.h>

/**
 * @class LButton
 * @brief Represents an interactive button in the UI.
 *
 * This class handles rendering a button, detecting mouse interactions,
 * changing button states, and displaying text.
 *
 * @author Nirav Patel
 */
LButton::LButton() :
	mPosition{ 0.f, 0.f },
	originalHeight{ 0.f },
	originalWidth{ 0.f },
	mCurrentSprite{ eButtonSpriteMouseOut },
	kButtonHeight{ 0 },
	kButtonWidth{ 0 } {}

LButton::~LButton() {
	mButtonPress = nullptr;
	mButtonHover = nullptr;
}

/**
 * @brief Sets the button's position.
 *
 * @param x The x-coordinate of the button.
 * @param y The y-coordinate of the button.
 */
void LButton::setPosition(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

/**
 * @brief Gets the current position of the button.
 *
 * @return The SDL_FPoint representing the button's position.
 */
SDL_FPoint LButton::getPosition() const {
	return mPosition;
}

/**
 * @brief Handles mouse events for the button, changing its state accordingly.
 *
 * @param e Pointer to an SDL_Event containing mouse event data.
 */
void LButton::handleEvent(SDL_Event* e) {
	// Define the duration for how long the sound should play (in milliseconds)
	const Uint32 soundDuration = 2000; // 2 seconds (change as needed)

	// If mouse event happened
	if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		// Get mouse position
		float x = -1.f, y = -1.f;
		SDL_GetMouseState(&x, &y);

		SDL_FRect spriteClips[] = {
			{0.f, static_cast<float>(0 * kButtonHeight), static_cast<float>(kButtonWidth), static_cast<float>(kButtonHeight)},
			{0.f, static_cast<float>(1 * kButtonHeight), static_cast<float>(kButtonWidth), static_cast<float>(kButtonHeight)},
			{0.f, static_cast<float>(2 * kButtonHeight), static_cast<float>(kButtonWidth), static_cast<float>(kButtonHeight)},
			{0.f, static_cast<float>(3 * kButtonHeight), static_cast<float>(kButtonWidth), static_cast<float>(kButtonHeight)},
		};

		// Calculate the clip's width and height based on the current sprite
		SDL_FRect currentClip = spriteClips[mCurrentSprite];

		// Check if mouse is inside the button's clipped area
		bool inside = (x >= mPosition.x && x <= mPosition.x + currentClip.w &&
					   y >= mPosition.y && y <= mPosition.y + currentClip.h);

		// Update button state based on mouse position
		if (!inside)
		{
			mCurrentSprite = eButtonSpriteMouseOut;
			mButtonSpriteTexture.setAlpha(255);
			currentSound = nullptr;
		} else {
			switch (e->type) {
				case SDL_EVENT_MOUSE_MOTION:
					if (mCurrentSprite != eButtonSpriteMouseOverMotion && currentSound == nullptr) {
						currentSound = mButtonHover;
					}
					mCurrentSprite = eButtonSpriteMouseOverMotion;
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					currentSound = mButtonPress;
					mCurrentSprite = eButtonSpriteMouseDown;
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					mCurrentSprite = eButtonSpriteMouseUp;
					break;
			}
		}
	}
}

/**
 * @brief Sets the text on the button.
 *
 * @param text The text to be displayed.
 * @param textColor The color of the text.
 * @return True if the text texture was successfully created, false otherwise.
 */
bool LButton::setText(const std::string& text, SDL_Color textColor) {
	bool result = mButtonSpriteTexture.loadFromRenderedText(text, textColor);
	if (result) {
		kButtonWidth = mButtonSpriteTexture.getWidth();
		kButtonHeight = mButtonSpriteTexture.getHeight();
		originalWidth = kButtonWidth;
		originalHeight = kButtonHeight;
	}
	return result;
}

bool LButton::loadFromFile(std::string path, std::string buttonPressPath, std::string buttonHoverPath, bool noSound) {
	bool result = mButtonSpriteTexture.loadFromFile(path);
	if (result) {
		kButtonWidth = mButtonSpriteTexture.getWidth();
		kButtonHeight = mButtonSpriteTexture.getHeight() / static_cast<float>(eButtonSpriteCount);
		originalWidth = kButtonWidth;
		originalHeight = kButtonHeight;
	}

	if (!noSound) {
		AudioManager& audioManager = AudioManager::getInstance();
		mButtonPress = audioManager.loadAudio(buttonPressPath.empty() ? "sfx/DefaultButtonPress.wav" : buttonPressPath);
		mButtonHover = audioManager.loadAudio(buttonHoverPath.empty() ? "sfx/ButtonHover.wav" : buttonHoverPath);
	} else {
		mButtonPress = nullptr;
		mButtonHover = nullptr;
	}

	return result;
}

void LButton::update() {
	if (currentSound != previousSound) {
		if (currentSound != nullptr) {
			Mix_PlayChannel(AudioManager::eEffectChannelUI, currentSound, 0);
		}
		previousSound = currentSound;
	}

	if (!Mix_Playing(AudioManager::eEffectChannelUI)) {
		currentSound = nullptr;
	}
}

/**
 * @brief Renders the button on the screen.
 */
void LButton::render() {
	SDL_FRect spriteClips[] = {
		{0.f, static_cast<float>(0 * originalHeight), static_cast<float>(originalWidth), static_cast<float>(originalHeight)},
		{0.f, static_cast<float>(1 * originalHeight), static_cast<float>(originalWidth), static_cast<float>(originalHeight)},
		{0.f, static_cast<float>(2 * originalHeight), static_cast<float>(originalWidth), static_cast<float>(originalHeight)},
		{0.f, static_cast<float>(3 * originalHeight), static_cast<float>(originalWidth), static_cast<float>(originalHeight)},
	};

	// Render the button using the current sprite state
	mButtonSpriteTexture.render(mPosition.x, mPosition.y, &spriteClips[mCurrentSprite], kButtonWidth, kButtonHeight);
}

/**
 * @brief Checks if the button has been clicked.
 *
 * @return True if the button is clicked, false otherwise.
 */
bool LButton::isClicked() const {
	// Get the current mouse position
	float x, y;
	SDL_GetMouseState(&x, &y);

	// Check if the mouse position is inside the button's area
	bool result = x >= mPosition.x && x <= mPosition.x + kButtonWidth &&
		y >= mPosition.y && y <= mPosition.y + kButtonHeight;

	return result;
}

void LButton::setSizeWithAspectRatio(float newWidth, float newHeight) {
	if (kButtonWidth == 0 || kButtonHeight == 0)
	{
		return; // Avoid division by zero
	}

	float aspectRatio = static_cast<float>(kButtonWidth) / kButtonHeight;

	// Adjust width and height while maintaining aspect ratio
	float adjustedWidth = newWidth;
	float adjustedHeight = newHeight;

	if (newWidth == 0)
	{
		adjustedWidth = newHeight * aspectRatio;
	} else if (newHeight == 0) {
		adjustedHeight = newWidth / aspectRatio;
	} else {
		// Maintain aspect ratio based on the closest fit
		float widthRatio = newWidth / kButtonWidth;
		float heightRatio = newHeight / kButtonHeight;

		if (widthRatio < heightRatio)
		{
			adjustedHeight = kButtonHeight * widthRatio;
		} else {
			adjustedWidth = kButtonWidth * heightRatio;
		}
	}

	// Calculate position adjustment
	float widthDiff = adjustedWidth - kButtonWidth;
	float heightDiff = adjustedHeight - kButtonHeight;

	// Adjust position so the button stays centered
	mPosition.x -= (widthDiff / 2.0f);
	mPosition.y -= (heightDiff / 2.0f);

	// Update button dimensions
	kButtonWidth = adjustedWidth;
	kButtonHeight = adjustedHeight;
}