/**
 * @file TextureLoader.cpp
 * @brief Implementation of the Texture Loader Class
 *
 * @details This file implements a texture loading system that manages SDL textures,
 * including loading from files and memory management. It implements texture caching
 * to prevent loading the same texture multiple times.
 */

#include <util/TextureLoader.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

 /** @brief Static map storing all loaded textures */
std::unordered_map<std::string, SDL_Texture*> TextureLoader::loadedTextures;

/**
 * @brief Gets the static map of loaded textures
 *
 * @details This function implements the Singleton pattern for the texture cache,
 * ensuring there's only one instance of the texture map throughout the program's lifetime.
 *
 * @return Reference to the static map of loaded textures
 */
static std::unordered_map<std::string, SDL_Texture*>& getLoadedTextures() {
	static std::unordered_map<std::string, SDL_Texture*> loadedTextures;
	return loadedTextures;
}

/**
 * @brief Load a texture from a file and return it
 *
 * @details This function implements texture caching. If the requested texture
 * has already been loaded, it returns the cached version instead of loading
 * it again. New textures are automatically added to the cache.
 *
 * @param renderer The SDL renderer used to create the texture
 * @param filename The path to the texture file, relative to the 'assets' folder
 * @return SDL_Texture* The loaded texture, or nullptr if loading failed
 *
 * @note The returned texture has SDL_BLENDMODE_BLEND enabled by default
 * @note The function automatically manages the temporary SDL_Surface used during loading
 */
SDL_Texture* TextureLoader::loadTexture(SDL_Renderer* renderer, std::string filename) {
	if (filename == "") {
		return nullptr;
	}

	auto& textures = getLoadedTextures();
	auto found = textures.find(filename);

	// Check to see if texture is already loaded.
	if (found != textures.end()) {
		return found->second;
	} else {
		std::string filepath = TextureLoader::TEXTURE_PATH + filename;

		SDL_Surface* surfaceTemp = IMG_Load(filepath.c_str());
		if (surfaceTemp != nullptr) {
			SDL_Texture* textureOutput = SDL_CreateTextureFromSurface(renderer, surfaceTemp);

			//Free the surface because it's no longer needed. 
			SDL_DestroySurface(surfaceTemp);

			if (textureOutput != nullptr) {
				//Enable transparency for the texture.
				SDL_SetTextureBlendMode(textureOutput, SDL_BLENDMODE_BLEND);

				textures[filename] = textureOutput;

				return textureOutput;
			}
		}
	}
}


/**
 * @brief Destroy all loaded textures and clear the texture cache
 *
 * @details This function should be called during cleanup to prevent memory leaks.
 * It iterates through all cached textures, properly destroys each one using SDL,
 * and clears the texture map.
 *
 * @warning After calling this function, any existing pointers to previously loaded
 * textures will be invalid
 */
void TextureLoader::deallocateTextures() {
	auto& textures = getLoadedTextures();

	while (textures.empty() == false) {
		auto e = textures.begin();
		if (e->second != nullptr)
			SDL_DestroyTexture(e->second);

		textures.erase(e);
	}
}