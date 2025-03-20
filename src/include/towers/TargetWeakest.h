/**
 * @file TargetWeakest.h
 * @brief Strategy for Targetting weakest Critter
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

class TargetWeakest : public TowerStrategy
{
public:
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};