/**
 * @class TargetWeakest
 * @brief Implementation of TargetWeakest class
 * @author Denmar Ermitano
 * @date 2025-03-09
 * 
 * @details TargetWeakest is a descendent of the TargetWeakest class
 * Part 1 implementation of the Strategy pattern required in Assignment 3 
 */

#include <towers/TargetWeakest.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

Critter* TargetWeakest::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;

	// iterates through critters
	for (int i = 0; i < critters.size(); i++)
	{

		// checks if critter is within range
		Critter* critter = critters[i];
		if (critter->isAlive() && tower.isCritterInRange(critter))
		{
			if (currentCritter != nullptr)
			{
				// replaces critter if critter is weaker
				if (critter->getHitPoints() < currentCritter->getHitPoints())
				{
					currentCritter = critter;
				}
			}
			// replaces null pointer if critter is in range
			else
			{
				currentCritter = critter;
			}
		}
	}

	return currentCritter;
}