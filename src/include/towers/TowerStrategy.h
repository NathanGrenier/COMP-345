/**
 * @file TowerStrategy.h
 * @brief Parent class for tower strategies
 * @author Denmar Ermitano
 * @date 2025-03-09
 *
 * @details 
 */

#pragma once
#include <critter/Critter.h>
#include <vector>

class Tower;

class TowerStrategy 
{
public:
	virtual Critter* targetCritter(std::vector<Critter*> critters, Tower& tower) = 0;
};