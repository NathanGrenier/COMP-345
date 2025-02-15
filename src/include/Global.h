#ifndef GLOBAL_H
#define GLOBAL_H

#include <states/GameState.h>
#include <SDL3_ttf/SDL_ttf.h>

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

class Global {
public:
	//Screen dimension constants
	static const int kScreenWidth{ 640 };
	static const int kScreenHeight{ 480 };
};

/* Function Prototypes */
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//State managers
void setNextState(GameState* nextState);
void changeState();
#endif