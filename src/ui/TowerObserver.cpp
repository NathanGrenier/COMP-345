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
    towerComponents.push_back(new DetailLabel(componentWidth));

    // attribute for Towers
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));
    towerComponents.push_back(new DetailAttribute(componentWidth));

    // buttons to sell and upgrade Towers
    towerComponents.push_back(new DetailButton(componentWidth));
    towerComponents.push_back(new DetailButton(componentWidth));

    for (int i = 0; i < towerComponents.size(); i++) {
        towerComponents[i]->setPosition(startingX + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, startingY);

        startingY += towerComponents[i]->getHeight() + DetailDisplayComponent::DETAIL_COMPONENT_PADDING;
    }
}


bool TowerObserver::initializeTowerComponents()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    (dynamic_cast<DetailLabel*>(towerComponents[0]))->setText("Tower Information", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setAttributeText("Range", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setAttributeText("Power", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setAttributeText("Rate of fire", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setAttributeText("Level", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setAttributeText("Upgrade cost", textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setAttributeText("Refund value", textColor);
    (dynamic_cast<DetailButton*>(towerComponents[7]))->setText("Upgrade Tower", textColor);
    (dynamic_cast<DetailButton*>(towerComponents[8]))->setText("Sell Tower", textColor);

    //updateAttributes();

    return true;
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
    for (int i = 0; i < towerComponents.size(); i++) {
        delete(towerComponents[i]);
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

void TowerObserver::updateAttributes()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

    Tower::UpgradeValues upgradeValues = currentTower->getUpgradeValues();

    std::string rangeStr = formatValueStr(currentTower->getRange(), upgradeValues.rangeIncrease);
    std::string powerStr = formatValueStr(currentTower->getPower(), upgradeValues.powerIncrease);
    std::string rateOfFireStr = formatValueStr(currentTower->getRateOfFire(), upgradeValues.rateOfFireIncrease);
    std::string levelStr = std::format("{} / {}", currentTower->getLevel(), currentTower->getMaxLevel());
    std::string upgradeCostStr = std::format("{}", currentTower->getUpgradeCost());
    std::string refundValueStr = std::format("{}", currentTower->getRefundValue());

    (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setValueText(rangeStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setValueText(powerStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setValueText(rateOfFireStr, textColor);

    (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setValueText(levelStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setValueText(upgradeCostStr, textColor);
    (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setValueText(refundValueStr, textColor);
}

std::string TowerObserver::formatValueStr(int currentValue, int upgradeValue)
{
    std::string valueStr;

    if (upgradeValue == 0)
    {
        return valueStr = std::to_string(currentValue);
    }

    return valueStr = std::format("{} ( +{} )", currentValue, upgradeValue);
}

void TowerObserver::addToBuyTowers(Tower* tower)
{
    buyTowers.push_back(tower);
}

