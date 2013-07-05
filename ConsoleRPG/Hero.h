#ifndef HERO_H
#define HERO_H
#include "Character.h"
#include "Weapon.h"
#include <string>
#include <vector>
#include <fstream>
#include "math.h"

using namespace std;

class Hero : public Character
{
    public:
        Hero();
        Hero(const int[]);
        void setName(string);
        string getName();
        int getStrength();
        int getArmor();
        int getSpeed();
        int getIntelligence();
        int getHealth();
        int getMana();
        int getAttackBonus();
        void addItem(Weapon);
        int getGold();
        bool displayInventory(int);
        void modifyHealth(int);
        void modifyMana(int);
        void restoreHealth();
        void restoreMana();
        bool isDead();
        void viewStats();
        void modifyGold(int);
        void modifyExperience(int);
        bool fullHealth();
        bool fullMana();
        void levelUp();
        int getLevel();
        int getXP();
        void save();
        bool load(string);
        int calculateMaxHealth();
        int calculateMaxMana();
        int calculateNextLevel();
        vector<string> getItems();
        string getWeaponName();
        int getWeaponCost();
        int getWeaponManaCost();
        int getWeaponStrength();
        int getWeaponOnDmgTm();
        int getWeaponOnDmg();
        int getWeaponWait();
        int getWeaponDrain();
        int getWeaponHealth();
        int getWeaponMana();
        void restoreWeapon();
    private:
        int gold;
        int nextLevel;
        int maxHealth;
        int maxMana;
        string name;
        vector<Weapon> inventory;
        vector<string> loadItems;
        Weapon weapon;
        Weapon tempWeapon;
        int mana;
        void equip(int);
};

#endif // HERO_H
