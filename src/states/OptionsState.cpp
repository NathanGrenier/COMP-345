#include <SDL3/SDL_events.h>
#include <states/OptionsState.h>
#include <SDL3/SDL_init.h>
#include <Global.h>
#include <states/TitleState.h>
#include <states/MainGameState.h>

float desiredHeight = 80;

static int prevVolumeMusic = -1;
static int prevVolumeUI = -1;
static int prevVolumeGame = -1;

OptionsState OptionsState::sOptionsState;

OptionsState* OptionsState::get() {
    return &sOptionsState;
}

bool OptionsState::enter() {
    bg = new ParallaxBackground();
    std::srand(std::time(0));

    for (int i = 0; i < Global::numberOfProps; ++i)
    {
        float randomSpeed = 5.0f + std::rand() % 11;
        bg->addLayer(randomSpeed, Global::kScreenHeight);
    }

    AudioManager::getInstance();

    backButton.loadFromFile("ui/LeftArrow.png");
    optionsMenuMessage.loadFromFile("ui/OptionsMenu.png");
	checkMark.loadFromFile("ui/CheckMark.png");
	disabledMark.loadFromFile("ui/DisabledMark.png");

    animationMessage.loadFromRenderedText("Background Animation", {255, 255, 255, 255});

    volumeMessageMusic.loadFromFile("ui/VolumeMusic.png");
    volumeMessageUI.loadFromFile("ui/VolumeUI.png");
    volumeMessageGame.loadFromFile("ui/VolumeGame.png");

    volumeNumberMusic.loadFromRenderedText(std::to_string(AudioManager::getInstance().getMusicLevelMapped(Global::gMusic)), { 255, 255, 255, 255 });
    volumeNumberUI.loadFromRenderedText(std::to_string(AudioManager::getInstance().getChannelVolumeMapped(AudioManager::eEffectChannelUI)), { 255, 255, 255, 255 });
    volumeNumberGame.loadFromRenderedText(std::to_string(AudioManager::getInstance().getChannelVolumeMapped(AudioManager::eEffectChannelTowerShot)), { 255, 255, 255, 255 });

    constexpr int spacing = 20;
    float screenWidth = Global::kScreenWidth;
    float screenHeight = Global::kScreenHeight;

    float titleWidth = screenWidth * 0.5f;
    float titleHeight = (static_cast<float>(optionsMenuMessage.getHeight()) / optionsMenuMessage.getWidth()) * titleWidth;
    float backButtonSize = titleHeight * 1.1;
    float renderedWidth = Global::kScreenWidth * 0.5f;
    float backButtonY = titleDistanceFromTop;

    backButton.setSizeWithAspectRatio(backButtonSize, 0);
    backButton.setPosition(backButtonDistanceFromLeft, backButtonY);

    float volumeMessageX = screenWidth * 0.45f;
    float volumeSliderX = screenWidth * 0.55f;
    float firstVolumeY = screenHeight * 0.28f;

    volumeSliderMusic = new VolumeSlider("ui/VolumeBar.png", "ui/VolumeKnob.png", volumeMessageX, firstVolumeY, 0, AudioManager::MAX_VOLUME, AudioManager::getInstance().getMusicVolumeLevel(Global::gMusic));
    volumeSliderUI = new VolumeSlider("ui/VolumeBar.png", "ui/VolumeKnob.png", volumeMessageX, firstVolumeY + spacing + desiredHeight, 0, AudioManager::MAX_VOLUME, AudioManager::getInstance().getChannelVolumeLevel(AudioManager::eEffectChannelUI));
    volumeSliderGame = new VolumeSlider("ui/VolumeBar.png", "ui/VolumeKnob.png", volumeMessageX, firstVolumeY + 2 * (spacing + desiredHeight), 0, AudioManager::MAX_VOLUME, AudioManager::getInstance().getChannelVolumeLevel(AudioManager::eEffectChannelTowerShot));

    return true;
}

bool OptionsState::exit() {
    delete bg;
    bg = nullptr;

    delete volumeSliderMusic;
    delete volumeSliderUI;
    delete volumeSliderGame;

    volumeSliderMusic = nullptr;
    volumeSliderUI = nullptr;
    volumeSliderGame = nullptr;

    return true;
}

void OptionsState::handleEvent(SDL_Event& e) {
    backButton.handleEvent(&e);
    checkMark.handleEvent(&e);
	disabledMark.handleEvent(&e);
    volumeSliderMusic->handleEvent(e);
    volumeSliderUI->handleEvent(e);
    volumeSliderGame->handleEvent(e);

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
    {
		if (backButton.isClicked())
		{
			if (MainGameState::optionsMenu)
			{
				setNextState(MainGameState::get());
			}
            else
            {
                setNextState(TitleState::get());
            }
        }
        if (checkMark.isClicked() && ParallaxBackground::isMoving == true)
        {
            ParallaxBackground::isMoving = false;
        }
        else if (disabledMark.isClicked() && ParallaxBackground::isMoving == false)
        {
            ParallaxBackground::isMoving = true;
        }
    }
}

void OptionsState::update() {
    int mappedVolumeMusic = volumeSliderMusic->getVolume();
    int mappedVolumeUI = volumeSliderUI->getVolume();
    int mappedVolumeGame = volumeSliderGame->getVolume();

    // Update only if the volume has changed
    if (mappedVolumeMusic != prevVolumeMusic) {
        AudioManager::getInstance().setMusicVolumeLevel(mappedVolumeMusic);
        prevVolumeMusic = mappedVolumeMusic;
    }

    if (mappedVolumeUI != prevVolumeUI) {
        AudioManager::getInstance().setChannelVolumeLevel(AudioManager::eEffectChannelUI, mappedVolumeUI);
        prevVolumeUI = mappedVolumeUI;
    }

    if (mappedVolumeGame != prevVolumeGame) {
        AudioManager::getInstance().setGameVolume(mappedVolumeGame);
        prevVolumeGame = mappedVolumeGame;
    }

    // Always update the volume display numbers
    volumeNumberMusic.loadFromRenderedText(std::to_string(
        static_cast<int>(AudioManager::getInstance().getMusicLevelMapped(Global::gMusic))),
        { 255, 255, 255, 255 });

    volumeNumberUI.loadFromRenderedText(std::to_string(
        static_cast<int>(AudioManager::getInstance().getChannelVolumeMapped(AudioManager::eEffectChannelUI))),
        { 255, 255, 255, 255 });

    volumeNumberGame.loadFromRenderedText(std::to_string(
        static_cast<int>(AudioManager::getInstance().getChannelVolumeMapped(AudioManager::eEffectChannelTowerShot))),
        { 255, 255, 255, 255 });

    if (ParallaxBackground::isMoving) {
        checkMark.update();
    }
    else {
        disabledMark.update();
    }

    backButton.update();
    bg->update(0.016f);
}


void OptionsState::render() {
    bg->render();

    float kScreenWidth = Global::kScreenWidth;
    float kScreenHeight = Global::kScreenHeight;

    // Render title
    optionsMenuMessage.render(
        (kScreenWidth - kScreenWidth * 0.5f) / 2,
        10.0f,
        nullptr,
        kScreenWidth * 0.5f,
        -1);

    float volumeMessageX = kScreenWidth * 0.25f; // Align messages to the left
    float volumeSliderX = kScreenWidth * 0.55f; // Sliders positioned slightly right
    float volumeNumberX = volumeSliderX + 120.0f;

    float firstVolumeY = kScreenHeight * 0.2f;
    constexpr int spacing = 20;

    backButton.render();

    // Render the volume messages (above and to the left of the sliders)
    volumeMessageMusic.render(volumeMessageX, firstVolumeY - 10, nullptr, -1, desiredHeight / 3);
    volumeMessageUI.render(volumeMessageX, firstVolumeY + spacing + desiredHeight - 10, nullptr, -1, desiredHeight / 3);
    volumeMessageGame.render(volumeMessageX, firstVolumeY + 2 * (spacing + desiredHeight) - 10, nullptr, -1, desiredHeight / 3);

    // Render volume sliders
    volumeSliderMusic->render();
    volumeSliderUI->render();
    volumeSliderGame->render();

    // Render volume numbers on the right side of the sliders
    volumeNumberMusic.render(volumeNumberX, firstVolumeY, nullptr, -1, desiredHeight / 3);
    volumeNumberUI.render(volumeNumberX, firstVolumeY + spacing + desiredHeight, nullptr, -1, desiredHeight / 3);
    volumeNumberGame.render(volumeNumberX, firstVolumeY + 2 * (spacing + desiredHeight), nullptr, -1, desiredHeight / 3);

	// Render check marks or disabled marks
    animationMessage.render(volumeMessageX, firstVolumeY + 3 * (spacing + desiredHeight), nullptr, -1, desiredHeight / 3);

    // Set position and size for checkMark and disabledMark
    checkMark.setPosition(volumeNumberX + 10, firstVolumeY + 3 * (spacing + desiredHeight));
    disabledMark.setPosition(volumeNumberX + 10, firstVolumeY + 3 * (spacing + desiredHeight));

    checkMark.setSizeWithAspectRatio(0, desiredHeight / 3);
    disabledMark.setSizeWithAspectRatio(0, desiredHeight / 3);

    if (ParallaxBackground::isMoving) {
        checkMark.render();
        checkMark.update();
    }
    else {
        disabledMark.render();
        disabledMark.update();
    }
}

