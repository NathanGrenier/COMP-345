#ifndef GLOBAL_H
#define GLOBAL_H

#include <states/GameState.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map/Map.h>

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

class Global {
public:
	//Screen dimension constants
	static const int kScreenWidth{ Map::PIXELS_PER_CELL * 15 };
	static const int kScreenHeight{ Map::PIXELS_PER_CELL * 11 };
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