#pragma once

#include <towers/Tower.h>

class TowerDecorator : public Tower {
public:
    explicit TowerDecorator(Tower* tower, SDL_FRect towerPosition, std::string indicatorPath) : towerPosition(towerPosition), wrappedTower(tower) {
        indicatorTexture.loadFromFile(indicatorPath);
    }

    ~TowerDecorator() {
        indicatorTexture.destroy();
    }

    std::vector<Projectile*>& getProjectiles() override {
        return wrappedTower->getProjectiles();
    }

    Critter* findCritter(std::vector<Critter*> critters) override {
        return wrappedTower->findCritter(critters);
    }

    void generateAllProjectiles() override {
        wrappedTower->generateAllProjectiles();
    }

    int getRange() const override {
        return wrappedTower->getRange();
    }

    int getPower() const override {
        return wrappedTower->getPower();
    }

    int getRateOfFire() const override {
        return wrappedTower->getRateOfFire();
    }

    int getLevel() const override {
        return wrappedTower->getLevel();
    }

    int getRefundValue() override {
        return wrappedTower->getRefundValue();
    }

    int getUpgradeCost() override {
        return wrappedTower->getUpgradeCost();
    }

    int getMaxLevel() override {
        return wrappedTower->getMaxLevel();
    }

    bool upgrade() override {
        return wrappedTower->upgrade();
    }

    void shootProjectile(Critter* critter) override {
        wrappedTower->shootProjectile(critter);
    }

    void clearProjectiles() override {
        wrappedTower->clearProjectiles();
    }

    bool isClicked(float scaleFactor) const override {
        return wrappedTower->isClicked(scaleFactor);
    }

    bool isCritterInRange(Critter* critter) override {
        return wrappedTower->isCritterInRange(critter);
    }

    UpgradeValues getUpgradeValues() const override {
        return wrappedTower->getUpgradeValues();
    }

    void render() override {
        wrappedTower->render();
    }

    void setRotation(float angle) override {
        wrappedTower->setRotation(angle);
    }

    float getRotation() const override {
        return wrappedTower->getRotation();
    }

    void setCurrentRenderRect(float originalX, float originalY, float w, float h) override {
        wrappedTower->setCurrentRenderRect(originalX, originalY, w, h);
    }

    SDL_FRect getCurrentRenderRect() const override {
        return wrappedTower->getCurrentRenderRect();
    }

    LTexture& getTowerTexture() override {
        return wrappedTower->getTowerTexture();
    }

    int& getShootingTimer() override {
        return wrappedTower->getShootingTimer();
    }

    void setShootingTimer(int newShootingTimer) override {
        wrappedTower->setShootingTimer(newShootingTimer);
    }

    Tower* getWrappedTower() {
        return wrappedTower;
    }

protected:
    Tower* wrappedTower;
    SDL_FRect towerPosition;
    LTexture indicatorTexture;
};
