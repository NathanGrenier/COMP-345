/**
 * @file TowerStrategy.h
 * @brief Parent class for tower strategies
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details Parent class of concrete TowerStrategies.
 */

#pragma once
#include <critter/Critter.h>
#include <vector>

// forward declaration for Tower
class Tower;

class TowerStrategy 
{
public:
	/**
	 * @brief targets a Critter within a Tower object's range.
	 *
	 * @param critters vector of Critters to check
	 * @param tower Tower to shoot a Projectile
	 * @return nullptr if no Critter is targettable
	 * @return the pointer of the Critter for the Tower to target
	 */
	virtual Critter* targetCritter(std::vector<Critter*> critters, Tower& tower) = 0;
};