#include <states/parts/Part3State.h>
#include "../../critter/CritterGroup.cpp"
#include <Global.h>

SDL_FRect startSquare = { 0.0f, 300.0f, 50.0f, 50.0f };  // Start square at left edge (adjust position as needed)
SDL_FRect endSquare = { Global::kScreenWidth - 50.0f, 300.0f, 50.0f, 50.0f };  // End square at right edge
int waveLevel = 1;

Part3State Part3State::sPart3State;

Part3State* Part3State::get() {
    return &sPart3State;
}

bool Part3State::enter() {

    return true;
}

bool Part3State::exit() {
    return true;
}

// Part3State.cpp (handleEvent function)
void Part3State::handleEvent(SDL_Event& e) {
    // Handle keyboard input for moving start and end squares
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
        case SDLK_W:  // Move start square up
            startSquare.y -= 10.0f;  // Adjust step size as needed
            break;
        case SDLK_S:  // Move start square down
            startSquare.y += 10.0f;
            break;
        case SDLK_UP:  // Move end square up
            endSquare.y -= 10.0f;
            break;
        case SDLK_DOWN:  // Move end square down
            endSquare.y += 10.0f;
            break;
        }
    }
}

void Part3State::update() {
    critterGroup.generateCritters(startSquare, endSquare, 0.16f);
    critterGroup.update(0.016f, endSquare);
}


// Part3State.cpp (render function)
void Part3State::render() {
    // Set color to green for the start and end squares
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);

    // Render start square (S)
    SDL_RenderFillRect(gRenderer, &startSquare);  // Draw the start square
    renderText("S", startSquare.x + startSquare.w / 4, startSquare.y + startSquare.h / 4);  // Label for 'S'

    // Render end square (E)
    SDL_RenderFillRect(gRenderer, &endSquare);  // Draw the end square
    renderText("E", endSquare.x + endSquare.w / 4, endSquare.y + endSquare.h / 4);  // Label for 'E'

    critterGroup.render(gRenderer);
}

// renderText helper function (assuming a function to render text on the screen)
void Part3State::renderText(const std::string& text, float x, float y) {
    SDL_Color textColor = { 255, 255, 255, 255 };  // White color for the label
    LTexture textTexture;
    textTexture.loadFromRenderedText(text, textColor);

    // Render the text at the given position
    textTexture.render(x, y);
}

Part3State::Part3State() : critterGroup{ waveLevel, startSquare, endSquare } {

}