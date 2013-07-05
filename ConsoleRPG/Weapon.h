#ifndef WEAPON_H
#define WEAPON_H
#include <string>
#include <iostream>

using namespace std;

class Weapon
{
    public:
        Weapon();
        Weapon(string, int, int[]);
        void printInfo();
        int getCost();
        int getManaCost();
        int getStrength();
        int getOnDmgTm();
        int getOnDmg();
        int getWait();
        int getDrain();
        int getHealth();
        int getMana();
        string getName();
        int getType();
    protected:
    private:
        string name;
        int type;
        bool magic;
        int attributes[9];//{gold cost, mana cost, strength, ngng dmg tm, ngng dmg, #turns wait, health drain, health regain, mana regain}
};

#endif // WEAPON_H
