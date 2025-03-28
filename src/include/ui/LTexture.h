/**
 * @file LTexture.h
 * @brief The header file for the LTexture class.
 * @author Nirav Patel, Nathan Grenier
 * @date 2025-03-28
 */
#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>

class LTexture {
public:
	// Constructors
	LTexture();
	~LTexture();

	// Static methods for loading and caching
	static LTexture loadFromFile(std::string path);
	static void deallocateAllTextures();

	// Instance methods for loading unique textures (e.g., text)
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	std::string getRandomBackground(const std::string& directory);

	// Utility methods
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setAlpha(Uint8 alpha);
	void setBlending(SDL_BlendMode blendMode);
	void setTextureSize(float newWidth, float newHeight);
	void render(float x, float y, SDL_FRect* clip = nullptr, float width = 0, float height = 0,
				double degrees = 0, float sizeMulti = 1.0f, SDL_FPoint* center = nullptr,
				SDL_FlipMode flipMode = SDL_FLIP_NONE);

	// Accessors
	float getWidth() const;
	float getHeight() const;

	LTexture(const LTexture&) = delete;
	LTexture& operator=(const LTexture&) = delete;

private:
	LTexture(SDL_Texture* texture, bool owns = false);

	void destroy();

	// Texture and ownership
	SDL_Texture* mTexture;
	bool mOwnsTexture;

	// Dimensions
	float mWidth;
	float mHeight;

	// Per-instance rendering properties
	Uint8 mR, mG, mB; // Color modulation
	Uint8 mAlpha;     // Alpha modulation

	// Static cache
	static std::unordered_map<std::string, SDL_Texture*> sTextureCache;
	static inline const std::string TEXTURE_PATH = "assets/";
};