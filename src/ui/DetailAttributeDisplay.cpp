#include <ui/DetailAttributeDisplay.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailLabel.h>
#include <ui/DetailButton.h>
#include <ui/DetailAttribute.h>
#include <Global.h>

DetailAttributeDisplay::DetailAttributeDisplay()
    : DetailDisplay::DetailDisplay(ATTRIBUTE_DISPLAY_WIDTH, Global::kScreenHeight)
{
    setPosition(Global::kScreenWidth - getWidth(), 0);

    int componentWidth = ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

    // label to buy Towers
    components.push_back(new DetailLabel(componentWidth));
    
    // buttons to buy Towers
    components.push_back(new DetailButton(componentWidth));
    components.push_back(new DetailButton(componentWidth));
    components.push_back(new DetailButton(componentWidth));

    int currentY = DetailDisplayComponent::DETAIL_COMPONENT_PADDING;
    
    for (int i = 0; i < components.size(); i++) {
        components[i]->setPosition(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY);

        currentY += components[i]->getHeight() + DetailDisplayComponent::DETAIL_COMPONENT_PADDING;
    }

    towerObserver = new TowerObserver(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY + 5 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING);
    
}


std::vector<DetailDisplayComponent*> DetailAttributeDisplay::getComponents() 
{
    return components;
}

std::vector<DetailDisplayComponent*> DetailAttributeDisplay::getTowerComponents()
{
    return towerObserver->getTowerComponents();
}

bool DetailAttributeDisplay::initializeComponents()
{
    SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
    (dynamic_cast<DetailLabel*>(components[0]))->setText("Buy Tower", textColor);

    (dynamic_cast<DetailButton*>(components[1]))->setText("Standard Tower", textColor);
    (dynamic_cast<DetailButton*>(components[2]))->setText("Rapid Fire Tower", textColor);
    (dynamic_cast<DetailButton*>(components[3]))->setText("Cannon Tower", textColor);
    
    towerObserver->initializeTowerComponents();

    return true;
}

void DetailAttributeDisplay::selectTower(Tower* tower)
{
    towerObserver->setCurrentTower(tower);

    // needs to unselect Critter
    // likewise, Critter needs to unselect Tower
}

bool DetailAttributeDisplay::isDisplayingTower()
{
    return towerObserver->getCurrentTower() != nullptr;
}

TowerObserver* DetailAttributeDisplay::getTowerObserver()
{
    return towerObserver;
}

/**
 * @brief 
 */
void DetailAttributeDisplay::render()
{
    DetailDisplay::render();
    
    for (int i = 0; i < components.size(); i++) {
        components[i]->render();
    }

    if (isDisplayingTower())
    {
        towerObserver->render();
    }
}

//DetailAttributeDisplay::~DetailAttributeDisplay()
//{
//    for (int i = 0; i < components.size(); i++) {
//        delete components[i];
//    }
//
//    components.clear();
//}

