#ifndef CHARACTER_H
#define CHARACTER_H
#include "Weapon.h"

using namespace std;

class Character
{
    public:
        Character();
    protected:
        int strength;
        int armor;
        int speed;
        int intelligence;
        int level;
        int experience;
        int health;
        Weapon weapon;
};

#endif // CHARACTER_H
