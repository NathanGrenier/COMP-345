#include <string>
#include <SDL3_image/SDL_image.h>
#include <ui/Texture.h>
#include <util/TextureManager.h>
#include <Global.h>
#include <filesystem>
#include <random>

/**
 * @class Texture
 * @brief A wrapper class for handling SDL textures.
 *
 * This class provides methods for loading textures from images or text,
 * rendering them, and applying transformations like color modulation, alpha transparency, and blending.
 *
 * Textures are only loaded once, then cached for reuse.
 *
 * @author Nirav Patel, Nathan Grenier
 */
Texture::Texture() : mR(255), mG(255), mB(255), mAlpha(255) {}

Texture::~Texture() {
	if (mModifiedTexture) {
		SDL_DestroyTexture(mModifiedTexture);
	}
	mBaseTexture = nullptr;
}

bool Texture::loadFromFile(const std::string& filePath, bool loadNew) {
	if (!loadNew && isTextureLoaded()) {
		return true;
	}

	mBaseTexture = TextureManager::getInstance().loadTexture(filePath);
	if (mBaseTexture) {
		SDL_GetTextureSize(mBaseTexture, &mWidth, &mHeight);
		fileName = filePath;
		return true;
	} else {
		std::cout << "Texture Failed to Load: " << filePath << std::endl;
	}
	return false;
}

bool Texture::loadFromRenderedText(const std::string& text, SDL_Color textColor) {
	mBaseTexture = TextureManager::getInstance().loadTextTexture(text, textColor);
	if (mBaseTexture) {
		SDL_GetTextureSize(mBaseTexture, &mWidth, &mHeight);
		return true;
	}
	return false;
}

void Texture::ensureUniqueTexture() {
	if (!mModifiedTexture && mBaseTexture) {
		// Create a copy of the base texture
		float width, height;
		SDL_GetTextureSize(mBaseTexture, &width, &height);
		mModifiedTexture = SDL_CreateTexture(TextureManager::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetTextureBlendMode(mModifiedTexture, SDL_BLENDMODE_BLEND);

		// Copy the base texture into the modified texture
		SDL_SetRenderTarget(TextureManager::getInstance().getRenderer(), mModifiedTexture);
		SDL_RenderTexture(TextureManager::getInstance().getRenderer(), mBaseTexture, nullptr, nullptr);
		SDL_SetRenderTarget(TextureManager::getInstance().getRenderer(), nullptr);
	}
}

/**
 * @brief Modifies the texture's color.
 *
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 */
void Texture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	ensureUniqueTexture();
	mR = r;
	mG = g;
	mB = b;
}

void Texture::setAlpha(Uint8 alpha) {
	ensureUniqueTexture();
	mAlpha = alpha;
}

/**
 * @brief Sets the blending mode of the texture.
 *
 * @param blendMode The SDL_BlendMode to apply.
 */
void Texture::setBlendMode(SDL_BlendMode blendMode) {
	ensureUniqueTexture();
	if (mModifiedTexture) {
		SDL_SetTextureBlendMode(mModifiedTexture, blendMode);
	}
}

void Texture::setTextureSize(float newWidth, float newHeight) {
	ensureUniqueTexture();
	if (mModifiedTexture && newWidth > 0 && newHeight > 0) {
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
void Texture::render(float x, float y, SDL_FRect* clip, float width, float height,
					 double degrees, float sizeMulti, SDL_FPoint* center, SDL_FlipMode flipMode) {
	SDL_Texture* textureToRender = mModifiedTexture ? mModifiedTexture : mBaseTexture;
	if (!textureToRender) {
		std::cout << "No texture to render for: " << fileName << std::endl;
		//SDL_Log("No texture to render.\n");
		return;
	}

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

	// Apply per-instance color and alpha before rendering. Should only ever be applied to the modifiedTexture
	if (mModifiedTexture) {
		SDL_SetTextureColorMod(textureToRender, mR, mG, mB);
		SDL_SetTextureAlphaMod(textureToRender, mAlpha);
	}

	SDL_RenderTextureRotated(TextureManager::getInstance().getRenderer(), textureToRender, clip, &dstRect, degrees, center, flipMode);
}

std::string Texture::getRandomBackground(const std::string& directory) {
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
float Texture::getWidth() const {
	return mWidth;
}

/**
 * @brief Gets the height of the texture.
 *
 * @return The height of the texture in pixels.
 */
float Texture::getHeight() const {
	return mHeight;
}