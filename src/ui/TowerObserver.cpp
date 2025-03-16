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
#include <format>
#include <string>

/**
 * @brief Constructor with starting position for placing DetailDisplayComponent
 * @param startingX horizontal starting position 
 * @param startingY vertical  starting position 
 */
TowerObserver::TowerObserver(int startingX, int startingY) : currentTower(nullptr)
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

    // adds spacing
    for (int i = 0; i < towerComponents.size(); i++) {
        towerComponents[i]->setComponentPosition(startingX, startingY);

        // adds spacing for DetailDisplayComponent: DetailButtons and DetailLabels
        if (i > 5 || i == 0)
        {
            startingY += DetailDisplayComponent::DETAIL_COMPONENT_SPACING;
        }
        else // regular spacing for DetailAttributes
        {
            startingY += DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
        }
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
 * @details Renders buy and sell button depending on whether a Tower is selected or if a Tower class is selected
 */
void TowerObserver::render()
{
    // omits the buy/sell button if a Tower subclass is selected
    int componentChangeNum = towerComponents.size();
    for (int i = 0; i < buyTowers.size(); i++)
    {
        // removes the indices of the last 2 Buttons
        if (currentTower == buyTowers[i])
        {
            componentChangeNum -= 2;
            break;
        }
    }

    // renders the DetailDisplayComponent
    for (int i = 0; i < componentChangeNum; i++) {
        towerComponents[i]->render();
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
