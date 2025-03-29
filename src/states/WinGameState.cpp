/** @class WinGameState
 *  @brief Implementation of the WinGameState.
 *  @author Denmar Ermitano
 */

#include <states/WinGameState.h>
#include <states/EndScreenState.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Global.h>

 /// Static instance of the WinGameState
WinGameState WinGameState::sWinGameState;

const std::string WinGameState::QUOTE_FILE_PATH = "assets/winQuotes/Quotes.json"; /**< @brief Path of randomized quotes */

/**
 * @brief Gets the singleton instance of WinGameState.
 *
 * @return Pointer to the WinGameState instance.
 */
WinGameState* WinGameState::get() {
	return &sWinGameState;
}

/**
 * @brief Enters the WinGameState, initializes textures and buttons.
 *
 * Loads the title text and initializes the buttons with corresponding labels and positions.
 *
 * @return True if successful, false otherwise.
 */
bool WinGameState::enter() {
	bool success = true;

	// quote for winning
	std::string quote = pickQuote(WinGameState::QUOTE_FILE_PATH);
	SDL_Color textColor{ 73, 178, 227, 0xFF };

	winQuote.loadFromRenderedText(quote, textColor);

	bool endScreenSuccess = EndScreenState::enter();

	return success && endScreenSuccess;
}

/**
 * @brief Renders the WinGameState screen.
 *
 * Clears the screen, renders the background, title text, and menu buttons.
 */
void WinGameState::render() {
	EndScreenState::render();

	// render winning quote
	float quoteHeight = (1.0f / 4.0f) * Global::kScreenHeight;
	float quoteWidth = Global::kScreenWidth / 2.0f;

	winQuote.render(
		(Global::kScreenWidth - quoteWidth) / 2.0f,
		quoteHeight,
		nullptr,
		quoteWidth, 0);
}

/**
 * @brief picks a random quote from a json file
 * @param filePath the file path of the json file
 * @return the randomly selected quote
 * @return specific quote for when there is no json file
 */
std::string WinGameState::pickQuote(const std::string& filePath)
{
	// loads the quote data
	nlohmann::json quoteData = loadQuoteData(filePath);

	// checks for quotes
	if (quoteData["count"] != 0)
	{
		// modulo on random to fit range
		int quoteIndex = std::rand() % quoteData["count"];

		// selects a quote
		return quoteData["quotes"][quoteIndex]["text"];
	}
	
	return "You also broke the game! :)";
}

/**
 * @brief Loads quote data from a JSON file.
 *
 * @param filePath The path to the JSON file.
 * @details Opens the specified file, reads its content, and attempts to parse it as JSON.
 *          If the file cannot be opened or the content is not valid JSON, an error message
 *          is printed to the standard error stream and an empty JSON object is returned.
 * 
 * @return nlohmann::json The loaded JSON data. 
 * @return An empty JSON object on failure.
 */
nlohmann::json WinGameState::loadQuoteData(const std::string& filePath)
{
	// Open the file
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filePath << std::endl;
		return {};  // Return an empty json object on failure
	}

	// Attempt to parse the file into a json object
	try {
		nlohmann::json tempQuoteData;
		file >> tempQuoteData;

		// Ensure the file content is valid JSON
		if (file.fail()) {
			std::cerr << "Error: Failed to parse the JSON from file " << filePath << std::endl;
			return {};  // Return an empty json object if parsing fails
		}

		return tempQuoteData;
	}
	catch (const nlohmann::json::parse_error& e) {
		std::cerr << "Error: Parsing failed. Exception: " << e.what() << std::endl;
		return {};  // Return an empty json object if an exception occurs
	}
}
