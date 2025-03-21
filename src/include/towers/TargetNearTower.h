/**
 * @file TargetNearTower.h
 * @brief Strategy for Targetting Critter nearest to Tower
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details Contains targetCritter method to find Critter closest to the Tower point
 * First checks if Critter is within the Tower's range,
 * then compares distance with the currently selected Critter.
 * Swaps if necessary.
 */

#pragma once
#include <critter/Critter.h>
#include <towers/TowerStrategy.h>
#include <towers/Tower.h>
#include <vector>

class TargetNearTower : public TowerStrategy
{
public:
	/**
	 * @brief targets the closest Critter to the Tower for a Tower object.
	 *
	 * @details Finds the closest Critter to the Tower within the range of the Tower object
	 * @param critters vector of Critters to check
	 * @param tower Tower to shoot a Projectile
	 * @return nullptr if no Critter is targettable
	 * @return the pointer of the Critter for the Tower to target
	 */
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};