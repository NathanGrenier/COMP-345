#include <states/parts/Part3State.h>
#include "../../critter/CritterGroup.cpp"
#include "../../tower/CritterTower.cpp"
#include <Global.h>

SDL_FRect startSquare;  // Start square at left edge (adjust position as needed)
SDL_FRect endSquare;  // End square at right edge

int playerGold;
int waveLevel;

std::string warningMessage = "";
float warningTime = 0.0f;

Part3State Part3State::sPart3State;
CritterGroup* critterGroup;

Part3State* Part3State::get() {
    return &sPart3State;
}

bool Part3State::enter() {
    startSquare = { 0.0f, 300.0f, 50.0f, 50.0f };
    endSquare = { Global::kScreenWidth - 50.0f, 300.0f, 50.0f, 50.0f };
    playerGold = 100;
    waveLevel = 1;
    critterGroup = new CritterGroup(waveLevel, playerGold, startSquare, endSquare);
    return true;
}

bool Part3State::exit() {
    return true;
}

// Part3State.cpp (handleEvent function)
void Part3State::handleEvent(SDL_Event& e) {
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
    else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float x, y;
        SDL_GetMouseState(&x, &y);

        if (e.button.button == SDL_BUTTON_LEFT) {
            placeTower(x, y);
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            sellTower(x, y);
        }
    }
}

void Part3State::update() {
    critterGroup->generateCritters(startSquare, endSquare, 0.16f);
    critterGroup->update(0.016f, endSquare);

    for (auto& tower : towers) {
        tower.update(0.016f, critterGroup->getCritters());
    }
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

    critterGroup->render(gRenderer);

    for (auto& tower : towers) {
        tower.render(gRenderer);
    }

    // Render player gold in the top-left corner
    renderText("Gold: " + std::to_string(playerGold), 10.0f, 10.0f);

    // Render the current wave number at the top-center of the screen
    std::string waveText = "Wave: " + std::to_string(waveLevel);
    float waveX = (Global::kScreenWidth - (waveText.length() * 10.0f)) / 2.0f;
    float waveY = 10.0f;  // Just below the top edge with some margin
    renderText(waveText, waveX, waveY);

    // Render the warning message at the bottom-center of the screen
    if (!warningMessage.empty()) {
        // Center the warning message horizontally at the bottom-middle
        float warningX = (Global::kScreenWidth - (warningMessage.length() * 10.0f)) / 2.0f;
        float warningY = Global::kScreenHeight - 40.0f;  // Position at the bottom with some margin
        renderText(warningMessage, warningX, warningY);
    }
}

// renderText helper function
void Part3State::renderText(const std::string& text, float x, float y) {
    SDL_Color textColor = { 0, 0, 0, 255 };  // White color for the label
    LTexture textTexture;
    textTexture.loadFromRenderedText(text, textColor);

    // Render the text at the given position
    textTexture.render(x, y);
}

void Part3State::placeTower(int x, int y) {
    int towerCost = 20;

    if (CritterTower::canBuy(playerGold, towerCost)) {
        SDL_FRect newTowerPos = { static_cast<float>(x), static_cast<float>(y), 50.0f, 50.0f };

        // Check if any tower is too close to the new tower position
        for (CritterTower& tower : towers) {
            float dx = std::abs(newTowerPos.x - tower.getPosition().x);
            float dy = std::abs(newTowerPos.y - tower.getPosition().y);

            if (dx < 50.0f && dy < 50.0f) {
                return;  // Prevent placing tower if too close to another one
            }
        }

        // Deduct gold and place the tower
        CritterTower::buyTower(playerGold, towerCost);
        towers.emplace_back(towerCost, 20, 150.0f, 1.5f, newTowerPos);
    }
}


void Part3State::sellTower(int x, int y) {
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        float dx = std::abs(it->getPosition().x - x);
        float dy = std::abs(it->getPosition().y - y);


        if (dx < 50.0f && dy < 50.0f) {
            // Sell tower and remove it from the list
            CritterTower::sellTower(playerGold, it->getCost() / 2);
            towers.erase(it);

            break;
        }
    }
}


Part3State::Part3State() {

}