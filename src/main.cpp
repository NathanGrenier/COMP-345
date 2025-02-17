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

/* Constants */
constexpr int kScreenFps{ 60 };

/* Global Variables */
//The window we'll be rendering to
SDL_Window* gWindow{ nullptr };

//The renderer used to draw to the window
SDL_Renderer* gRenderer = nullptr;

//Global font
TTF_Font* gFont = nullptr;

//The frames per second texture
LTexture gFpsTexture;

//Game state object
GameState* gCurrentState{ nullptr };
GameState* gNextState{ nullptr };

/* Class Implementations */
void setNextState(GameState* newState) {
	//If the user doesn't want to exit
	if (gNextState != ExitState::get())
	{
		//Set the next state
		gNextState = newState;
	}
}

void changeState() {
	//If the state needs to be changed
	if (gNextState != nullptr)
	{
		gCurrentState->exit();
		gNextState->enter();

		//Change the current state ID
		gCurrentState = gNextState;
		gNextState = nullptr;
	}
}

/* Function Implementations */
bool init() {
	//Initialization flag
	bool success{ true };

	//Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		success = false;
	} else
	{
		//Create window with renderer
		if (!SDL_CreateWindowAndRenderer("Tower Defense - NullTerminators", Global::kScreenWidth, Global::kScreenHeight, 0, &gWindow, &gRenderer))
		{
			SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		} else
		{
			//Enable VSync
			if (!SDL_SetRenderVSync(gRenderer, 1))
			{
				SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			// Set the correct blend mode
			if (!SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND))
			{
				SDL_Log("Could not set blend mode! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			//Initialize font loading
			if (!TTF_Init())
			{
				SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}

	return success;
}

bool loadMedia() {
	//File loading flag
	bool success{ true };

	//Load scene font
	std::string fontPath = "./assets/fonts/lazy.ttf";
	if (gFont = TTF_OpenFont(fontPath.c_str(), 28); gFont == nullptr)
	{
		SDL_Log("Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
		success = false;
	} else
	{
		//Load text
		SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF };
		if (!gFpsTexture.loadFromRenderedText("Enter to start/stop or space to pause/unpause", textColor))
		{
			SDL_Log("Could not load text texture %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
			success = false;
		}
	}

	return success;
}

void close() {
	//Clean up textures
	gFpsTexture.destroy();

	//Free font
	TTF_CloseFont(gFont);
	gFont = nullptr;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {
	//Final exit code
	int exitCode{ 0 };
	
	//Initialize
	if (!init())
	{
		SDL_Log("Unable to initialize program!\n");
		exitCode = 1;
	} else
	{
		//Load media
		if (!loadMedia())
		{
			SDL_Log("Unable to load media!\n");
			exitCode = 2;
		} else
		{
			//The quit flag
			bool quit{ false };

			//The event data
			SDL_Event e;

			//Timer to calculate FPS
			LTimer fpsTimer;

			//Timer to cap frame rate
			LTimer capTimer;

			//Frame counter
			Uint64 renderedFrames = 0;

			//Time spend rendering
			Uint64 renderingNS = 0;

			//Reset FPS calculation flag
			bool resetFps = true;

			//Set the current game state object and start state machine
			gCurrentState = IntroState::get();
			gCurrentState->enter();
			SDL_SetRenderVSync(gRenderer, 1);

			//The main loop
			while (quit == false)
			{
				//If the FPS calculation must be reset
				if (resetFps)
				{
					//Reset FPS variables
					fpsTimer.start();
					renderedFrames = 0;
					renderingNS = 0;
					resetFps = false;
				}

				//Start frame time
				capTimer.start();

				//Get event data
				while (SDL_PollEvent(&e))
				{
					//Handle state events
					gCurrentState->handleEvent(e);

					//If event is quit type
					if (e.type == SDL_EVENT_QUIT)
					{
						setNextState(ExitState::get());
						quit = true;
					}
					else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
					{
						setNextState(TitleState::get());
					}
				}

				//Do state logic
				gCurrentState->update();

				//Change state if needed
				changeState();

				//Fill the background
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Do state rendering
				gCurrentState->render();

				//Update screen
				SDL_RenderPresent(gRenderer);

				//Cap frame rate
				constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps;
				Uint64 frameNs = capTimer.getTicksNS();
				if (frameNs < nsPerFrame)
				{
					SDL_DelayNS(nsPerFrame - frameNs);
				}
			}
		}
	}

	//Clean up
	close();

	return exitCode;
}