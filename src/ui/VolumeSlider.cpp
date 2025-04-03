#include <ui/VolumeSlider.h>
#include <iostream>

VolumeSlider::VolumeSlider(const std::string& barPath, const std::string& knobPath, int x, int y, int minVol, int maxVol, int initialVol)
    : mX(x), mY(y), mMinVolume(minVol), mMaxVolume(maxVol), mDragging(false) {
    mBarTexture.loadFromFile(barPath);
    mKnobTexture.loadFromFile(knobPath);

    // Clamp the initial volume to be within bounds
    mVolume = std::max(mMinVolume, std::min(initialVol, mMaxVolume));

    // Compute dimensions
    int barWidth = mBarTexture.getWidth();
    int knobWidth = mKnobTexture.getWidth();

    // Set the knob position based on the initial volume
    float percent = float(mVolume - mMinVolume) / (mMaxVolume - mMinVolume);
    mKnobX = x + percent * (barWidth - knobWidth);
}

void VolumeSlider::handleEvent(SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = static_cast<float>(e.button.x);
        float my = static_cast<float>(e.button.y);

        // Check if the click is inside the knob
        if (mx >= mCurrentKnobRect.x && mx <= mCurrentKnobRect.x + mCurrentKnobRect.w &&
            my >= mCurrentKnobRect.y && my <= mCurrentKnobRect.y + mCurrentKnobRect.h) {
            mDragging = true;
        }
    }
    else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        mDragging = false;
    }
    else if (e.type == SDL_EVENT_MOUSE_MOTION && mDragging) {
        float mx = static_cast<float>(e.motion.x);
        float minX = mCurrentBarRect.x;
        float maxX = mCurrentBarRect.x + mCurrentBarRect.w - mCurrentKnobRect.w;

        mCurrentKnobRect.x = std::max(minX, std::min(mx - (mCurrentKnobRect.w / 2), maxX));

        // Convert position to volume level
        float percent = (mCurrentKnobRect.x - minX) / (maxX - minX);
        mVolume = mMinVolume + int(percent * (mMaxVolume - mMinVolume));
    }
}

void VolumeSlider::render() {
    constexpr float multiplier = 0.5f;

    float barWidth = mBarTexture.getWidth() * multiplier;
    float barHeight = mBarTexture.getHeight() * multiplier;
    float knobWidth = mKnobTexture.getWidth() * multiplier;
    float knobHeight = mKnobTexture.getHeight() * multiplier;

    float barX = mX - (barWidth * multiplier);
    float barY = mY - (barHeight * multiplier);

    // Store the rendered rects for tracking
    mCurrentBarRect = { barX + 40.0f, barY, barWidth, barHeight };

    // Ensure mCurrentKnobRect is always correctly updated
    if (!mDragging) {
        float percent = float(mVolume - mMinVolume) / (mMaxVolume - mMinVolume);
        mCurrentKnobRect.x = mCurrentBarRect.x + percent * (mCurrentBarRect.w - knobWidth);
    }

    mCurrentKnobRect.y = barY + (barHeight - knobHeight) * multiplier;
    mCurrentKnobRect.w = knobWidth;
    mCurrentKnobRect.h = knobHeight;

    // Render the volume bar
    mBarTexture.render(mCurrentBarRect.x, mCurrentBarRect.y, nullptr, mCurrentBarRect.w, mCurrentBarRect.h);

    // Render the knob
    mKnobTexture.render(mCurrentKnobRect.x, mCurrentKnobRect.y, nullptr, mCurrentKnobRect.w, mCurrentKnobRect.h);
}




