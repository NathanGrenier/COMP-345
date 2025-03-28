#include <string>
#include <SDL3_image/SDL_image.h>
#include <ui/LTexture.h>
#include <Global.h>
#include <filesystem>
#include <random>

/**
 * @class LTexture
 * @brief A wrapper class for handling SDL textures.
 *
 * This class provides methods for loading textures from images or text,
 * rendering them, and applying transformations like color modulation, alpha transparency, and blending.
 *
 * Textures are only loaded once, then cached for reuse.
 *
 * @author Nirav Patel, Nathan Grenier
 */
std::unordered_map<std::string, SDL_Texture*> LTexture::sTextureCache;

LTexture::LTexture()
	: mTexture(nullptr), mOwnsTexture(true), mWidth(0), mHeight(0),
	mR(255), mG(255), mB(255), mAlpha(255) {}

LTexture::LTexture(SDL_Texture* texture, bool owns)
	: mTexture(texture), mOwnsTexture(owns), mR(255), mG(255), mB(255), mAlpha(255) {
	if (mTexture) {
		SDL_GetTextureSize(mTexture, &mWidth, &mHeight);
	} else {
		mWidth = 0;
		mHeight = 0;
	}
}

/**
 * @brief Destructor that cleans up the texture.
 */
LTexture::~LTexture() {
	if (mOwnsTexture && mTexture) {
		destroy();
	}
}

void LTexture::deallocateAllTextures() {
	for (auto& pair : sTextureCache) {
		SDL_DestroyTexture(pair.second);
	}
	sTextureCache.clear();
}

LTexture LTexture::loadFromFile(std::string path) {
	std::string fullPath = TEXTURE_PATH + path;
	auto it = sTextureCache.find(fullPath);
	SDL_Texture* texture = nullptr;

	if (it != sTextureCache.end()) {
		texture = it->second; // Use cached texture
	} else {
		SDL_Surface* loadedSurface = IMG_Load(fullPath.c_str());
		if (!loadedSurface) {
			SDL_Log("Unable to load image %s! SDL_image error: %s\n", fullPath.c_str(), SDL_GetError());
			return;
		}
		// TODO: Refactor so that color of transparent pixels can be specified (but will default to values below)
		SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0x00, 0xFF, 0xFF));
		texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (!texture) {
			SDL_Log("Unable to create texture from %s! SDL error: %s\n", fullPath.c_str(), SDL_GetError());
			return;
		}
		sTextureCache[fullPath] = texture;

		SDL_DestroySurface(loadedSurface);
	}
	return LTexture(texture, false); // Instance does not own the texture
}

/**
 * @brief Creates a texture from rendered text using SDL_ttf.
 *
 * @param textureText The text to render.
 * @param textColor The color of the text.
 * @return True if the texture was successfully created, false otherwise.
 */
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
	// Clean up existing texture
	destroy();

	// Load text surface
	if (SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), 0, textColor); textSurface == nullptr) {
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
	} else {
		// Create texture from surface
		if (mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface); mTexture == nullptr) {
			SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		} else {
			mWidth = static_cast<float>(textSurface->w);
			mHeight = static_cast<float>(textSurface->h);
		}

		// Free temp surface
		SDL_DestroySurface(textSurface);
	}

	// Return success if texture loaded
	return mTexture != nullptr;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
	destroy();
	SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), 0, textColor);
	if (!textSurface) {
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
		return false;
	}
	mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (!mTexture) {
		SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	} else {
		mWidth = static_cast<float>(textSurface->w);
		mHeight = static_cast<float>(textSurface->h);
		mOwnsTexture = true; // Text textures are unique and owned
	}
	SDL_DestroySurface(textSurface);
	return mTexture != nullptr;
}

/**
 * @brief Frees the texture and resets its dimensions.
 */
void LTexture::destroy() {
	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

/**
 * @brief Modifies the texture's color.
 *
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 */
void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	mR = r;
	mG = g;
	mB = b;
}

void LTexture::setAlpha(Uint8 alpha) {
	mAlpha = alpha;
}

/**
 * @brief Sets the blending mode of the texture.
 *
 * @param blendMode The SDL_BlendMode to apply.
 */
void LTexture::setBlending(SDL_BlendMode blendMode) {
	SDL_SetTextureBlendMode(mTexture, blendMode);
}

void LTexture::setTextureSize(float newWidth, float newHeight) {
	if (newWidth > 0 && newHeight > 0) {
		mWidth = newWidth;
		mHeight = newHeight;
	}
}

/**
 * @brief Renders the texture with optional clipping, scaling, rotation, and flipping.
 *
 * @param x The x-coordinate of the render position.
 * @param y The y-coordinate of the render position.
 * @param clip The section of the texture to render (nullptr for full texture).
 * @param width The desired width (if <= 0, maintains aspect ratio).
 * @param height The desired height (if <= 0, maintains aspect ratio).
 * @param degrees The rotation angle in degrees.
 * @param center The point to rotate around (nullptr for center).
 * @param flipMode The SDL_FlipMode to apply.
 */
void LTexture::render(float x, float y, SDL_FRect* clip, float width, float height,
					  double degrees, float sizeMulti, SDL_FPoint* center, SDL_FlipMode flipMode) {
	if (!mTexture) return;

	SDL_FRect dstRect = { x, y, mWidth, mHeight };
	float aspectRatio = mWidth / mHeight;

	// Default to clip dimensions if clip is given
	if (clip) {
		dstRect.w = clip->w;
		dstRect.h = clip->h;
	}

	// Resize while maintaining aspect ratio if necessary
	if (width > 0 && height <= 0) {
		// Only width is provided, calculate height based on aspect ratio
		dstRect.w = width;
		dstRect.h = dstRect.w / aspectRatio;
	} else if (height > 0 && width <= 0) {
		// Only height is provided, calculate width based on aspect ratio
		dstRect.h = height;
		dstRect.w = dstRect.h * aspectRatio;
	} else {
		// If both width and height are provided, use them directly
		if (width > 0) dstRect.w = width;
		if (height > 0) dstRect.h = height;
	}

	// Scale Size
	float centerX = dstRect.x + dstRect.w / 2.0f;
	float centerY = dstRect.y + dstRect.h / 2.0f;
	dstRect.w *= sizeMulti;
	dstRect.h *= sizeMulti;
	dstRect.x = centerX - dstRect.w / 2.0f;
	dstRect.y = centerY - dstRect.h / 2.0f;

	// Apply per-instance color and alpha before rendering
	SDL_SetTextureColorMod(mTexture, mR, mG, mB);
	SDL_SetTextureAlphaMod(mTexture, mAlpha);

	SDL_RenderTextureRotated(gRenderer, mTexture, clip, &dstRect, degrees, center, flipMode);
}

std::string LTexture::getRandomBackground(const std::string& directory) {
	std::vector<std::string> files;

	// Iterate through the directory and collect file names
	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			files.push_back(entry.path().string());
		}
	}

	// Ensure there are files to pick from
	if (files.empty()) {
		SDL_Log("No background images found in %s\n", directory.c_str());
		return "";
	}

	// Select a random file
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distrib(0, files.size() - 1);

	return files[distrib(gen)];
}

/**
 * @brief Gets the width of the texture.
 *
 * @return The width of the texture in pixels.
 */
float LTexture::getWidth() const {
	return mWidth;
}

/**
 * @brief Gets the height of the texture.
 *
 * @return The height of the texture in pixels.
 */
float LTexture::getHeight() const {
	return mHeight;
}