#pragma once

#include <critter/Critter.h>
#include <map/Map.h>
#include <string>

class CritterFactory {
public:
	static Critter* createCritterByType(std::string type, int waveLevel, SDL_FRect position, Map* map);
};