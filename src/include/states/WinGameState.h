/**
 * @file WinGameState.h
 * @brief The header file for the WinGameState class.
 * @author Denmar Ermitano
 * @date 2025-03-26
 */
#pragma once

#include <states/EndScreenState.h>
#include <UI/Texture.h>
#include <string>
#include <nlohmann/json.hpp>

class WinGameState : public EndScreenState {
public:
    const static std::string QUOTE_FILE_PATH;
    
    //Transitions
    bool enter() override;

    //Main loop functions
    void render() override;

    //Static accessor
    static WinGameState* get();

private:
    //Static instance
    static WinGameState sWinGameState;

    std::string pickQuote(const std::string& filePath);
    nlohmann::json loadQuoteData(const std::string& filePath);
    
    //Win quote
    Texture winQuote;
};
