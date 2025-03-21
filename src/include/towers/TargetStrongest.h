/**
 * @file TargetStrongest.h
 * @brief Strategy for Targetting strongest Critter 
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details Contains targetCritter method to find strongest Critter
 * First checks if Critter is within the Tower's range,
 * then compares strongest with the currently selected Critter.
 * Swaps if necessary.
 * Remaining hit points is used to check and compare strength for a Critter.
 * Not to confuse with strength member variable of the Critter class.
 */

#pragma once
#include <critter/Critter.h>
#include <towers/TowerStrategy.h>
#include <towers/Tower.h>
#include <vector>

class TargetStrongest : public TowerStrategy
{
public:
	/**
	 * @brief targets the strongest Critter for a Tower object.
	 *
	 * @details Finds the strongest Critter within the range of the Tower object
	 * @param critters vector of Critters to check
	 * @param tower Tower to shoot a Projectile
	 * @return nullptr if no Critter is targettable
	 * @return the pointer of the Critter for the Tower to target
	 */
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};