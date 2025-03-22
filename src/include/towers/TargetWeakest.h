/**
 * @file TargetWeakest.h
 * @brief Strategy for Targetting weakest Critter
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details Contains targetCritter method to find weakest Critter
 * First checks if Critter is within the Tower's range,
 * then compares weakness with the currently selected Critter.
 * Swaps if necessary.
 * Remaining hit points is used to check and compare weakness for a Critter.
 */

#pragma once
#include <critter/Critter.h>
#include <towers/TowerStrategy.h>
#include <towers/Tower.h>
#include <vector>

class TargetWeakest : public TowerStrategy
{
public:
	/**
	 * @brief targets the weakest Critter for a Tower object.
	 *
	 * @details Finds the weakest Critter within the range of the Tower object
	 * @param critters vector of Critters to check
	 * @param tower Tower to shoot a Projectile
	 * @return nullptr if no Critter is targettable
	 * @return the pointer of the Critter for the Tower to target
	 */
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};