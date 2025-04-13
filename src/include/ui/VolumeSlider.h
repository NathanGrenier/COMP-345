#pragma once

#include <SDL3/SDL.h>
#include <ui/Texture.h>

class VolumeSlider {
public:
    VolumeSlider(const std::string& barPath, const std::string& knobPath, float x, float y, int minVol = 0, int maxVol = 100, int initialVol = 50);

    void handleEvent(SDL_Event& e);
    void render();

    int getVolume() const { return mVolume; }

private:
    Texture mBarTexture;
    Texture mKnobTexture;

    float mX, mY;     // Bar position
    bool mDragging;

    int mMinVolume;
    int mMaxVolume;
    int mVolume;

    SDL_FRect mCurrentBarRect;
    SDL_FRect mCurrentKnobRect;
};
