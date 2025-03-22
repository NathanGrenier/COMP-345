#include <critter/CritterFactory.h>
#include <critter/NormalCritter.h>
#include <critter/FastCritter.h>
#include <critter/TankCritter.h>
#include <cstdlib>

Critter* CritterFactory::createCritterByType(std::string type, int waveLevel, SDL_FRect position, Map* map) {
	waveLevel -= 1;

	if (type == "Normal") {
		return new NormalCritter(waveLevel, position, map);
	} else if (type == "Fast") {
		return new FastCritter(waveLevel, position, map);
	} else if (type == "Tank") {
		return new TankCritter(waveLevel, position, map);
	} else {
		std::cerr << "Unknown critter type: " << type << ". Defaulting to NormalCritter." << std::endl;
		return new NormalCritter(waveLevel, position, map);
	}
}