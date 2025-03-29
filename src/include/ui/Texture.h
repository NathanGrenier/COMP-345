/**
 * @file LTexture.h
 * @brief The header file for the LTexture class.
 * @author Nirav Patel, Nathan Grenier
 * @date 2025-03-28
 */
#pragma once

#include <SDL3/SDL.h>
#include <string>

class Texture {
public:
	// Constructors
	Texture();
	~Texture();

	// Static methods for loading and caching
	bool loadFromFile(const std::string& filename, bool loadNew = false);
	bool loadFromRenderedText(const std::string& text, SDL_Color color);

	std::string getRandomBackground(const std::string& directory);
	std::string fileName = "";

	// Utility methods
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setAlpha(Uint8 alpha);
	void setBlendMode(SDL_BlendMode blendMode);
	void setTextureSize(float newWidth, float newHeight);
	void render(float x, float y, SDL_FRect* clip = nullptr, float width = 0, float height = 0,
				double degrees = 0, float sizeMulti = 1.0f, SDL_FPoint* center = nullptr,
				SDL_FlipMode flipMode = SDL_FLIP_NONE);

	// Accessors
	float getWidth() const;
	float getHeight() const;
	bool isBaseTextureSet() const { return mBaseTexture != nullptr; }
	bool isModifiedTextureSet() const { return mModifiedTexture != nullptr; }
	bool isTextureLoaded() const { return isBaseTextureSet(); }
private:
	// Textures
	SDL_Texture* mBaseTexture = nullptr;  // Cached texture (not owned)
	SDL_Texture* mModifiedTexture = nullptr;  // Owned by this instance

	// Dimensions
	float mWidth = 0.0f;;
	float mHeight = 0.0f;;

	// Per-instance rendering properties
	Uint8 mR, mG, mB; // Color modulation
	Uint8 mAlpha;     // Alpha modulation

	void ensureUniqueTexture();
};