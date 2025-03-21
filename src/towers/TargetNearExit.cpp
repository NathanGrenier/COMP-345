/**
 * @class TargetNearExit
 * @brief Implementation of TargetNearExit class
 * @author Denmar Ermitano
 * @date 2025-02-22
 * 
 * @details TargetNearExit is a descendent of the TowerStrategy class
 * Part 1 implementation of the Strategy pattern required in Assignment 3 
 */

#include <towers/TargetNearExit.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

/**
 * @brief 
 * @param critters 
 * @return 
 */
Critter* TargetNearExit::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;

	for (int i = 0; i < critters.size(); i++)
	{
		Critter* critter = critters[i];
		if (tower.isCritterInRange(critter))
		{
			if (currentCritter != nullptr)
			{
				if (critter->getDistanceTravelled() > currentCritter->getDistanceTravelled())
				{
					currentCritter = critter;
				}
			}
			else 
			{
				currentCritter = critter;
			}
		}
	}

	return currentCritter;
}