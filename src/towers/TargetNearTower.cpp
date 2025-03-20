/**
 * @class TargetNearTower
 * @brief Implementation of TargetNearTower class
 * @author Denmar Ermitano
 * @date 2025-02-22
 * 
 * @details TargetNearTower is a descendent of the TargetNearTower class
 * Part 1 implementation of the Strategy pattern required in Assignment 3 
 */

#include <towers/TargetNearTower.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

/**
 * @brief 
 * @param critters 
 * @return 
 */
Critter* TargetNearTower::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;
	float currentDistance = -1;

	for (int i = 0; i < critters.size(); i++)
	{
		Critter* critter = critters[i];
		if (tower.isCritterInRange(critter))
		{
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