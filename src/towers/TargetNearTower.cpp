/**
 * @class TargetNearTower
 * @brief Implementation of TargetNearTower class
 * @author Denmar Ermitano
 * @date 2025-03-09
 * 
 * @details TargetNearTower is a descendent of the TargetNearTower class
 * Part 1 implementation of the Strategy pattern required in Assignment 3 
 */

#include <towers/TargetNearTower.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

Critter* TargetNearTower::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;
	float currentDistance = -1;

	// iterates through critters
	for (int i = 0; i < critters.size(); i++)
	{
		// checks if critter is in range
		Critter* critter = critters[i];
		if (critter->isAlive() && tower.isCritterInRange(critter))
		{
			
			// checks distance from Tower and replaces if critter is closer
			float newDistance = tower.calcDistance(critter);
			if (newDistance < currentDistance || currentDistance == -1)
			{
				currentCritter = critter;
				currentDistance = newDistance;
			}
		}
	}

	return currentCritter;
}