/**
 * @class DetailAttributeDisplay
 * @brief Implementation of the DetailAttributeDisplay class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <ui/DetailAttributeDisplay.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailLabel.h>
#include <ui/DetailButton.h>
#include <ui/DetailAttribute.h>
#include <Global.h>

/**
 * @brief Default constructor.
 */
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

/**
 * @brief Accessor for components
 * @return the array of DetailDisplayComponent object pointers
 */
std::vector<DetailDisplayComponent*> DetailAttributeDisplay::getComponents() 
{
    return components;
}

/**
 * @brief Accessor for Tower detail components
 * @return the array of DetailDisplayComponent object pointers for the TowerObserver
 */
std::vector<DetailDisplayComponent*> DetailAttributeDisplay::getTowerComponents()
{
    return towerObserver->getTowerComponents();
}

/**
 * @brief Accessor for the TowerObserver
 * @return the TowerObserver used in the DetailDisplay
 */
TowerObserver* DetailAttributeDisplay::getTowerObserver()
{
    return towerObserver;
}

/**
 * @brief Initalizes the components in the TowerObserver
 * 
 * @details Simply calls the function on towerComponents
 * @return true if DetailDisplayComponent objects were successfully created
 * @return false if DetailDisplayComponent objects could not be created
 */
bool DetailAttributeDisplay::initializeComponents()
{
    return towerObserver->initializeTowerComponents();
}

/**
 * @brief Selects Tower to be shown through the TowerObserver
 * @param tower the Tower to have details displayed in the TowerObserver
 */
void DetailAttributeDisplay::selectTower(Tower* tower)
{
    towerObserver->setCurrentTower(tower);

    // needs to unselect Critter
    // likewise, Critter needs to unselect Tower
}

/**
 * @brief Checks if Tower details are currently supposed to be displayed
 * @return true if Tower details should be displayed
 * @return false if Tower details should not be displayed, either empty or Critter
 */
bool DetailAttributeDisplay::isDisplayingTower()
{
    return towerObserver->getCurrentTower() != nullptr;
}

/**
 * @brief Handles hover/click events for buttons in the DetailAttributeDisplay
 * 
 * @details Calls the handleEvent(e) methods in the DetailButton's class
 * Also calls the TowerObserver object's buttons' handleEvent(e) methods
 * @param e the event to be handled for the DetailButton
 */
void DetailAttributeDisplay::handleButtonEvents(SDL_Event* e)
{
    // handles buy Tower buttons
    (dynamic_cast<DetailButton*>(components[1]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(components[2]))->handleEvent(e);
    (dynamic_cast<DetailButton*>(components[3]))->handleEvent(e);
    
    // handles buttons from TowerObserver
    towerObserver->handleButtonEvents(e);
}

/**
 * @brief Displays the UI
 */
void DetailAttributeDisplay::render()
{
    DetailDisplay::render();
    
    // renders the buttons to buy Towers
    for (int i = 0; i < components.size(); i++) {
        components[i]->render();
    }

    // displays Tower details through TowerObserver if needed
    if (isDisplayingTower())
    {
        towerObserver->render();
    }
}

