/**
 * @class TargetStrongest
 * @brief Implementation of TargetStrongest class
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details TargetStrongest is a descendent of the TowerStrategy class
 * Part 1 implementation of the Strategy pattern required in Assignment 3
 */

#include <towers/TargetStrongest.h>
#include <towers/Tower.h>
#include <critter/Critter.h>

/**
 * @brief
 * @param critters
 * @return
 */
Critter* TargetStrongest::targetCritter(std::vector<Critter>& critters, Tower& tower)
{
	Critter* currentCritter = nullptr;

	for (int i = 0; i < critters.size(); i++)
	{
		Critter* critter = &critters[i];
		if (tower.isCritterInRange(*critter))
		{
			if (currentCritter != nullptr)
			{
				if (critter->getHitPoints() > currentCritter->getHitPoints())
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