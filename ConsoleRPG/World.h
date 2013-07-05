#ifndef WORLD_H
#define WORLD_H
#include "Hero.h"
#include "Store.h"
#include "Battle.h"
#include <iostream>
#include <cstdlib>

using namespace std;

class World
{
    public:
        World();
    protected:
    private:
        Hero hero;
        Store store;
        void chooseStats();
        void displayMenu();
        void viewStats();
        void viewInventory();
        void visitStore();
        void randomEncounter();
        void saveGame();
        void loadGame();
        void quitGame();
};

#endif // WORLD_H
