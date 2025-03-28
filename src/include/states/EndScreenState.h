/**
 * @file EndScreenState.h
 * @brief The header file for the EndScreenState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include "GameState.h"
#include "../UI/LTexture.h"
#include "../UI/LButton.h"
#include <critter/Critter.h>
#include <string>

class EndScreenState : public GameState {
public:
    /// @brief Number of buttons in the main menu.
    static const int BUTTON_COUNT = 3;

    /// @brief Number of statistics in the main menu.
    static const int STAT_COUNT = 4;

    /// @brief Number of statistics in the main menu.
    static constexpr float END_SCREEN_BUTTON_HEIGHT = 40.0f;
    static constexpr float END_SCREEN_STATS_HEIGHT = 10.0f;
    static constexpr float END_SCREEN_STATS_WIDTH = 60.0f;
    static constexpr float END_SCREEN_UI_SPACING = 20.0f;
    static constexpr float END_SCREEN_TITLE_WIDTH = 350.0f;
    const static std::string DEFAULT_KILLER_CRITTER_STR;

    //Static accessor
    static EndScreenState* get();

    //Transitions
    bool enter() override;
    bool exit() override;

    //Main loop functions
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render() override;

    //Statistics mutators
    void setWave(int wave);
    void setTowersBought(int towersBought);
    void setCrittersKilled(int crittersKilled);
    void setKilledBy(std::string killedBy);

private:
    //Static instance
    static EndScreenState sEndScreenState;

    LTexture messageTexture;

    ParallaxBackground* bg;
    int wave = -1;
    int towersBought = -1;
    int crittersKilled = -1;
    std::string killedBy = DEFAULT_KILLER_CRITTER_STR;

    /// @brief Array of main menu buttons.
    LButton* buttons;

    /// @brief Array of statistic labels.
    LTexture* statLabels;

    /// @brief Array of statistics.
    LTexture* stats;
};
