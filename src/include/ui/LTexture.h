/**
 * @file LTexture.h
 * @brief The header file for the LTexture class.
 * @author Nirav Patel
 * @date 2025-02-16
 */
#pragma once

#include <string>
#include <SDL3/SDL_render.h>
class LTexture {
public:
	//Symbolic constant
	static constexpr float kOriginalSize = -1.f;

	//Initializes texture variables
	LTexture();

	//Cleans up texture variables
	~LTexture();

	//Loads texture from disk
	bool loadFromFile(std::string path);

	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Cleans up texture
	void destroy();

	//Sets color modulation
	void setColor(Uint8 r, Uint8 g, Uint8 b);

	//Sets opacity
	void setAlpha(Uint8 alpha);

	//Sets blend mode
	void setBlending(SDL_BlendMode blendMode);

	void setTextureSize(int newWidth, int newHeight);

	//Draws texture
	void render(float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE);

	//Gets texture dimensions
	int getWidth();
	int getHeight();

private:
	//Contains texture data
	SDL_Texture* mTexture;

	//Texture dimensions
	int mWidth;
	int mHeight;
};
