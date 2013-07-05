#ifndef MONSTER_H
#define MONSTER_H
#include "Character.h"

using namespace std;

class Monster : public Character
{
    public:
        Monster();
        Monster(int, int);
        int getSpeed();
        int getStrength();
        int getAttackBonus();
        int getArmor();
        int getLevel();
        int getHealth();
        void modifyHealth(int);
        int getPrize();
        bool isDead();
    protected:
    private:
        int level;
        int health;
        int strength;
        int speed;
        int armor;
};

#endif // MONSTER_H
