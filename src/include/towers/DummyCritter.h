#pragma once

class DummyCritter
{
public:
    static const int CRITTER_SIZE = 15;

    DummyCritter(float x, float y, int health);
    bool damageCritter(int damage);

    int getHealth();
    float getX();
    float getY();

    void generateCritter();

private:
    float x;
    float y;
    int health;
};
