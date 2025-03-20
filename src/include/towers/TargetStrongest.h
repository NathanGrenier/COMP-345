/**
 * @file TargetStrongest.h
 * @brief Strategy for Targetting strongest Critter 
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

class TargetStrongest : public TowerStrategy
{
public:
	Critter* targetCritter(std::vector<Critter*> critters, Tower& tower);
};