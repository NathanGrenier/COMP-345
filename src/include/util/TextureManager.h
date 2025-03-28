#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <tuple>
#include <map>

class TextureManager {
public:
	static TextureManager& getInstance();
	void init(SDL_Renderer* renderer, TTF_Font* font);
	void deallocateAllTextures();

	SDL_Texture* loadTexture(const std::string& filepath, bool useColorKey = false, SDL_Color transparentColor = { 0x00, 0x00, 0x00, 0xFF });
	SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color);

	SDL_Renderer* getRenderer() const { return mRenderer; }
private:
	TextureManager() = default;
	~TextureManager() = default;

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	SDL_Renderer* mRenderer = nullptr;
	TTF_Font* mFont = nullptr;
	std::map<std::tuple<std::string, Uint8, Uint8, Uint8>, SDL_Texture*> mTextureCache;
	std::map<std::tuple<std::string, Uint8, Uint8, Uint8, Uint8>, SDL_Texture*> mTextTextureCache;

	static inline const std::string TEXTURE_PATH = "assets/";
};