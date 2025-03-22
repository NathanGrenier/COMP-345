/**
 * @file Global.h
 * @brief The header file for the Global class, which manages global settings and state transitions.
 *
 * This file contains constants for screen dimensions, external variables for SDL components,
 * and prototypes for functions related to initialization, media loading, and state management.
 *
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <states/GameState.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map/Map.h>
#include <string>

 // External SDL variables
extern SDL_Renderer* gRenderer; ///< Global pointer to the SDL renderer.
extern TTF_Font* gFont; ///< Global pointer to the font used for rendering text.

/**
 * @class Global
 * @brief The header file for the Global class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
class Global {
public:
    static inline const float kScreenWidth{ 800 };  // Constant, set at compile-time
    static inline const float kScreenHeight{ 600 }; // Constant, set at compile-time

    static float viewerWidth;  // This will now be settable
    static float headerHeight;  // Constant value

    static Map* currentMap;
    static SDL_FRect mapViewRect;
};

/**
 * @brief Initializes SDL and creates the game window and renderer.
 *
 * This function starts up the SDL library, creates a window for rendering, and prepares the necessary
 * resources for the game to run.
 *
 * @return `true` if initialization is successful, `false` otherwise.
 */
bool init();

/**
 * @brief Loads necessary media files such as fonts, textures, and other assets.
 *
 * This function loads all the media assets required by the game, such as fonts and textures, into
 * the program's memory for rendering.
 *
 * @return `true` if media loading is successful, `false` otherwise.
 */
bool loadMedia();

/**
 * @brief Frees media resources and shuts down SDL.
 *
 * This function frees any dynamically allocated media and shuts down SDL systems to cleanly exit
 * the game.
 */
void close();

/**
 * @brief Sets the next game state.
 *
 * This function sets the state that the game will transition to after the current state finishes.
 *
 * @param nextState A pointer to the new game state.
 */
void setNextState(GameState* nextState);

/**
 * @brief Changes the current game state to the next state.
 *
 * This function triggers the state change, ensuring that the transition from the current state
 * to the next state is executed properly.
 */
void changeState();
