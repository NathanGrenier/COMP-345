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
TowerObserver::TowerObserver(float startingX, float startingY) : currentTower(nullptr), towerStrategyIndex(-1) {
	float componentWidth = DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

	// label for Tower information
	towerComponents.push_back(new DetailLabel(componentWidth, "ui/TowerInformation.png"));

	// attribute for Towers
	towerComponents.push_back(new DetailAttribute(componentWidth));
	towerComponents.push_back(new DetailAttribute(componentWidth));
	towerComponents.push_back(new DetailAttribute(componentWidth));
	towerComponents.push_back(new DetailAttribute(componentWidth));
	towerComponents.push_back(new DetailAttribute(componentWidth));
	towerComponents.push_back(new DetailAttribute(componentWidth));

	// buttons to sell and upgrade Towers
	towerComponents.push_back(new DetailButton(componentWidth, "ui/UpgradeTower.png"));
	towerComponents.push_back(new DetailButton(componentWidth, "ui/SellTower.png"));

	// label for TowerStrategy object of the Tower
	towerComponents.push_back(new DetailLabel(componentWidth, "ui/CritterTargetting.png"));

	// array of TowerStrategy paths
	towerStrategyPaths = new std::string[STRATEGY_COUNT];
	towerStrategyPaths[0] = "ui/TargetNearestToExit.png";
	towerStrategyPaths[1] = "ui/TargetNearestToTower.png";
	towerStrategyPaths[2] = "ui/TargetStrongest.png";
	towerStrategyPaths[3] = "ui/TargetWeakest.png";

	towerComponents.push_back(new DetailButton(componentWidth, towerStrategyPaths[0]));

	// adds spacing
	for (int i = 0; i < towerComponents.size(); i++) {
		towerComponents[i]->setComponentPosition(startingX, startingY);
		float additionalSpacing = 0.0f;

		// adds spacing for DetailDisplayComponent: DetailButtons and DetailLabels
		if (i > 6 || i == 0)
		{
			additionalSpacing = DetailDisplayComponent::DETAIL_COMPONENT_SPACING;

			if (i == 8)
			{
				additionalSpacing += DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
			}
		} else // regular spacing for DetailAttributes
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
bool TowerObserver::initializeTowerComponents() {
	SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };

	bool success1 = (dynamic_cast<DetailAttribute*>(towerComponents[1]))->setAttributeText("Range", textColor);
	bool success2 = (dynamic_cast<DetailAttribute*>(towerComponents[2]))->setAttributeText("Power", textColor);
	bool success3 = (dynamic_cast<DetailAttribute*>(towerComponents[3]))->setAttributeText("Rate of fire", textColor);
	bool success4 = (dynamic_cast<DetailAttribute*>(towerComponents[4]))->setAttributeText("Level", textColor);
	bool success5 = (dynamic_cast<DetailAttribute*>(towerComponents[5]))->setAttributeText("Buying cost", textColor);
	bool success6 = (dynamic_cast<DetailAttribute*>(towerComponents[6]))->setAttributeText("Refund value", textColor);

	return (success1 && success2 && success3 && success4 && success5 && success6);
}

/**
 * @brief Accessor for towerComponents vector
 * @return the DetailDisplayComponent that describe the Tower
 */
std::vector<DetailDisplayComponent*> TowerObserver::getTowerComponents() {
	return towerComponents;
}

/**
 * @brief Renders the DetailDisplayComponent to describe a Tower
 * @details Renders upgrade, sell, and strategy button depending on whether a Tower is selected or if a Tower class is selected
 */
void TowerObserver::render() {
	// Omit the upgrade/sell/strategy button if a Tower subclass is selected
	int componentChangeNum = static_cast<int>(towerComponents.size());

	// whether to include buying/selling and changing Critter targetting while buying a Tower
	for (int i = 0; i < buyTowers.size(); i++) {
		if (currentTower == buyTowers[i]) {
			towerStrategyIndex = -1;
			componentChangeNum -= 4;
			break;
		}
	}

	// checking for tower level
	int maxLevel = currentTower->getMaxLevel();
	int currentLevel = currentTower->getLevel();
	bool towerAtMaxLevel = (maxLevel == currentLevel);

	// sets the position for the relevant Tower components, whether you can buy/upgrade or none
	float verticalPosition5 = towerComponents[4]->getComponentYPosition() + DetailAttribute::DETAIL_ATTRIBUTE_SPACING;
	if (towerAtMaxLevel)
	{
		towerComponents[6]->setComponentPosition(towerComponents[6]->getComponentXPosition(), verticalPosition5);
	} else
	{
		towerComponents[5]->setComponentPosition(towerComponents[5]->getComponentXPosition(), verticalPosition5);
		towerComponents[6]->setComponentPosition(towerComponents[6]->getComponentXPosition(), verticalPosition5 + DetailAttribute::DETAIL_ATTRIBUTE_SPACING);
	}

	// Render the DetailDisplayComponent
	for (int i = 0; i < componentChangeNum; i++) {
		if (!(towerAtMaxLevel && i == 5))
		{
			if (i == 10) {
				dynamic_cast<DetailButton*>(towerComponents[i])->render();
			}
			towerComponents[i]->render();
		}
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
		for (float i = x + timeOffset; i < x + w; i += dotSpacing) {
			for (float t = 0; t < dotThickness; t++) {
				SDL_RenderPoint(gRenderer, i, y + t);  // Inward thickness
			}
		}

		// ---- Right Edge (top to bottom for clockwise) ----
		for (float i = y + timeOffset; i < y + h; i += dotSpacing) {
			for (float t = 0; t < dotThickness; t++) {
				SDL_RenderPoint(gRenderer, x + w - 1 - t, i);  // Inward thickness
			}
		}

		// ---- Bottom Edge (right to left for clockwise) ----
		for (float i = x + w - timeOffset; i > x; i -= dotSpacing) {
			for (float t = 0; t < dotThickness; t++) {
				SDL_RenderPoint(gRenderer, i, y + h - 1 - t);  // Inward thickness
			}
		}

		// ---- Left Edge (bottom to top for clockwise) ----
		for (float i = y + h - timeOffset; i > y; i -= dotSpacing) {
			for (float t = 0; t < dotThickness; t++) {
				SDL_RenderPoint(gRenderer, x + t, i);  // Inward thickness
			}
		}
	}
}


/**
 * @brief Desctructor, deleting the DetailDisplayComponents
 */
TowerObserver::~TowerObserver() {
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
void TowerObserver::update(Observable* observable) {
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
void TowerObserver::setCurrentTower(Tower* tower) {
	currentTower = tower;
}

/**
 * @brief Accessor for currentTower
 * @return the Tower that is currently selected
 */
Tower* TowerObserver::getCurrentTower() {
	return currentTower;
}

/**
 * @brief Handles button events for the buy and sell DetailButtons
 * @param e the SDL event to handle
 */
void TowerObserver::handleButtonEvents(SDL_Event& e) {
	(dynamic_cast<DetailButton*>(towerComponents[7]))->handleEvent(&e);
	(dynamic_cast<DetailButton*>(towerComponents[8]))->handleEvent(&e);

	if (currentTower != nullptr) {
		(dynamic_cast<DetailButton*>(towerComponents[7]))->update();
		(dynamic_cast<DetailButton*>(towerComponents[8]))->update();
		(dynamic_cast<DetailButton*>(towerComponents[10]))->update();
	}

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
void TowerObserver::updateAttributes() {
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

	dynamic_cast<DetailAttribute*>(towerComponents[5])->setAttributeText("Upgrade cost", textColor);
	std::string buyUpgradeCostStr;

	// determines whether to show either initial cost or upgrade cost
	if (towerAtMaxLevel)
	{
		buyUpgradeCostStr = "0";
	} else
	{
		buyUpgradeCostStr = std::to_string(currentTower->getUpgradeCost());

		for (int i = 0; i < buyTowers.size(); i++) {
			if (currentTower == buyTowers[i]) {
				dynamic_cast<DetailAttribute*>(towerComponents[5])->setAttributeText("Buying cost", textColor);
				buyUpgradeCostStr = std::to_string(currentTower->getBuyingCost());
				break;
			}
		}
	}

	// sets the text for the DetailAttribute values
	(dynamic_cast<DetailAttribute*>(towerComponents[1]))->setValueText(rangeStr, textColor);
	(dynamic_cast<DetailAttribute*>(towerComponents[2]))->setValueText(powerStr, textColor);
	(dynamic_cast<DetailAttribute*>(towerComponents[3]))->setValueText(rateOfFireStr, textColor);
	(dynamic_cast<DetailAttribute*>(towerComponents[4]))->setValueText(levelStr, textColor);
	(dynamic_cast<DetailAttribute*>(towerComponents[5]))->setValueText(buyUpgradeCostStr, textColor);
	(dynamic_cast<DetailAttribute*>(towerComponents[6]))->setValueText(refundValueStr, textColor);
}

/**
 * @brief Updates the TowerStrategy button to match the one with the currently selected Tower
 */
void TowerObserver::updateStrategyButton() {
	towerStrategyIndex = TowerGroup::getStrategyIndex(currentTower);

	try {
		DetailButton* oldButton = dynamic_cast<DetailButton*>(towerComponents[10]);

		float xPosition = oldButton->getXPos();
		float yPosition = oldButton->getYPos();

		if (oldButton) {
			delete oldButton;
		}

		float componentWidth = DetailAttributeDisplay::ATTRIBUTE_DISPLAY_WIDTH - 2 * DetailDisplayComponent::DETAIL_COMPONENT_PADDING;

		DetailButton* newButton = new DetailButton(componentWidth, towerStrategyPaths[towerStrategyIndex]);
		newButton->setComponentPosition(xPosition, yPosition);

		towerComponents[10] = newButton;
	}
	catch (const std::bad_cast&) {
		std::cerr << "Error: towerComponents[10] is not a DetailButton!" << std::endl;
	}
}


/**
 * @brief Formats string to be displayed in DetailAttribute
 * @param currentValue current value of attribute
 * @param upgradeValue upgrade value of attribute
 * @param towerAtMaxLevel if Tower is at highest level
 * @return formatted string to represent the value of a DetailAttribute
 */
std::string TowerObserver::formatValueStr(int currentValue, int upgradeValue, bool towerAtMaxLevel) {
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
void TowerObserver::addToBuyTowers(Tower* tower) {
	buyTowers.push_back(tower);
}
