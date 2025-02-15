#include <SDL3_ttf/SDL_ttf.h>
#ifndef GLOBAL_H
#define GLOBAL_H

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

class Global
{
public:
	//Screen dimension constants
	static const int kScreenWidth{ 640 };
	static const int kScreenHeight{ 480 };
};
#endif