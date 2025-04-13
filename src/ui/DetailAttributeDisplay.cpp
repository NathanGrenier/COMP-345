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
#include <critter/CritterObserver.h>
#include <ui/TowerObserver.h>
#include <Global.h>

const float DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH = Global::viewerWidth;

/**
 * @brief Default constructor.
 */
DetailAttributeDisplay::DetailAttributeDisplay()
	: DetailDisplay(ATTRIBUTE_DISPLAY_WIDTH, Global::kScreenHeight)
{
	setPosition(Global::kScreenWidth - getWidth(), 0);

	float componentWidth = ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

	// Existing components for Towers
	components.push_back(new DetailLabel(componentWidth, "ui/BuyTower.png"));
	components.push_back(new DetailButton(componentWidth, "ui/StandardTower.png"));
	components.push_back(new DetailButton(componentWidth, "ui/RapidFireTower.png"));
	components.push_back(new DetailButton(componentWidth, "ui/CannonTower.png"));

	float currentY = DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

	// Initialize Tower buttons
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->setComponentPosition(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY);
		currentY += DetailDisplayComponent::DETAIL_COMPONENT_SPACING;
	}

	towerObserver = new TowerObserver(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY + DetailDisplayComponent::TOWER_COMPONENT_SPACING);
	critterObserver = new CritterObserver(mPosition.x + DetailDisplayComponent::DETAIL_COMPONENT_PADDING, currentY + DetailDisplayComponent::CRITTER_COMPONENT_SPACING);
}

DetailAttributeDisplay::~DetailAttributeDisplay() {
	// Clean up components
	for (auto component : components)
	{
		delete component;
	}
	components.clear();
	// Clean up observers
	delete towerObserver;
	towerObserver = nullptr;
	delete critterObserver;
	critterObserver = nullptr;
}

/**
 * @brief Accessor for components
 * @return the array of DetailDisplayComponent object pointers
 */
std::vector<DetailDisplayComponent *> DetailAttributeDisplay::getComponents()
{
	return components;
}

/**
 * @brief Accessor for Tower detail components
 * @return the vector of DetailDisplayComponent object pointers for the TowerObserver
 */
std::vector<DetailDisplayComponent *> DetailAttributeDisplay::getTowerComponents()
{
	return towerObserver->getTowerComponents();
}

/**
 * @brief Accessor for the TowerObserver
 * @return the TowerObserver used in the DetailDisplay
 */
TowerObserver *DetailAttributeDisplay::getTowerObserver()
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
	return towerObserver->initializeTowerComponents() && critterObserver->initializeCritterComponents();
}

/**
 * @brief Selects Tower to be shown through the TowerObserver
 * @param tower the Tower to have details displayed in the TowerObserver
 */
void DetailAttributeDisplay::selectTower(Tower *tower)
{
	towerObserver->setCurrentTower(tower);
	critterObserver->setCurrentCritter(nullptr);
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
void DetailAttributeDisplay::handleButtonEvents(SDL_Event &e)
{
	// handles buy Tower buttons
	(dynamic_cast<DetailButton *>(components[1]))->handleEvent(&e);
	(dynamic_cast<DetailButton *>(components[2]))->handleEvent(&e);
	(dynamic_cast<DetailButton *>(components[3]))->handleEvent(&e);

	// handles buttons from TowerObserver
	towerObserver->handleButtonEvents(e);
}

/**
 * @brief Accessor for critter component vector
 * @return the DetailDisplayComponent objects for the Critter UI
 */
std::vector<DetailDisplayComponent *> DetailAttributeDisplay::getCritterComponents()
{
	return critterComponents;
}

/**
 * @brief Accessor for the CritterObserver
 * @return the CritterObserver used in the DetailDisplay
 */
CritterObserver *DetailAttributeDisplay::getCritterObserver()
{
	return critterObserver;
}

/**
 * @brief Selects Critter to be shown through the CritterObserver
 * @param critter the Tower to have details displayed in the CritterObserver
 */
void DetailAttributeDisplay::selectCritter(Critter *critter)
{
	// When a critter is selected, you could set its details to the display components or perform additional logic
	if (critter != nullptr)
	{
		// Clear current components first
		critterComponents.clear();

		// Set the critter observer or handle the critter-related display logic
		if (critterObserver != nullptr)
		{
			critterObserver->setCurrentCritter(critter);
		}
	}
	towerObserver->setCurrentTower(nullptr);
}

/**
 * @brief Checks if Critter details are currently supposed to be displayed
 * @return true if Critter details should be displayed
 * @return false if Critter details should not be displayed, either empty or Tower
 */
bool DetailAttributeDisplay::isDisplayingCritter()
{
	return critterObserver->getCurrentCritter() != nullptr;
}

/**
 * @brief Displays the UI
 */
void DetailAttributeDisplay::render()
{
	DetailDisplay::render();

	// renders the buttons to buy Towers
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->render();
	}

	// displays Tower details through TowerObserver if needed
	if (isDisplayingTower())
	{
		towerObserver->render();
	}

	// displays Tower details through TowerObserver if needed
	if (isDisplayingCritter())
	{
		critterObserver->render();
	}
}

void DetailAttributeDisplay::update()
{
    for (auto* component : components) {
        // Attempt a dynamic cast to DetailButton
        if (DetailButton* button = dynamic_cast<DetailButton*>(component)) {
            button->update();
        }
    }
}

