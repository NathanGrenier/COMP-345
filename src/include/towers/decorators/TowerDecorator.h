#pragma once

#include <towers/Tower.h>

/// @brief The TowerDecorator class is a decorator pattern that wraps a Tower object and allows for additional functionality.
class TowerDecorator : public Tower {
public:
    /**
     * @brief Constructs a TowerDecorator that wraps an existing Tower object.
     * @param tower The Tower object to be wrapped.
     * @param towerPosition The position of the tower in the game world.
     * @param indicatorPath The path to the indicator texture file.
     */
    explicit TowerDecorator(Tower* tower, SDL_FRect towerPosition, std::string indicatorPath)
        : towerPosition(towerPosition), wrappedTower(tower) {
        indicatorTexture.loadFromFile(indicatorPath);
    }

    /// @brief Destructor that destroys the indicator texture.
    ~TowerDecorator() {
        indicatorTexture.destroy();
    }

    /**
     * @brief Retrieves the projectiles fired by the wrapped Tower.
     * @return A reference to the vector of projectiles.
     */
    std::vector<Projectile*>& getProjectiles() override {
        return wrappedTower->getProjectiles();
    }

    /**
     * @brief Finds the first critter in the list that the wrapped Tower can target.
     * @param critters The list of critters to search through.
     * @return The critter that is targeted by the tower.
     */
    Critter* findCritter(std::vector<Critter*> critters) override {
        return wrappedTower->findCritter(critters);
    }

    /// @brief Generates all projectiles for the wrapped Tower.
    void generateAllProjectiles() override {
        wrappedTower->generateAllProjectiles();
    }

    /**
     * @brief Retrieves the range of the wrapped Tower.
     * @return The range of the tower.
     */
    int getRange() const override {
        return wrappedTower->getRange();
    }

    /**
     * @brief Retrieves the power of the wrapped Tower.
     * @return The power of the tower.
     */
    int getPower() const override {
        return wrappedTower->getPower();
    }

    /**
     * @brief Retrieves the rate of fire of the wrapped Tower.
     * @return The rate of fire of the tower.
     */
    int getRateOfFire() const override {
        return wrappedTower->getRateOfFire();
    }

    /**
     * @brief Retrieves the level of the wrapped Tower.
     * @return The level of the tower.
     */
    int getLevel() const override {
        return wrappedTower->getLevel();
    }

    /**
     * @brief Retrieves the refund value of the wrapped Tower.
     * @return The refund value of the tower.
     */
    int getRefundValue() override {
        return wrappedTower->getRefundValue();
    }

    /**
     * @brief Retrieves the upgrade cost of the wrapped Tower.
     * @return The upgrade cost of the tower.
     */
    int getUpgradeCost() override {
        return wrappedTower->getUpgradeCost();
    }

    /**
     * @brief Retrieves the maximum level of the wrapped Tower.
     * @return The maximum level of the tower.
     */
    int getMaxLevel() override {
        return wrappedTower->getMaxLevel();
    }

    /**
     * @brief Upgrades the wrapped Tower if possible.
     * @return true if the upgrade was successful, false otherwise.
     */
    bool upgrade() override {
        return wrappedTower->upgrade();
    }

    /**
     * @brief Makes the wrapped Tower shoot a projectile at the given critter.
     * @param critter The critter to shoot the projectile at.
     */
    void shootProjectile(Critter* critter) override {
        wrappedTower->shootProjectile(critter);
    }

    /// @brief Clears all projectiles fired by the wrapped Tower.
    void clearProjectiles() override {
        wrappedTower->clearProjectiles();
    }

    /**
     * @brief Checks if the wrapped Tower was clicked.
     * @param scaleFactor The scale factor to adjust for screen resolution.
     * @return true if the tower was clicked, false otherwise.
     */
    bool isClicked(float scaleFactor) const override {
        return wrappedTower->isClicked(scaleFactor);
    }

    /**
     * @brief Checks if a critter is in range of the wrapped Tower.
     * @param critter The critter to check for range.
     * @return true if the critter is within range, false otherwise.
     */
    bool isCritterInRange(Critter* critter) override {
        return wrappedTower->isCritterInRange(critter);
    }

    /**
     * @brief Retrieves the upgrade values of the wrapped Tower.
     * @return The upgrade values of the tower.
     */
    UpgradeValues getUpgradeValues() const override {
        return wrappedTower->getUpgradeValues();
    }

    /// @brief Renders the wrapped Tower.
    void render() override {
        wrappedTower->render();
    }

    /**
     * @brief Sets the rotation of the wrapped Tower.
     * @param angle The angle to rotate the tower.
     */
    void setRotation(float angle) override {
        wrappedTower->setRotation(angle);
    }

    /**
     * @brief Retrieves the rotation of the wrapped Tower.
     * @return The current rotation of the tower.
     */
    float getRotation() const override {
        return wrappedTower->getRotation();
    }

    /**
     * @brief Sets the current render rectangle for the wrapped Tower.
     * @param originalX The X-coordinate of the original render position.
     * @param originalY The Y-coordinate of the original render position.
     * @param w The width of the render rectangle.
     * @param h The height of the render rectangle.
     */
    void setCurrentRenderRect(float originalX, float originalY, float w, float h) override {
        wrappedTower->setCurrentRenderRect(originalX, originalY, w, h);
    }

    /**
     * @brief Retrieves the current render rectangle of the wrapped Tower.
     * @return The current render rectangle of the tower.
     */
    SDL_FRect getCurrentRenderRect() const override {
        return wrappedTower->getCurrentRenderRect();
    }

    /**
     * @brief Retrieves the texture of the wrapped Tower.
     * @return A reference to the tower's texture.
     */
    LTexture& getTowerTexture() override {
        return wrappedTower->getTowerTexture();
    }

    /**
     * @brief Retrieves the shooting timer of the wrapped Tower.
     * @return A reference to the shooting timer.
     */
    int& getShootingTimer() override {
        return wrappedTower->getShootingTimer();
    }

    /**
     * @brief Sets the shooting timer of the wrapped Tower.
     * @param newShootingTimer The new value for the shooting timer.
     */
    void setShootingTimer(int newShootingTimer) override {
        wrappedTower->setShootingTimer(newShootingTimer);
    }

    /**
     * @brief Retrieves the wrapped Tower object.
     * @return A pointer to the wrapped Tower.
     */
    Tower* getWrappedTower() {
        return wrappedTower;
    }

protected:
    Tower* wrappedTower; ///< The wrapped Tower object.
    SDL_FRect towerPosition; ///< The position of the tower in the game world.
    LTexture indicatorTexture; ///< The texture for the tower's indicator.
};
