#pragma once

class Projectile
{
    public:
        Projectile(float x, float y, int damage, bool isArea);
        Projectile(float x, float y, int damage, bool isArea, int projectileSize);
        int getDamage();
        int getIsArea();

        void move(float xSpeed, float ySpeed);
        void generateProjectile();
        bool isOutside();

        bool checkCollision(float critterX, float critterY);

    private:
        float x;
        float y;
        int damage;
        int projectileSize;
        bool isArea;
};
