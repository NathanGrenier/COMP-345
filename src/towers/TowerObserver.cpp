/**
 * @class TowerObserver
 * @brief Implementation of the TowerObserver class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/TowerObserver.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailAttributeDisplay.h>
#include <ui/DetailLabel.h>
#include <ui/DetailAttribute.h>
#include <ui/DetailButton.h>
#include <towers/TargetNearExit.h>
#include <towers/TargetNearTower.h>
#include <towers/TargetStrongest.h>
#include <towers/TargetWeakest.h>
#include <format>
#include <string>
#include <towers/TowerGroup.h>

/**
 * @brief Constructor with starting position for placing DetailDisplayComponent
 * @param startingX horizontal starting position 
 * @param startingY vertical  starting position 
 */
TowerObserver::TowerObserver(int startingX, int startingY) : currentTower(nullptr), towerStrategyIndex(-1)
{
    int componentWidth = DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;
    
    // label for Tower information
    towerComponents.push_back(new DetailLabel(componentWidth, "assets/ui/TowerInformation.png"));

    // attribute for Towers
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));

    // buttons to sell and upgrade Towers
    towerComponents.push_back(new DetailButton(componentWidth, "assets/ui/UpgradeTower.png"));
    towerComponents.push_back(new DetailButton(componentWidth, "assets/ui/SellTower.png"));

    towerComponents.push_back(new DetailLabel(componentWidth, "assets/ui/CritterTargetting.png"));

    towerStrategyPaths = new std::string[STRATEGY_COUNT];
    towerStrategyPaths[0] = "assets/ui/TargetNearestToExit.png";
    towerStrategyPaths[1] = "assets/ui/TargetNearestToTower.png";
    towerStrategyPaths[2] = "assets/ui/TargetStrongest.png";
    towerStrategyPaths[3] = "assets/ui/TargetWeakest.png";

    towerComponents.push_back(new DetailButton(componentWidth, towerStrategyPaths[0]));

    // adds spacing
    for (int i = 0; i < towerComponents.size(); i++) {
        towerComponents[i]->setComponentPosition(startingX, startingY);
        int additionalSpacing = 0;

        // adds spacing for DetailDisplayComponent: DetailButtons and DetailLabels
        if (i > 6 || i == 0)
        {
            additionalSpacing = DetailDisplayComponent::DETAIL_COMPONENT_SPACING;

            if (i == 8)
            {
                additionalSpacing += DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
            }
        }
        else // regular spacing for DetailAttributes
        {
            additionalSpacing = DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
        }

        startingY += additionalSpacing;
    }
}

/**
 * @brief Initializes the DetailDisplayComponent that describe a Tower
 * @return true DetailDisplayComponents were initalized
 * @return false if could not initialize the DetailDisplayComponents
 */
bool TowerObserver::initializeTowerComponents()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    bool success1 = (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setAttributeText("Range", textColor);
    bool success2 = (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setAttributeText("Power", textColor);
    bool success3 = (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setAttributeText("Rate of fire", textColor);
    bool success4 = (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setAttributeText("Level", textColor);
    bool success5 = (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setAttributeText("Upgrade cost", textColor);
    bool success6 = (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setAttributeText("Refund value", textColor);

    return (success1 && success2 && success3 && success4 && success5 && success6);
}

/**
 * @brief Accessor for towerComponents vector
 * @return the DetailDisplayComponent that describe the Tower
 */
std::vector<DetailDisplayComponent*> TowerObserver::getTowerComponents()
{
    return towerComponents;
}

/**
 * @brief Renders the DetailDisplayComponent to describe a Tower
 * @details Renders upgrade, sell, and strategy button depending on whether a Tower is selected or if a Tower class is selected
 */
void TowerObserver::render() {
    // Omit the upgrade/sell/strategy button if a Tower subclass is selected
    int componentChangeNum = towerComponents.size();
    
    for (int i = 0; i < buyTowers.size(); i++) {
        if (currentTower == buyTowers[i]) {
            towerStrategyIndex = -1;
            componentChangeNum -= 4;
            break;
        }
    }

    // Render the DetailDisplayComponent
    for (int i = 0; i < componentChangeNum; i++) {
        towerComponents[i]->render();
    }

    // Check if currentTower exists
    if (currentTower != nullptr) {
        // Get position and dimensions of the tower
        SDL_FRect towerRect = currentTower->getCurrentRenderRect();

        // Padding around the tower
        const float padding = 4.0f;

        // Adjusted rectangle with padding
        float x = towerRect.x - padding;
        float y = towerRect.y - padding;
        float w = towerRect.w + 2 * padding;
        float h = towerRect.h + 2 * padding;

        // Set color for outline (black)
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

        // Dot properties
        const int dotSpacing = 6;     // Space between dots
        const int dotThickness = 2;   // Thickness of dots
        const int speed = 100;        // Speed of animation (lower = faster)

        // Animation offset based on time
        int timeOffset = (SDL_GetTicks() / speed) % dotSpacing;

        // ---- Top Edge (left to right for clockwise) ----
        for (int i = x + timeOffset; i < x + w; i += dotSpacing) {
            for (int t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, i, y + t);  // Inward thickness
            }
        }

        // ---- Right Edge (top to bottom for clockwise) ----
        for (int i = y + timeOffset; i < y + h; i += dotSpacing) {
            for (int t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, x + w - 1 - t, i);  // Inward thickness
            }
        }

        // ---- Bottom Edge (right to left for clockwise) ----
        for (int i = x + w - timeOffset; i > x; i -= dotSpacing) {
            for (int t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, i, y + h - 1 - t);  // Inward thickness
            }
        }

        // ---- Left Edge (bottom to top for clockwise) ----
        for (int i = y + h - timeOffset; i > y; i -= dotSpacing) {
            for (int t = 0; t < dotThickness; t++) {
                SDL_RenderPoint(gRenderer, x + t, i);  // Inward thickness
            }
        }
    }
}


/**
 * @brief Desctructor, deleting the DetailDisplayComponents
 */
TowerObserver::~TowerObserver()
{
    for (int i = 0; i < towerComponents.size(); i++) 
    {
        delete towerComponents[i];
    }

    towerComponents.clear();
}

/**
 * @brief Updates displayed Tower attributes if Observed tower launches notification
 * @param observable 
 */
void TowerObserver::update(Observable* observable)
{
    if (observable == currentTower) 
    {
        // update the attributes on the view
        updateAttributes();
        updateStrategyButton();
    }
}

/**
 * @brief Mutator for currentTower
 * @param tower the new Tower to be selected
 */
void TowerObserver::setCurrentTower(Tower* tower)
{
    currentTower = tower;
}

/**
 * @brief Accessor for currentTower
 * @return the Tower that is currently selected 
 */
Tower* TowerObserver::getCurrentTower() 
{
    return currentTower;
}

/**
 * @brief Handles button events for the buy and sell DetailButtons
 * @param e the SDL event to handle
 */
void TowerObserver::handleButtonEvents(SDL_Event& e)
{
    (dynamic_cast<DetailButton*>(towerComponents[7]))->handleEvent(&e);
    (dynamic_cast<DetailButton*>(towerComponents[8]))->handleEvent(&e);

    if (towerStrategyIndex >= 0)
    {
        (dynamic_cast<DetailButton*>(towerComponents[10]))->handleEvent(&e);
    }
}

/**
 * @brief Updates the displayed DetailAttribute values of the selected Tower
 * @details Adds upgrade increments next to current values if available (omitted if no increment or if max level)
 * Shows the maximum level next to current level
 * 
 */
void TowerObserver::updateAttributes()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    // checks level against max Tower level
    int maxLevel = currentTower->getMaxLevel();
    int currentLevel = currentTower->getLevel();
    bool towerAtMaxLevel = (maxLevel == currentLevel);

    // generates strings for DetailAttribute values
    Tower::UpgradeValues upgradeValues = currentTower->getUpgradeValues();
    std::string rangeStr = formatValueStr(currentTower->getRange(), upgradeValues.rangeIncrease, towerAtMaxLevel);
    std::string powerStr = formatValueStr(currentTower->getPower(), upgradeValues.powerIncrease, towerAtMaxLevel);
    std::string rateOfFireStr = formatValueStr(currentTower->getRateOfFire(), upgradeValues.rateOfFireIncrease, towerAtMaxLevel);
    std::string levelStr = std::format("{} / {}", currentTower->getLevel(), currentTower->getMaxLevel());
    std::string refundValueStr = std::format("{}", currentTower->getRefundValue());

    // shows dash if already max level as upgrade cost
    std::string upgradeCostStr;
    if (towerAtMaxLevel)
    {
        upgradeCostStr = "-";
    }
    else
    {
        upgradeCostStr = std::to_string(currentTower->getUpgradeCost());
    }

    // sets the text for the DetailAttribute values
    (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setValueText(rangeStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setValueText(powerStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setValueText(rateOfFireStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setValueText(levelStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setValueText(upgradeCostStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setValueText(refundValueStr, textColor);
}

/**
 * @brief 
 * 
 */
void TowerObserver::updateStrategyButton()
{
    TowerStrategy* critterTargettingStrategy = currentTower->getCritterTargettingStrategy();
    towerStrategyIndex = TowerGroup::getStrategyIndex(currentTower);

    (dynamic_cast<DetailButton*>(towerComponents[10]))->setComponentImagePath(towerStrategyPaths[towerStrategyIndex]);
}

/**
 * @brief Formats string to be displayed in DetailAttribute
 * @param currentValue current value of attribute 
 * @param upgradeValue upgrade value of attribute 
 * @param towerAtMaxLevel if Tower is at highest level
 * @return formatted string to represent the value of a DetailAttribute
 */
std::string TowerObserver::formatValueStr(int currentValue, int upgradeValue, bool towerAtMaxLevel)
{
    std::string valueStr;

    // if no upgrade available 
    // or if at max level
    if (upgradeValue == 0 || towerAtMaxLevel)
    {
        return valueStr = std::to_string(currentValue);
    }

    // formats with upgrade
    return valueStr = std::format("{} ( +{} )", currentValue, upgradeValue);
}

/**
 * @brief Adds dummy Tower for the DetailAttributeDisply to show before buying a Tower
 * @param tower Tower representing attributes
 */
void TowerObserver::addToBuyTowers(Tower* tower)
{
    buyTowers.push_back(tower);
}
