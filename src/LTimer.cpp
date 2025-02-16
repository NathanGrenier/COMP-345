#include <SDL3/SDL_timer.h>
#include <LTimer.h>

//LTimer Implementation
LTimer::LTimer() :
	mStartTicks{ 0 },
	mPausedTicks{ 0 },

	mPaused{ false },
	mStarted{ false } {
}

void LTimer::start() {
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicksNS();
	mPausedTicks = 0;
}

void LTimer::stop() {
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause() {
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicksNS() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause() {
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicksNS() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint64 LTimer::getTicksNS() {
	//The actual timer time
	Uint64 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		} else
		{
			//Return the current time minus the start time
			time = SDL_GetTicksNS() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted() {
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused() {
	//Timer is running and paused
	return mPaused && mStarted;
}