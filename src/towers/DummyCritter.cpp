#include <Global.h>
#include <towers/DummyCritter.h>

DummyCritter::DummyCritter(float x, float y, int health)
    : x(x), y(y), health(health)
{

}

bool DummyCritter::damageCritter(int damage)
{
    health -= damage;
    return (health <= 0);
}

int DummyCritter::getHealth() 
{
    return health;
}

float DummyCritter::getX()
{
    return x;
}

float DummyCritter::getY()
{
    return y;
}

void DummyCritter::generateCritter()
{
    SDL_FRect fillRect = { x, y, CRITTER_SIZE, CRITTER_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 0x1B, 0x82, 0x4D, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}
