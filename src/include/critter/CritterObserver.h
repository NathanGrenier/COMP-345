#pragma once

#include <vector>
#include <util/Observer.h>
#include <critter/Critter.h>
#include <ui/DetailDisplayComponent.h>
#include <ui/DetailAttributeDisplay.h>
#include <ui/DetailAttribute.h>
#include <ui/DetailLabel.h>
#include <ui/DetailButton.h>
#include <util/Observable.h>
#include <format>
#include <string>

/**
 * @class CritterObserver
 * @brief Observes and displays information about a selected critter.
 */

class CritterObserver : public Observer {
public:
    /**
     * @brief Constructor with starting position for placing DetailDisplayComponent.
     * @param startingX Horizontal starting position.
     * @param startingY Vertical starting position.
     */
    CritterObserver(float startingX, float startingY);

    /**
     * @brief Initializes the DetailDisplayComponent for displaying Critter's information.
     * @return true if initialization was successful.
     * @return false if initialization failed.
     */
    bool initializeCritterComponents();

    /**
     * @brief Renders the components that describe the selected critter.
     */
    void render();

    /**
     * @brief Updates the displayed Critter attributes if observed critter launches notification.
     * @param observable The observable critter.
     */
    void update(Observable* observable) override;

    /**
     * @brief Sets the critter to be observed.
     * @param critter The critter to be observed.
     */
    void setCurrentCritter(Critter* critter);

    /**
     * @brief Accessor for the current critter being observed.
     * @return The critter currently selected for observation.
     */
    Critter* getCurrentCritter();

    /**
     * @brief Handles button events (e.g., sell or upgrade).
     * @param e The SDL event to handle.
     */
    void handleButtonEvents(SDL_Event* e);

    /**
     * @brief Destructor that cleans up the created components.
     */
    ~CritterObserver();

private:
    /**
     * @brief Updates the displayed DetailAttribute values of the selected Critter.
     */
    void updateAttributes();

    /**
     * @brief Formats the string to be displayed for an attribute.
     * @param currentValue Current value of the attribute.
     * @param upgradeValue Upgrade value of the attribute.
     * @param critterAtMaxLevel Whether the critter is at max level.
     * @return Formatted string for the attribute.
     */
    std::string formatValueStr(int currentValue, int upgradeValue, bool critterAtMaxLevel);

    Critter* currentCritter;  ///< The critter currently being observed.
    std::vector<DetailDisplayComponent*> critterComponents;  ///< The components used to display critter information.
};

