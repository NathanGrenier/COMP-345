#include <ui/TowerObserver.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailAttributeDisplay.h>
#include <ui/DetailLabel.h>
#include <ui/DetailAttribute.h>
#include <format>
#include <string>



#include <iostream>


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

    for (int i = 0; i < towerComponents.size(); i++) {
        towerComponents[i]->setComponentPosition(startingX, startingY);

        if (i > 5 || i == 0)
        {
            startingY += DetailDisplayComponent::DETAIL_COMPONENT_SPACING;
        }
        else 
        {
            startingY += DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
        }
    }
}


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

std::vector<DetailDisplayComponent*> TowerObserver::getTowerComponents()
{
    return towerComponents;
}

void TowerObserver::render()
{
    int componentChangeNum = towerComponents.size();

    for (int i = 0; i < buyTowers.size(); i++)
    {
        if (currentTower == buyTowers[i])
        {
            componentChangeNum -= 2;
            break;
        }
    }

    for (int i = 0; i < componentChangeNum; i++) {
        towerComponents[i]->render();
    }
}

TowerObserver::~TowerObserver()
{
    for (int i = 0; i < towerComponents.size(); i++) 
    {
        delete towerComponents[i];
    }

    towerComponents.clear();
}

void TowerObserver::update(Observable* observable)
{
    if (observable == currentTower) 
    {
        // update the attributes on the view

        updateAttributes();
    }
}

void TowerObserver::setCurrentTower(Tower* tower)
{
    currentTower = tower;
}

Tower* TowerObserver::getCurrentTower() 
{
    return currentTower;
}

void TowerObserver::handleButtonEvents(SDL_Event* e)
{
    (dynamic_cast<DetailButton*>(towerComponents[7]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(towerComponents[8]))->handleEvent(e);
}

void TowerObserver::updateAttributes()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    Tower::UpgradeValues upgradeValues = currentTower->getUpgradeValues();
    int maxLevel = currentTower->getMaxLevel();
    int currentLevel = currentTower->getLevel();
    bool towerAtMaxLevel = (maxLevel == currentLevel);

    std::string rangeStr = formatValueStr(currentTower->getRange(), upgradeValues.rangeIncrease, towerAtMaxLevel);
    std::string powerStr = formatValueStr(currentTower->getPower(), upgradeValues.powerIncrease, towerAtMaxLevel);
    std::string rateOfFireStr = formatValueStr(currentTower->getRateOfFire(), upgradeValues.rateOfFireIncrease, towerAtMaxLevel);
    std::string levelStr = std::format("{} / {}", currentTower->getLevel(), currentTower->getMaxLevel());


    std::string upgradeCostStr;
    if (towerAtMaxLevel)
    {
        upgradeCostStr = "-";
    }
    else
    {
        upgradeCostStr = std::to_string(currentTower->getUpgradeCost());
    }

    std::string refundValueStr = std::format("{}", currentTower->getRefundValue());

    (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setValueText(rangeStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setValueText(powerStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setValueText(rateOfFireStr, textColor);

    (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setValueText(levelStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setValueText(upgradeCostStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setValueText(refundValueStr, textColor);
}

std::string TowerObserver::formatValueStr(int currentValue, int upgradeValue, bool towerAtMaxLevel)
{
    std::string valueStr;

    if (upgradeValue == 0 || towerAtMaxLevel)
    {
        return valueStr = std::to_string(currentValue);
    }

    return valueStr = std::format("{} ( +{} )", currentValue, upgradeValue);
}

void TowerObserver::addToBuyTowers(Tower* tower)
{
    buyTowers.push_back(tower);
}

