#include <critter/CritterObserver.h>
#include <ui/DetailLabel.h>
#include <ui/DetailAttribute.h>
#include <ui/DetailButton.h>
#include <format>
#include <string>
#include <ui/DetailAttributeDisplay.h>

CritterObserver::CritterObserver(float startingX, float startingY) : currentCritter(nullptr) {
    float componentWidth = DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

    // Label for Critter information
    critterComponents.push_back(new DetailLabel(componentWidth, "assets/ui/CritterInformation.png"));

    // Attributes for Critter
    critterComponents.push_back(new DetailAttribute(componentWidth));
    critterComponents.push_back(new DetailAttribute(componentWidth));
    critterComponents.push_back(new DetailAttribute(componentWidth));
    critterComponents.push_back(new DetailAttribute(componentWidth));

    // Add spacing between components
    for (int i = 0; i < critterComponents.size(); i++) {
        critterComponents[i]->setComponentPosition(startingX, startingY);

        if (i > 3 || i == 0) {
            startingY += DetailDisplayComponent::DETAIL_COMPONENT_SPACING;
        }
        else {
            startingY += DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
        }
    }
}

bool CritterObserver::initializeCritterComponents() {
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    bool success1 = (dynamic_cast<DetailAttribute*>(critterComponents[1]))->setAttributeText("Speed", textColor);
    bool success2 = (dynamic_cast<DetailAttribute*>(critterComponents[2]))->setAttributeText("Hit Points", textColor);
    bool success3 = (dynamic_cast<DetailAttribute*>(critterComponents[3]))->setAttributeText("Strength", textColor);
    bool success4 = (dynamic_cast<DetailAttribute*>(critterComponents[4]))->setAttributeText("Reward", textColor);

    return success1 && success2 && success3 && success4;
}

void CritterObserver::render() {
    for (int i = 0; i < critterComponents.size(); i++) {
        critterComponents[i]->render();
    }

    // Check if the current critter exists
    if (currentCritter != nullptr) {
        // Get position and dimensions of the critter
        SDL_FRect critterRect = currentCritter->getCurrentRenderRect();

        // Padding around the critter
        const float padding = 4.0f;

        // Adjusted rectangle for padding
        float x = critterRect.x - padding;
        float y = critterRect.y - padding;
        float w = critterRect.w + 2 * padding;
        float h = critterRect.h + 2 * padding;

        // Set color for outline (black in this case)
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

        // Dot properties
        const int dotSpacing = 6;       // Space between dots
        const int dotThickness = 2;     // Thickness of dots
        const int speed = 100;          // Speed of animation (lower is faster)

        // Animation phase offset based on time
        int timeOffset = (SDL_GetTicks() / speed) % dotSpacing;

        // ---- Top Edge (right to left for counter-clockwise) ----
        for (float i = x + w - timeOffset; i > x; i -= dotSpacing) {
            for (float t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, i, y + t);
            }
        }

        // ---- Left Edge (bottom to top for counter-clockwise) ----
        for (float i = y + h - timeOffset; i > y; i -= dotSpacing) {
            for (float t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, x + w - 1 - t, i);
            }
        }

        // ---- Bottom Edge (left to right for counter-clockwise) ----
        for (float i = x + timeOffset; i < x + w; i += dotSpacing) {
            for (float t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, i, y + h - 1 - t);
            }
        }

        // ---- Right Edge (top to bottom for counter-clockwise) ----
        for (float i = y + timeOffset; i < y + h; i += dotSpacing) {
            for (float t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, x + t, i);
            }
        }
    }
}

void CritterObserver::update(Observable* observable) {
    if (observable == currentCritter) {
        updateAttributes();

        if (!currentCritter->isAlive() || currentCritter->atExit()) {
            setCurrentCritter(nullptr);
        }
    }
}

void CritterObserver::setCurrentCritter(Critter* critter) {
    currentCritter = critter;
}

Critter* CritterObserver::getCurrentCritter() {
    return currentCritter;
}

void CritterObserver::handleButtonEvents(SDL_Event* e) {
    (dynamic_cast<DetailButton*>(critterComponents[5]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(critterComponents[6]))->handleEvent(e);
}

CritterObserver::~CritterObserver() {
    currentCritter = nullptr;

    critterComponents.clear();
}

void CritterObserver::updateAttributes() {
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    std::string speedStr = std::to_string(static_cast<int>(currentCritter->getSpeed()));
    std::string hitPointsStr = std::to_string(static_cast<int>(currentCritter->getHitPoints()));
    std::string strengthStr = std::to_string(static_cast<int>(currentCritter->getStrength()));
    std::string rewardStr = std::to_string(static_cast<int>(currentCritter->getReward()));

    (dynamic_cast<DetailAttribute*>(critterComponents[1]))->setValueText(speedStr, textColor);
    (dynamic_cast<DetailAttribute*>(critterComponents[2]))->setValueText(hitPointsStr, textColor);
    (dynamic_cast<DetailAttribute*>(critterComponents[3]))->setValueText(strengthStr, textColor);
    (dynamic_cast<DetailAttribute*>(critterComponents[4]))->setValueText(rewardStr, textColor);
}

std::string CritterObserver::formatValueStr(int currentValue, int upgradeValue, bool critterAtMaxLevel) {
    if (critterAtMaxLevel) {
        return std::to_string(currentValue);
    }
    return std::format("{} ( +{} )", currentValue, upgradeValue);
}
