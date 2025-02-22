/**
 * @file DummyCritter.h
 * @brief Class for testing Tower Classes
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details Class of DummyCritters
 * Allows for sub-classes of Tower to be tested
 * Provides a target for those Tower classes, along with a set amount of health points
 * Test for damage and range of Towers
 */

#pragma once

class DummyCritter
{
public:
    static const int CRITTER_SIZE = 15; /** @brief size of dummy critters in pixels */

    DummyCritter(float x, float y, int health);
    bool damageCritter(int damage); /** removes health points from critter */

    int getHealth();
    float getX();
    float getY();

    void generateCritter();

private:
    float x; /** x @brief position for projectile */
    float y; /** y @brief  for projectile */
    int health; /** @brief health points until dummy critter is considered dead */
};
