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

	// Constructor for shared textures
	LTexture(SDL_Texture* texture, bool owns);

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

	void setTextureSize(float newWidth, float newHeight);

	//Draws texture
	void render(float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize, double degrees = 0.0, float sizeMulti = 1.0f, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE);

	//Gets texture dimensions
	float getWidth() const;
	float getHeight() const;

	static std::string getRandomBackground(const std::string& directory);
private:
	//Contains texture data
	SDL_Texture* mTexture;
	bool mOwnsTexture;

	//Texture dimensions
	float mWidth;
	float mHeight;
};
