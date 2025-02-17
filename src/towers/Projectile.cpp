#include <Global.h>
#include <towers/Projectile.h>
#include <iostream>
#include <towers/DummyCritter.h>

Projectile::Projectile(float x, float y, int damage, bool isArea)
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(3)
{

}

Projectile::Projectile(float x, float y, int damage, bool isArea, int projectileSize) 
    : x(x), y(y), damage(damage), isArea(isArea), projectileSize(projectileSize)
{
    
}

int Projectile::getDamage() 
{
    return damage;
}

void Projectile::move(float xSpeed, float ySpeed)
{
    x += xSpeed;
    y += ySpeed;
}

void Projectile::generateProjectile() 
{
    SDL_FRect fillRect = { x, y, projectileSize, projectileSize };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

bool Projectile::isOutside()
{
    return ((x < 0 || x > Global::kScreenWidth) || (y < 0 || y > Global::kScreenHeight));
}

bool Projectile::checkCollision(float critterX, float critterY) {
    //float tolerance = -4.0f;
    float tolerance = 0.0f;
    
    return 
        x < critterX + DummyCritter::CRITTER_SIZE + tolerance &&
        x + projectileSize > critterX - tolerance &&
        y < critterY + DummyCritter::CRITTER_SIZE + tolerance &&
        y + projectileSize > critterY - tolerance;
}

