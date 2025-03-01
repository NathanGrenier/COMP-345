#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

#include "States/TitleState.cpp"
#include "States/IntroState.cpp"
#include "States/ExitState.cpp"
#include "ui/LTexture.cpp"
#include <LTimer.h>
#include <iostream>

/** @file main.cpp
 *  @brief Entry point for the Tower Defense game.
 *
 *  This file initializes SDL, manages game states, handles events, and controls rendering.
 *  The game operates at a fixed frame rate of 60 FPS.
 *
 *  @author Nirav Patel
 */

 /** @brief The target frames per second. */
constexpr int kScreenFps{ 60 };

/** @brief The main application window. */
SDL_Window* gWindow{ nullptr };

/** @brief The renderer for drawing to the window. */
SDL_Renderer* gRenderer = nullptr;

/** @brief The global font for rendering text. */
TTF_Font* gFont = nullptr;

/** @brief Texture for rendering FPS information. */
LTexture gFpsTexture;

/** @brief The current game state. */
GameState* gCurrentState{ nullptr };

/** @brief The next game state to transition to. */
GameState* gNextState{ nullptr };


int Global::viewerWidth = 100;
int Global::headerHeight = 20;

std::string currentMap;


/**
 * @brief Sets the next game state.
 *
 * If the requested state is not the ExitState, it will be set as the next state.
 *
 * @param newState The new state to transition to.
 */
void setNextState(GameState* newState) {
	if (gNextState != ExitState::get()) {
		gNextState = newState;
	}
}

/**
 * @brief Changes the game state if a transition is needed.
 *
 * Exits the current state, enters the new state, and updates the game state pointer.
 */
void changeState() {
	if (gNextState != nullptr) {
		gCurrentState->exit();
		gNextState->enter();
		gCurrentState = gNextState;
		gNextState = nullptr;
	}
}

/**
 * @brief Initializes SDL, creates a window and renderer, and sets up SDL_ttf.
 *
 * @return True if initialization was successful, false otherwise.
 */
bool init() {
	bool success{ true };

	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Create window and renderer
		if (!SDL_CreateWindowAndRenderer("Tower Defense - NullTerminators", Global::kScreenWidth, Global::kScreenHeight, 0, &gWindow, &gRenderer)) {
			SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Enable VSync
			if (!SDL_SetRenderVSync(gRenderer, 1)) {
				SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			// Set blending mode
			if (!SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND)) {
				SDL_Log("Could not set blend mode! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			// Initialize SDL_ttf
			if (!TTF_Init()) {
				SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}

	return success;
}

/**
 * @brief Loads media resources such as fonts and textures.
 *
 * @return True if all media loaded successfully, false otherwise.
 */
bool loadMedia() {
	bool success{ true };

	// Load font
	std::string fontPath = "./assets/fonts/lazy.ttf";
	if (gFont = TTF_OpenFont(fontPath.c_str(), 28); gFont == nullptr) {
		SDL_Log("Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
		success = false;
	}
	else {
		// Load text
		SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF };
		if (!gFpsTexture.loadFromRenderedText("Enter to start/stop or space to pause/unpause", textColor)) {
			SDL_Log("Could not load text texture %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
			success = false;
		}
	}

	return success;
}

/**
 * @brief Cleans up SDL resources and quits SDL subsystems.
 */
void close() {
	gFpsTexture.destroy();

	// Free font
	TTF_CloseFont(gFont);
	gFont = nullptr;

	// Destroy window and renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	// Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

/**
 * @brief The main function of the game.
 *
 * Initializes SDL, manages the game loop, processes events, updates and renders states,
 * and enforces a 60 FPS cap.
 *
 * @param argc Argument count (unused).
 * @param args Argument values (unused).
 * @return Exit code (0 if successful).
 */
int main(int argc, char* args[]) {
	int exitCode{ 0 };

	// Initialize SDL
	if (!init()) {
		SDL_Log("Unable to initialize program!\n");
		exitCode = 1;
	}
	else {
		// Load media
		if (!loadMedia()) {
			SDL_Log("Unable to load media!\n");
			exitCode = 2;
		}
		else {
			// Quit flag
			bool quit{ false };

			// SDL event storage
			SDL_Event e;

			// FPS timer
			LTimer fpsTimer;

			// Frame rate cap timer
			LTimer capTimer;

			// Frame counter
			Uint64 renderedFrames = 0;
			Uint64 renderingNS = 0;

			// Flag for resetting FPS calculation
			bool resetFps = true;

			// Start game state machine
			gCurrentState = IntroState::get();
			gCurrentState->enter();
			SDL_SetRenderVSync(gRenderer, 1);

			// Main game loop
			while (!quit) {
				// Reset FPS calculations if needed
				if (resetFps) {
					fpsTimer.start();
					renderedFrames = 0;
					renderingNS = 0;
					resetFps = false;
				}

				// Start frame timer
				capTimer.start();

				// Process events
				while (SDL_PollEvent(&e)) {
					// Handle events in the current state
					gCurrentState->handleEvent(e);

					// Handle quit event
					if (e.type == SDL_EVENT_QUIT) {
						setNextState(ExitState::get());
						quit = true;
					}
					else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
						setNextState(TitleState::get());
					}
				}

				// Update the current state
				gCurrentState->update();

				// Change state if needed
				changeState();

				// Clear the screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// Render the current state
				gCurrentState->render();

				// Present the updated frame
				SDL_RenderPresent(gRenderer);

				// Cap frame rate to maintain 60 FPS
				constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps;
				Uint64 frameNs = capTimer.getTicksNS();
				if (frameNs < nsPerFrame) {
					SDL_DelayNS(nsPerFrame - frameNs);
				}
			}
		}
	}

	// Cleanup and exit
	close();
	return exitCode;
}
