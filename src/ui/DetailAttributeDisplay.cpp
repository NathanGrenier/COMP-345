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
    components.push_back(new DetailLabel(componentWidth, "assets/ui/BuyTower.png"));
    
    // buttons to buy Towers
    components.push_back(new DetailButton(componentWidth, "assets/ui/StandardTower.png"));
    components.push_back(new DetailButton(componentWidth, "assets/ui/RapidFireTower.png"));
    components.push_back(new DetailButton(componentWidth, "assets/ui/CannonTower.png"));

    int currentY = DetailDisplayComponent::DETAIL_COMPONENT_PADDING;
    
    for (int i = 0; i < components.size(); i++) {
        components[i]->setComponentPosition(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY);

        currentY += DetailDisplayComponent::DETAIL_COMPONENT_SPACING;
    }

    towerObserver = new TowerObserver(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY + DetailDisplayComponent::DETAIL_COMPONENT_SPACING);
    
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
    return towerObserver->initializeTowerComponents();
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


void DetailAttributeDisplay::handleButtonEvents(SDL_Event* e)
{
    (dynamic_cast<DetailButton*>(components[1]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(components[2]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(components[3]))->handleEvent(e);
    towerObserver->handleButtonEvents(e);
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

