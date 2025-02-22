/**
 * @file LTimer.h
 * @brief The header file for the LTimer class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

class LTimer {
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint64 getTicksNS();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint64 mStartTicks;

	//The ticks stored when the timer was paused
	Uint64 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};