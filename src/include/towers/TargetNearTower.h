/**
 * @file TargetNearTower.h
 * @brief Strategy for Targetting Critter nearest to Tower
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

class TargetNearTower : public TowerStrategy
{
public:
	Critter* targetCritter(std::vector<Critter>& critters, Tower& tower);
};