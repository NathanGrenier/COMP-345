#include <SDL3/SDL_timer.h>
#include <LTimer.h>

/**
 * @class LTimer
 * @brief A class that encapsulates a timer with start, stop, pause, and unpause functionality.
 *
 * The LTimer class provides functionality to measure elapsed time in nanoseconds and manage
 * timer states such as running, paused, or stopped. It is useful for managing time intervals
 * and frame rates in games or other time-sensitive applications.
 *
 * @author Nirav Patel
 */
LTimer::LTimer() :
    mStartTicks{ 0 },
    mPausedTicks{ 0 },
    mPaused{ false },
    mStarted{ false } {
}

/**
 * @brief Starts the timer.
 *
 * This method starts the timer from the current time and resets any paused time. Once started,
 * the timer will keep running until it is stopped or paused.
 */
void LTimer::start() {
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicksNS();
    mPausedTicks = 0;
}

/**
 * @brief Stops the timer.
 *
 * This method stops the timer and resets all time tracking variables. The timer cannot be used
 * again until it is started.
 */
void LTimer::stop() {
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

/**
 * @brief Pauses the timer.
 *
 * This method pauses the timer and stores the elapsed time at the moment it was paused.
 * After pausing, the timer can be resumed using the `unpause()` method.
 */
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

/**
 * @brief Unpauses the timer.
 *
 * This method unpauses the timer and resumes counting from where it left off when it was paused.
 */
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

/**
 * @brief Gets the elapsed time in nanoseconds.
 *
 * This method returns the total elapsed time since the timer started, accounting for both the
 * running and paused states. If the timer is paused, it returns the time accumulated at the moment
 * it was paused.
 *
 * @return The elapsed time in nanoseconds.
 */
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
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicksNS() - mStartTicks;
        }
    }

    return time;
}

/**
 * @brief Checks if the timer has been started.
 *
 * This method checks if the timer has been started and is either running or paused.
 *
 * @return True if the timer has been started, false otherwise.
 */
bool LTimer::isStarted() {
    //Timer is running and paused or unpaused
    return mStarted;
}

/**
 * @brief Checks if the timer is currently paused.
 *
 * This method checks if the timer is paused. The timer can only be paused if it was previously started.
 *
 * @return True if the timer is paused, false otherwise.
 */
bool LTimer::isPaused() {
    //Timer is running and paused
    return mPaused && mStarted;
}
