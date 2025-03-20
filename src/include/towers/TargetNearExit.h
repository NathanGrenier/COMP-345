/**
 * @file TargetNearExit.h
 * @brief Strategy for Targetting Critter closest to the exit point
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details
 */

#pragma once
#include <critter/Critter.h>
#include <towers/TowerStrategy.h>
#include <towers/Tower.h>
#include <vector>

class TargetNearExit : public TowerStrategy
{
public:
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};