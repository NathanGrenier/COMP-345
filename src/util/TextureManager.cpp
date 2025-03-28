#include <util/TextureManager.h>

TextureManager& TextureManager::getInstance() {
	static TextureManager instance;
	return instance;
}

void TextureManager::init(SDL_Renderer* renderer, TTF_Font* font) {
	mRenderer = renderer;
	mFont = font;
}

SDL_Texture* TextureManager::loadTexture(const std::string& path, bool useColorKey, SDL_Color transparentColor) {
	std::string fullPath = TEXTURE_PATH + path;

	auto key = std::make_tuple(fullPath, transparentColor.r, transparentColor.g, transparentColor.b);
	auto it = mTextureCache.find(key);
	if (it != mTextureCache.end()) {
		return it->second; // Return cached texture
	}

	SDL_Surface* loadedSurface = IMG_Load(fullPath.c_str());
	if (!loadedSurface) {
		SDL_Log("Unable to load image %s! SDL_image error: %s\n", fullPath.c_str(), SDL_GetError());
		return nullptr;
	}

	if (useColorKey) {
		Uint32 colorKey = SDL_MapSurfaceRGB(loadedSurface, transparentColor.r, transparentColor.g, transparentColor.b);
		SDL_SetSurfaceColorKey(loadedSurface, true, colorKey);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
	if (!texture) {
		SDL_Log("Unable to create texture from %s! SDL error: %s\n", fullPath.c_str(), SDL_GetError());
		SDL_DestroySurface(loadedSurface);
		return nullptr;
	}

	mTextureCache[key] = texture;
	SDL_DestroySurface(loadedSurface);

	return texture;
}

SDL_Texture* TextureManager::loadTextTexture(const std::string& text, SDL_Color textColor) {
	auto key = std::make_tuple(text, textColor.r, textColor.g, textColor.b, textColor.a);
	auto it = mTextTextureCache.find(key);
	if (it != mTextTextureCache.end()) {
		return it->second;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(mFont, text.c_str(), 0, textColor);
	if (!surface) {
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	if (!texture) {
		SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	}
	SDL_DestroySurface(surface);

	mTextTextureCache[key] = texture;
	return texture;
}

void TextureManager::deallocateAllTextures() {
	for (auto& pair : mTextureCache) {
		SDL_DestroyTexture(pair.second);
	}
	mTextureCache.clear();

	for (auto& pair : mTextTextureCache) {
		SDL_DestroyTexture(pair.second);
	}
	mTextTextureCache.clear();
}