/**
 * @file GameState.h
 * @brief The header file for the GameState class.
 *
 * This class serves as an abstract base class for all game states.
 * It defines the interface for handling state transitions, event processing,
 * game updates, and rendering.
 *
 * @author Nirav Patel
 * @date 2025-02-16
 */

#pragma once

#include <SDL3/SDL_events.h> 

 /**
  * @class GameState
  * @brief Abstract base class for all game states.
  *
  * The `GameState` class provides an interface for handling game states
  * such as entering, exiting, event handling, updating logic, and rendering.
  * All derived states must implement these functions.
  */
class GameState {
public:
    /**
     * @brief Distance of the title from the top of the window in pixels.
     */
    float titleDistanceFromTop = 20.0f;

    /**
     * @brief Distance of the back button from the left of the window in pixels.
     */
    float backButtonDistanceFromLeft = 30.0f;
    
    /**
     * @brief Called when entering the state.
     *
     * This function is executed when transitioning into the current state.
     * Any initialization specific to the state should be done here.
     *
     * @return Returns `true` if the state enters successfully, `false` otherwise.
     */
    virtual bool enter() = 0;

    /**
     * @brief Called when exiting the state.
     *
     * This function is executed when transitioning out of the current state.
     * Any cleanup required before switching states should be done here.
     *
     * @return Returns `true` if the state exits successfully, `false` otherwise.
     */
    virtual bool exit() = 0;

    /**
     * @brief Handles input events.
     *
     * This function processes SDL events such as key presses and mouse inputs.
     * Each game state should handle events relevant to its functionality.
     *
     * @param e The SDL_Event object containing event data.
     */
    virtual void handleEvent(SDL_Event& e) = 0;

    /**
     * @brief Updates the game logic.
     *
     * This function is called every frame to update the game state logic,
     * such as movement, AI behavior, and game state transitions.
     */
    virtual void update() = 0;

    /**
     * @brief Renders the game state.
     *
     * This function is responsible for drawing all visual elements of the state.
     * It should be called every frame to refresh the screen.
     */
    virtual void render() = 0;

    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures that derived classes are properly destroyed when a state is deleted.
     */
    virtual ~GameState() = default;
};
