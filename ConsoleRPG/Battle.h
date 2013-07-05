#ifndef BATTLE_H
#define BATTLE_H
#include "Hero.h"
#include "Monster.h"
#include <string>
#include <cstdlib>

using namespace std;

class Battle
{
    public:
        Battle(Hero&, int, int);
        bool initiate();
        int getPrize();
    protected:
    private:
        Hero *hero;
        Weapon *spell;
        Monster monster;
        int option;
        int timeCount;
        int onWaitLeft;
        int onDmgLeft;
        int dmg;
        int hMod;
        int mMod;
        bool fight();
        bool heroAttack();
        bool monsterAttack();
        void spellDamage(int);
        void spellOngoing(int, int);
        bool spellWait(int);
        void spellDrain(int);
        void spellHealth(int);
        void spellMana(int);
};

#endif // BATTLE_H
