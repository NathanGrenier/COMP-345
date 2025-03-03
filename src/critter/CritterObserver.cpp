#include <critter/CritterObserver.h>
#include <ui/DetailLabel.h>
#include <ui/DetailAttribute.h>
#include <ui/DetailButton.h>
#include <format>
#include <string>
#include <ui/DetailAttributeDisplay.h>

CritterObserver::CritterObserver(int startingX, int startingY) : currentCritter(nullptr) {
    int componentWidth = DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

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
}

void CritterObserver::update(Observable* observable) {
    if (observable == currentCritter) {
        updateAttributes();
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
    for (int i = 0; i < critterComponents.size(); i++) {
        delete critterComponents[i];
    }
    critterComponents.clear();
}

void CritterObserver::updateAttributes() {
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    std::string speedStr = std::to_string(currentCritter->getSpeed());
    std::string hitPointsStr = std::to_string(currentCritter->getHitPoints());
    std::string strengthStr = std::to_string(currentCritter->getStrength());
    std::string rewardStr = std::to_string(currentCritter->getReward());

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
