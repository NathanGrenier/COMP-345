#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <sstream>

#include "States/TitleState.cpp"
#include "States/IntroState.cpp"
#include "States/ExitState.cpp"
#include <LTimer.h>
#include <iostream>
#include <util/TextureManager.h>
#include <util/AudioManager.h>
#include <algorithm>

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
Texture gFpsTexture;

/** @brief The current game state. */
GameState* gCurrentState{ nullptr };

/** @brief The next game state to transition to. */
GameState* gNextState{ nullptr };
Map* Global::currentMap;
ParallaxBackground* bg;

//Playback audio device
SDL_AudioDeviceID gAudioDeviceId{ 0 };

//The music that will be played
Mix_Music* Global::gMusic{ nullptr };

float Global::viewerWidth = kScreenWidth * 0.3f;
float Global::headerHeight = kScreenHeight * 0.15f;

SDL_FRect Global::mapViewRect = {
	Global::kScreenWidth * 0.02f,
	Global::headerHeight + Global::kScreenHeight * 0.02f,
	static_cast<float>(Global::kScreenWidth - Global::viewerWidth) - 2 * Global::kScreenWidth * 0.02f,
	static_cast<float>(Global::kScreenHeight - Global::headerHeight) - 2 * Global::kScreenHeight * 0.02f };

bool isFading = false;	// Controls when fading occurs
float fadeAlpha = 0.0f; // Alpha value (0 = fully visible, 255 = fully black)
bool fadeIn = false; // Determines fade direction
LTimer fadeTimer; // Timer for tracking fade duration
constexpr Uint64 FADE_DURATION = 350'000'000;

std::ofstream outFile;

/**
 * @brief Sets the next game state.
 *
 * If the requested state is not the ExitState, it will be set as the next state.
 *
 * @param newState The new state to transition to.
 */
void setNextState(GameState* newState) {
	if (gNextState != ExitState::get())
	{
		gNextState = newState;
	}
}

/**
 * @brief Changes the game state if a transition is needed.
 *
 * Exits the current state, enters the new state, and updates the game state pointer.
 */
void changeState() {
	if (gNextState != nullptr)
	{
		// Start fading
		isFading = true;
		fadeIn = false; // Start with fade-out effect
		fadeAlpha = 0.0f;
		fadeTimer.start();
	}
}

/**
 * @brief Initializes SDL, creates a window and renderer, and sets up SDL_ttf.
 *
 * @return True if initialization was successful, false otherwise.
 */
bool init() {
	// Get current time and format it as HH:MM:SS
	std::time_t currentTime = std::time(nullptr);  // Get the current time
	std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime);  // Convert to local time using localtime_s


	char timestamp[20];  // Buffer to store formatted time (YYYYMMDD_HHMMSS format)
	std::strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", &timeInfo);  // Format time

	// Construct the filename with the timestamp
	std::stringstream filenameStream;
	filenameStream << "LOG_" << timestamp << ".txt";  // Example: LOG_20250328_123045.txt
	std::string filename = filenameStream.str();  // Convert the stream to a string

	// Create and open the text file for writing (Append mode)
	outFile.open(filename, std::ios::app);

	if (!outFile) {  // Check if the file is opened successfully
		std::cerr << "Error: Could not create the file!" << std::endl;
	} else {
		outFile << "Log started at: " << timestamp << std::endl;  // Write log start time
	}

	bool success{ true };
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		success = false;
	} else
	{
		// Create window and renderer
		if (!SDL_CreateWindowAndRenderer("Tower Defense - NullTerminators", static_cast<int>(Global::kScreenWidth), static_cast<int>(Global::kScreenHeight), 0, &gWindow, &gRenderer))
		{
			SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		} else
		{
			// Enable VSync
			if (!SDL_SetRenderVSync(gRenderer, 1))
			{
				SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			// Set blending mode
			if (!SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND))
			{
				SDL_Log("Could not set blend mode! SDL error: %s\n", SDL_GetError());
				success = false;
			}

			// Initialize SDL_ttf
			if (!TTF_Init())
			{
				SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
				success = false;
			}

			if (!SDL_SetWindowIcon(gWindow, IMG_Load("assets/icon.png"))) {
				SDL_Log("Window Icon could not be set: %s\n", SDL_GetError());
				success = false;
			}

			//Set audio spec
			SDL_AudioSpec audioSpec;
			SDL_zero(audioSpec);
			audioSpec.format = SDL_AUDIO_F32;
			audioSpec.channels = 2;
			audioSpec.freq = 44100;

			//Open audio device
			gAudioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
			if (gAudioDeviceId == 0)
			{
				SDL_Log("Unable to open audio! SDL error: %s\n", SDL_GetError());
				success = false;
			} else
			{
				//Initialize SDL_mixer
				if (!Mix_OpenAudio(gAudioDeviceId, nullptr))
				{
					printf("SDL_mixer could not initialize! SDL_mixer error: %s\n", SDL_GetError());
					success = false;
				}
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
	std::string fontPath = "assets/fonts/lazy2.ttf";
	gFont = TTF_OpenFont(fontPath.c_str(), 28);
	if (gFont == nullptr)
	{
		SDL_Log("Failed to load font %s: %s", fontPath.c_str(), SDL_GetError());
		success = false;
	}

	return success;
}

/**
 * @brief Cleans up SDL resources and quits SDL subsystems.
 */
void close() {
	//Free music
	Mix_CloseAudio();
	SDL_CloseAudioDevice(gAudioDeviceId);
	gAudioDeviceId = 0;

	TextureManager::getInstance().deallocateAllTextures();

	// Free font
	TTF_CloseFont(gFont);
	gFont = nullptr;

	// Destroy window and renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	
	delete bg;
	bg = nullptr;

	// Quit SDL subsystems
	Mix_Quit();
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
	if (!init())
	{
		SDL_Log("Unable to initialize program!\n");
		exitCode = 1;
	} else
	{
		// Load media
		if (!loadMedia())
		{
			SDL_Log("Unable to load media!\n");
			exitCode = 2;
		} else
		{
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

			// Create TextureManager Singleton Instance
			TextureManager::getInstance().init(gRenderer, gFont);
			gFpsTexture.loadFromRenderedText("Enter to start/stop or space to pause/unpause", { 0x00, 0x00, 0x00, 0xFF });

			bg = new ParallaxBackground();
			std::srand(static_cast<unsigned int>(std::time(0)));

			for (int i = 0; i < Global::numberOfProps; ++i)
			{
				float randomSpeed = 5.0f + std::rand() % 11;
				bg->addLayer(randomSpeed, Global::kScreenHeight);
			}

			AudioManager::getInstance().init(AudioManager::MAX_VOLUME / 4);
			Global::gMusic = AudioManager::getInstance().loadMusic("music.wav");

			// Start game state machine
			gCurrentState = IntroState::get();
			gCurrentState->enter();
			SDL_SetRenderVSync(gRenderer, 1);

			Mix_PlayMusic(Global::gMusic, -1);

			// Main game loop
			while (!quit)
			{
				// Start frame timer
				capTimer.start();

				// Process events
				while (SDL_PollEvent(&e))
				{
					gCurrentState->handleEvent(e);

					if (e.type == SDL_EVENT_QUIT)
					{
						setNextState(ExitState::get());
						quit = true;
					} else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
					{
						setNextState(TitleState::get());
					}
				}

				// Handle fading transition
				if (isFading)
				{
					// Correct fadeProgress calculation (scaled from 0 to 1)
					Uint64 elapsedTime = fadeTimer.getTicksNS();
					float fadeProgress = static_cast<float>(elapsedTime) / static_cast<float>(FADE_DURATION);
					fadeAlpha = std::clamp(fadeIn ? (1.0f - fadeProgress) * 255.0f : fadeProgress * 255.0f, 0.0f, 255.0f);

					// If fade-out is complete, switch game states
					if (fadeAlpha >= 255 && !fadeIn)
					{
						gCurrentState->exit();
						gNextState->enter();
						gCurrentState = gNextState;
						gCurrentState->update();
						gNextState = nullptr;
						fadeIn = true;
						fadeTimer.start();
					}

					// If fade-in is complete, stop fading
					if (fadeAlpha <= 0 && fadeIn)
					{
						isFading = false;
						gCurrentState->update();
					}
				}

				// Update the current state if not transitioning
				if (!isFading)
				{
					bg->update(0.016f);
					gCurrentState->update();
					changeState();
				}

				// Clear the screen
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
				SDL_RenderClear(gRenderer);

				// Render the current state
				bg->render();
				gCurrentState->render();

				// Apply fade effect if needed
				if (isFading)
				{
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, static_cast<Uint8>(fadeAlpha));
					SDL_RenderFillRect(gRenderer, nullptr);
				}

				// Present the updated frame
				SDL_RenderPresent(gRenderer);

				// Cap frame rate
				constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps;
				Uint64 frameNs = capTimer.getTicksNS();
				if (frameNs < nsPerFrame)
				{
					SDL_DelayNS(nsPerFrame - frameNs);
				}
			}
		}
	}

	if (outFile.is_open()) {
		outFile.close();
	}

	// Cleanup and exit
	close();
	return exitCode;
}
