#include <string>
#include <SDL3_image/SDL_image.h>
#include <ui/LTexture.h>
#include <Global.h>

/**
 * @class LTexture
 * @brief A wrapper class for handling SDL textures.
 *
 * This class provides methods for loading textures from images or text,
 * rendering them, and applying transformations like color modulation, alpha transparency, and blending.
 *
 * @author Nirav Patel
 */
LTexture::LTexture() :
	// Initialize texture variables
	mTexture{ nullptr },
	mWidth{ 0 },
	mHeight{ 0 } {
}

/**
 * @brief Destructor that cleans up the texture.
 */
LTexture::~LTexture() {
	// Clean up texture
	destroy();
}

/**
 * @brief Loads an image from a file and creates an SDL texture.
 *
 * @param path The file path to the image.
 * @return True if the texture was successfully loaded, false otherwise.
 */
bool LTexture::loadFromFile(std::string path) {
	// Clean up texture if it already exists
	destroy();

	// Load surface
	if (SDL_Surface* loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr) {
		SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
	}
	else {
		// Color key image (removes cyan background)
		if (!SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0x00, 0xFF, 0xFF))) {
			SDL_Log("Unable to color key! SDL error: %s", SDL_GetError());
		}
		else {
			// Create texture from surface
			if (mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface); mTexture == nullptr) {
				SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError());
			}
			else {
				// Get image dimensions
				mWidth = static_cast<float>(loadedSurface->w);
				mHeight = static_cast<float>(loadedSurface->h);
			}
		}

		// Clean up loaded surface
		SDL_DestroySurface(loadedSurface);
	}

	// Return success if texture loaded
	return mTexture != nullptr;
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
	}
	else {
		// Create texture from surface
		if (mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface); mTexture == nullptr) {
			SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			mWidth = static_cast<float>(textSurface->w);
			mHeight = static_cast<float>(textSurface->h);
		}

		// Free temp surface
		SDL_DestroySurface(textSurface);
	}

	// Return success if texture loaded
	return mTexture != nullptr;
}

/**
 * @brief Frees the texture and resets its dimensions.
 */
void LTexture::destroy() {
	// Clean up texture
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
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

/**
 * @brief Adjusts the texture's alpha transparency.
 *
 * @param alpha The alpha value (0-255), where 0 is fully transparent and 255 is fully opaque.
 */
void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
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
void LTexture::render(float x, float y, SDL_FRect* clip, float width, float height, double degrees, SDL_FPoint* center, SDL_FlipMode flipMode) {
	// Set texture position
	SDL_FRect dstRect = { x, y, static_cast<float>(mWidth), static_cast<float>(mHeight) };

	// Calculate the aspect ratio of the texture
	float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);

	// Default to clip dimensions if clip is given
	if (clip != nullptr) {
		dstRect.w = clip->w;
		dstRect.h = clip->h;
	}

	// Resize while maintaining aspect ratio if necessary
	if (width > 0 && height <= 0) {
		// Only width is provided, calculate height based on aspect ratio
		dstRect.w = width;
		dstRect.h = dstRect.w / aspectRatio;
	}
	else if (height > 0 && width <= 0) {
		// Only height is provided, calculate width based on aspect ratio
		dstRect.h = height;
		dstRect.w = dstRect.h * aspectRatio;
	}
	else {
		// If both width and height are provided, use them directly
		if (width > 0) dstRect.w = width;
		if (height > 0) dstRect.h = height;
	}

	// Render texture with the updated dimensions
	SDL_RenderTextureRotated(gRenderer, mTexture, clip, &dstRect, degrees, center, flipMode);
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
