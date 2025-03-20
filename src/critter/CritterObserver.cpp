#include <critter/CritterObserver.h>
#include <ui/DetailLabel.h>
#include <ui/DetailAttribute.h>
#include <ui/DetailButton.h>
#include <format>
#include <string>
#include <ui/DetailAttributeDisplay.h>

/**
 * @class CritterObserver
 * @brief Observes and displays information about a critter.
 *
 * The CritterObserver class manages the display of various attributes of a critter,
 * such as speed, hit points, strength, and reward. It provides functionality for
 * updating the information when the critter's state changes and renders visual
 * indicators like a border around the critter.
 */
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

/**
 * @brief Initializes the components displaying critter attributes.
 *
 * Sets the labels for the critter attributes like speed, hit points, strength, and reward.
 *
 * @return True if all components were initialized successfully, false otherwise.
 */
bool CritterObserver::initializeCritterComponents() {
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    bool success1 = (dynamic_cast<DetailAttribute*>(critterComponents[1]))->setAttributeText("Speed", textColor);
    bool success2 = (dynamic_cast<DetailAttribute*>(critterComponents[2]))->setAttributeText("Hit Points", textColor);
    bool success3 = (dynamic_cast<DetailAttribute*>(critterComponents[3]))->setAttributeText("Strength", textColor);
    bool success4 = (dynamic_cast<DetailAttribute*>(critterComponents[4]))->setAttributeText("Reward", textColor);

    return success1 && success2 && success3 && success4;
}

/**
 * @brief Renders the critter observer and its components.
 *
 * This function renders the components of the CritterObserver, and if the current
 * critter is not null, it also renders an animated border around the critter.
 */
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

/**
 * @brief Updates the critter observer based on changes in the observed critter.
 *
 * This function checks if the current critter has changed and updates the observer's
 * displayed attributes accordingly. It also handles critter death or exit events.
 *
 * @param observable The object being observed (the critter).
 */
void CritterObserver::update(Observable* observable) {
    if (observable == currentCritter) {
        updateAttributes();

        if (!currentCritter->isAlive() || currentCritter->atExit()) {
            setCurrentCritter(nullptr);
        }
    }
}

/**
 * @brief Sets the current critter being observed.
 *
 * @param critter The critter to observe.
 */
void CritterObserver::setCurrentCritter(Critter* critter) {
    currentCritter = critter;
}

/**
 * @brief Gets the current critter being observed.
 *
 * @return The current critter being observed.
 */
Critter* CritterObserver::getCurrentCritter() {
    return currentCritter;
}

/**
 * @brief Handles button events for the critter observer's components.
 *
 * This function forwards the event to the buttons in the observer's UI to handle interactions.
 *
 * @param e The SDL event to be handled.
 */
void CritterObserver::handleButtonEvents(SDL_Event* e) {
    (dynamic_cast<DetailButton*>(critterComponents[5]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(critterComponents[6]))->handleEvent(e);
}

/**
 * @brief Destructor for the CritterObserver.
 *
 * Cleans up any resources used by the CritterObserver.
 */
CritterObserver::~CritterObserver() {
    currentCritter = nullptr;

    critterComponents.clear();
}

/**
 * @brief Updates the attribute values displayed for the current critter.
 *
 * This function updates the attribute text for the critter's speed, hit points, strength,
 * and reward based on the current critter's state.
 */
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

/**
 * @brief Formats the value string to display the current and upgrade values.
 *
 * If the critter has reached its max level, only the current value is displayed.
 * Otherwise, both the current value and the upgrade value are shown.
 *
 * @param currentValue The current value of the critter attribute.
 * @param upgradeValue The value of the next upgrade to the critter attribute.
 * @param critterAtMaxLevel Whether the critter has reached its maximum level.
 * @return A formatted string displaying the current and upgrade values.
 */
std::string CritterObserver::formatValueStr(int currentValue, int upgradeValue, bool critterAtMaxLevel) {
    if (critterAtMaxLevel) {
        return std::to_string(currentValue);
    }
    return std::format("{} ( +{} )", currentValue, upgradeValue);
}
