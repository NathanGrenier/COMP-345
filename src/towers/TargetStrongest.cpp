/**
 * @class TargetStrongest
 * @brief Implementation of TargetStrongest class
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details TargetStrongest is a descendent of the TowerStrategy class
 * Part 1 implementation of the Strategy pattern required in Assignment 3
 */

#include <towers/TargetStrongest.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

Critter* TargetStrongest::targetCritter(std::vector<Critter*> critters, Tower& tower)
{
	Critter* currentCritter = nullptr;

	// iterates through critters
	for (int i = 0; i < critters.size(); i++)
	{
		Critter* critter = critters[i];

		// checks if critter is in range
		if (tower.isCritterInRange(critter))
		{
			if (currentCritter != nullptr)
			{
				// replaces critter if critter is stronger than previous critter
				if (critter->getHitPoints() > currentCritter->getHitPoints())
				{
					currentCritter = critter;
				}
			}
			// replaces null pointer with critter in range
			else
			{
				currentCritter = critter;
			}
		}
	}

	return currentCritter;
}