/**
 * @file TextureLoader.h
 * @brief Load and store textures.
 * @author Nathan Grenier
 * @date 2025-02-15
 */
#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

class TextureLoader {
public:
	static SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string filename);
	static void deallocateTextures();

private:
	static std::unordered_map<std::string, SDL_Texture*> loadedTextures;
	static inline const std::string TEXTURE_PATH = "assets/";
};