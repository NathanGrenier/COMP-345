#include <states/IntroState.h>
#include <SDL3/SDL_log.h>
#include <states/TitleState.h>
#include <Global.h>
#include <cmath>

IntroState IntroState::sIntroState;
float mOscillationTime = 0.0f;

//InrtoState Implementation
IntroState* IntroState::get() {
	//Get static instance
	return &sIntroState;
}

bool IntroState::enter() {
	//Loading success flag
	bool success = true;

	//Load text
	SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
	if (success &= mMessageTexture.loadFromRenderedText("The NullTerminators Present...", textColor); !success)
	{
		SDL_Log("Failed to render intro text!\n");
		success = false;
	}

	return success;
}

bool IntroState::exit() {
	//Free background and text
	mBackgroundTexture.destroy();
	mMessageTexture.destroy();

	return true;
}

void IntroState::handleEvent(SDL_Event& e) {
	//If the user pressed enter
	if ((e.type == SDL_EVENT_MOUSE_BUTTON_DOWN))
	{
		//Move onto title state
		setNextState(TitleState::get());
	}
}

void IntroState::update() {

}

void IntroState::render() {
	// Update oscillation time
	mOscillationTime += 0.05f; // Adjust speed of oscillation

	// Calculate the vertical oscillation offset (range of -5 to 5 pixels)
	float oscillationOffset = std::sin(mOscillationTime) * 5.0f;

	// Show the background
	mBackgroundTexture.render(0, 0);

	// Show the message with the oscillation effect
	mMessageTexture.render(
		(Global::kScreenWidth - mMessageTexture.getWidth()) / 2.f,
		(Global::kScreenHeight - mMessageTexture.getHeight()) / 2.f + oscillationOffset
	);
}


IntroState::IntroState() {}
