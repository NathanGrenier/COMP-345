#pragma once

#include <states/GameState.h>
#include <ui/VolumeSlider.h>
#include <ui/LButton.h>
#include <util/AudioManager.h>
#include <ui/ParallaxBackground.h>
#include <ui/Texture.h>

class OptionsState : public GameState {
public:
    //Static accessor
    static OptionsState* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

private:
    static OptionsState sOptionsState;

    LButton backButton;
    ParallaxBackground* bg;
    Texture optionsMenuMessage;

    VolumeSlider* volumeSliderUI;
    Texture volumeMessageUI;
    Texture volumeNumberUI;

    VolumeSlider* volumeSliderGame;
    Texture volumeMessageGame;
    Texture volumeNumberGame;

    VolumeSlider* volumeSliderMusic;
    Texture volumeMessageMusic;
    Texture volumeNumberMusic;

    Texture animationMessage;
    LButton checkMark;
    LButton disabledMark;
};