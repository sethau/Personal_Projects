#ifndef STORE_H
#define STORE_H
#include <vector>
#include "Weapon.h"

using namespace std;

class Store
{
    public:
        Store();
        void displayInventory();
        Weapon purchaseItem(int, int);
        Weapon getItem(string);
        int sellItem();
    protected:
    private:
        int option;
        vector<Weapon> weapons;
        vector<Weapon> potions;
        vector<Weapon> spells;
};

#endif // STORE_H
