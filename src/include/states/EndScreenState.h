/**
 * @file EndScreenState.h
 * @brief The header file for the EndScreenState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include <states/GameState.h>
#include <UI/Texture.h>
#include <UI/LButton.h>
#include <critter/Critter.h>
#include <string>

class EndScreenState : public GameState {
public:
    static const int BUTTON_COUNT = 3; /** @brief Number of buttons in the main menu. */
    static const int STAT_COUNT = 4; /** @brief Number of statistics in the main menu. */

    static constexpr float END_SCREEN_BUTTON_HEIGHT = 40.0f; /** @brief height of a button. */
    static constexpr float END_SCREEN_STATS_HEIGHT = 13.0f; /** @brief height of stats. */
    static constexpr float END_SCREEN_STATS_SPACING = 7.0f; /** @brief spacing of stats. */
    static constexpr float END_SCREEN_UI_SPACING = 20.0f; /** @brief spacing for UI elements. */
    static constexpr float END_SCREEN_TITLE_WIDTH = 350.0f; /** @brief width of title on endscreen. */
    const static std::string DEFAULT_KILLER_CRITTER_STR; /** @brief default critter type for when you win the game. */

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


protected:
    std::string killedBy = DEFAULT_KILLER_CRITTER_STR; /**< @brief type of critter that ended the game */

private:
    //Static instance
    static EndScreenState sEndScreenState; 

    Texture messageTexture; /**< @brief title for end screen */

    ParallaxBackground* bg;
    int wave = -1; /**< @brief wave game ended on */
    int towersBought = -1; /**< @brief numbers of towers bought. */
    int crittersKilled = -1; /**< @brief number of critters killed. */

    LButton* buttons; /**< @brief Array of main menu buttons. */
    Texture* statLabels; /**< @brief Array of statistic labels. */
    Texture* stats; /**< @brief Array of statistics */
};
