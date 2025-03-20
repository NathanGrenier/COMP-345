/**
 * @file Critter.h
 * @brief The header file for the Critter class.
 * @author Nirav Patel
 * @date 2025-02-21
 *
 * @details This file contains the declaration of the Critter class, which represents
 *          the enemies in the tower defense game. Critters follow a flow field path
 *          towards an exit and can be damaged by towers. They also have health, speed,
 *          strength, and a reward value upon death.
 */

#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <map/Map.h>
#include <util/Observable.h>
#include <ui/LTexture.h>

 /**
  * @class Critter
  * @brief Represents an enemy in the tower defense game.
  *
  * The Critter class models an enemy unit that moves across the map, follows a path to
  * the exit, takes damage from towers, and steals gold when it reaches the exit. The
  * class also handles rendering and updating the critter's position, health, and other
  * properties.
  */
class Critter : public Observable, public FlowFieldObserver {
public:
    /**
     * @brief Constructs a Critter with specified properties.
     *
     * Initializes the critter with given level, speed, hit points, strength, reward, and
     * initial position.
     *
     * @param level The level of the critter.
     * @param speed The movement speed of the critter.
     * @param hitPoints The hit points (health) of the critter.
     * @param strength The strength of the critter (damage dealt).
     * @param reward The reward given when the critter is killed.
     * @param start The starting position of the critter.
     * @param map The map on which the critter moves.
     */
    Critter(int level, float speed, float hitPoints, int strength, int reward, SDL_FRect start, Map* map);

    /**
     * @brief Destructor for Critter.
     *
     * Cleans up any resources associated with the critter.
     */
    ~Critter();

    static constexpr float CRITTER_WIDTH_SCALE = 1.00f; /**< Width scale for the critter */
    static constexpr float CRITTER_HEIGHT_SCALE = 1.00f; /**< Height scale for the critter */

    static constexpr float CRITTER_HEALTHBAR_HEIGHT = CRITTER_HEIGHT_SCALE * 0.3f; /**< Height of the health bar */
    static constexpr float CRITTER_HEALTHBAR_PADDING = CRITTER_HEIGHT_SCALE * 0.5f; /**< Padding for the health bar */

    /**
     * @brief Called when the flow field has changed.
     *
     * Updates the critter's movement based on changes to the flow field.
     */
    void onFlowFieldChanged() override;

    /**
     * @brief Moves the critter towards the exit.
     *
     * Updates the critter's position based on its speed and the current flow field,
     * ensuring it moves towards its target.
     *
     * @param deltaTime The time difference between frames (for smooth movement).
     * @param critters The list of all critters on the map (for collision detection).
     * @param spacing The distance between critters to prevent overlap.
     */
    void move(float deltaTime, const std::vector<Critter*> critters, float spacing);

    /**
     * @brief Applies damage to the critter.
     *
     * Reduces the critter's health based on the damage taken.
     *
     * @param damage The amount of damage to apply to the critter.
     */
    void takeDamage(float damage);

    /**
     * @brief Checks if the critter is alive.
     *
     * @return true if the critter's hit points are greater than 0, false otherwise.
     */
    bool isAlive() const;

    /**
     * @brief Steals gold from the player if the critter reaches the exit.
     *
     * @param playerGold The current amount of gold the player has, which will be reduced.
     */
    void stealGold(int& playerGold) const;

    /**
     * @brief Renders the critter.
     *
     * Draws the critter to the screen using its texture or a basic shape.
     */
    void render();

    /**
     * @brief Updates the critter's state.
     *
     * This function updates any properties of the critter, including health and damage states.
     */
    void update();

    /**
     * @brief Gets the critter's speed.
     *
     * @return The critter's speed.
     */
    float getSpeed() const;

    /**
     * @brief Sets the critter's speed.
     *
     * @param speed The new speed value for the critter.
     */
    void setSpeed(float speed);

    /**
     * @brief Gets the critter's reward.
     *
     * @return The reward given to the player when the critter is killed.
     */
    int getReward() const { return reward; }

    /**
     * @brief Gets the critter's strength.
     *
     * @return The strength of the critter.
     */
    int getStrength() const { return strength; }

    /**
     * @brief Gets the critter's hit points.
     *
     * @return The critter's current hit points.
     */
    float getHitPoints() const { return hitPoints; }

    /**
     * @brief Sets the critter's hit points.
     *
     * @param hitPoints The new value for the critter's hit points.
     */
    void setHitPoints(float hitPoints);

    /**
     * @brief Checks if the critter has been clicked.
     *
     * @return true if the critter is clicked, false otherwise.
     */
    bool isClicked() const;

    /**
     * @brief Checks if the critter is currently damaged.
     *
     * @return true if the critter is hurt, false otherwise.
     */
    bool isDamaged() const { return isHurt; };

    /**
     * @brief Gets the critter's position.
     *
     * @return The position of the critter.
     */
    SDL_FRect getPosition() const;

    /**
     * @brief Gets the critter's current render rectangle.
     *
     * @return The render rectangle of the critter.
     */
    SDL_FRect getCurrentRenderRect() const { return currentRenderRect; };

    /**
     * @brief Checks if the critter has reached the exit.
     *
     * @return true if the critter has reached the exit, false otherwise.
     */
    bool atExit() const;

    /**
     * @brief Sets whether the critter has reached the exit.
     *
     * @param con true if the critter has reached the exit, false otherwise.
     */
    void setAtExit(bool con);

private:
    int level; /**< The level of the critter */
    float speed; /**< The speed at which the critter moves */
    float hitPoints; /**< The current health of the critter */
    int strength; /**< The damage dealt by the critter */
    int reward; /**< The reward given when the critter is killed */
    SDL_FRect position; /**< The position of the critter */
    bool isAtExit; /**< Whether the critter has reached the exit */
    int maxHitPoints; /**< The maximum health of the critter */
    Map* map; /**< The map on which the critter moves */
    SDL_FRect currentRenderRect; /**< The rectangle for rendering the critter */
    LTexture critterTexture; /**< The texture used to render the critter */

    bool isHurt = false; /**< Whether the critter is currently hurt */
    Uint64 damageTimer = 0; /**< Timer to control damage flashing effect */
    float redTintAlpha = 0.0f; /**< Alpha value for red tint effect */
    float greenTintAlpha = 0.0f; /**< Alpha value for green tint effect */
    float blueTintAlpha = 0.0f; /**< Alpha value for blue tint effect */
    const float maxRedAlpha = 255.0f; /**< Maximum alpha value for red tint */
    const Uint32 damageDuration = 100; /**< Duration of the damage effect in milliseconds */

    int targetCellX; /**< The X coordinate of the target cell in the flow field */
    int targetCellY; /**< The Y coordinate of the target cell in the flow field */
    SDL_FPoint targetPos; /**< The pixel position of the target (center of the target cell) */
};
