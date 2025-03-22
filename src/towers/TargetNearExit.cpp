/**
 * @class TargetNearExit
 * @brief Implementation of TargetNearExit class
 * @author Denmar Ermitano
 * @date 2025-03-09
 * 
 * @details TargetNearExit is a descendent of the TowerStrategy class
 * Part 1 implementation of the Strategy pattern required in Assignment 3 
 */

#include <towers/TargetNearExit.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

Critter* TargetNearExit::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;

	// iterates through all critters
	for (int i = 0; i < critters.size(); i++)
	{
		Critter* critter = critters[i];
		if (critter->isAlive() && tower.isCritterInRange(critter))
		{
			// replacing currently selected critter
			if (currentCritter != nullptr)
			{
				// checks for distance travelled
				if (critter->getDistanceTravelled() > currentCritter->getDistanceTravelled())
				{
					currentCritter = critter;
				}
			}
			// replaces null pointer with first targetable critter
			else 
			{
				currentCritter = critter;
			}
		}
	}

	return currentCritter;
}