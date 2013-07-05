#include "World.h"

const int SHOW_ALL = 0;
const int SHOW_SELL = 0;
const int GOLD_MOD = 2;
const int NUM_MONSTER_TYPES = 3;

const int STRONG[] = {8, 6, 3, 3};
const int FAST[] = {3, 6, 8, 3};
const int INTELLIGENT[] = {2, 6, 4, 7};
const int BALANCED[] = {6, 6, 3, 5};

World::World()
{
    chooseStats();
}

void World::chooseStats()
{
    string statPrompt, namePrompt, name, dummy, sOpt,
        inval;
    char option;
    bool valid = false, load = false;


    statPrompt = "Choose your character:";
    statPrompt += "\r\n1) STRONG";
    statPrompt += "\r\n2) FAST";
    statPrompt += "\r\n3) INTELLIGENT";
    statPrompt += "\r\n4) BALANCED";
    statPrompt += "\r\n5) Load Saved Character";
    statPrompt += "\r\n\n>>";

    namePrompt = "\r\n\nWhat shall your name be?";
    namePrompt += "\r\n>>";

    inval = "\r\n\n\t\tPlease enter an integer 0<x<5\r\n\n";

    while (!valid)
    {
        cout << statPrompt;
        cin >> sOpt;
        //clear rest of line, if more than one word was entered
        getline(cin, dummy);

        //check for potentially invalid input
        if (sOpt.length() == 1 && isdigit(sOpt.at(0)))
        {
            option = sOpt.at(0);

            switch (option)
            {
                case '1':
                    hero = Hero(STRONG);
                    valid = true;
                    break;
                case '2':
                    hero = Hero(FAST);
                    valid = true;
                    break;
                case '3':
                    hero = Hero(INTELLIGENT);
                    valid = true;
                    break;
                case '4':
                    hero = Hero(BALANCED);
                    valid = true;
                    break;
                case '5':
                    loadGame();
                    valid = true;
                    load = true;
                    break;
                default:
                    cout << inval;
                    break;
            }
        }
        else
        {
            cout << inval;
        }
    }

    if (!load)
    {
        cout << namePrompt;
        getline(cin, name);
        hero.setName(name);

        cout << "\r\nWelcome, " + name + "!";

        //show main menu
        displayMenu();
    }
}

void World::displayMenu()
{
    string menuOptions, prompt, inval, sOpt, dummy;
    char option;
    bool valid = false;

    menuOptions = "\r\n\n";
    menuOptions += "1) View Stats\r\n";
    menuOptions += "2) View Inventory\r\n";
    menuOptions += "3) Visit store\r\n";
    menuOptions += "4) Random Encounter\r\n";
    menuOptions += "5) Load Game\r\n";
    menuOptions += "6) Save Game\r\n";
    menuOptions += "7) Quit Game";

    prompt = "\r\n\n\t\tPlease choose an option: ";

    inval = "\r\n\n\t\tPlease enter an integer 0<x<6\r\n\n";

    if (hero.isDead())
    {
        quitGame();
    }

    //prompt until a valid option is selected
    while (!valid)
    {
        cout << menuOptions;
        cout << prompt;
        cin >> sOpt;
        //clear rest of line, if more than one word was entered
        getline(cin, dummy);

        //check for potentially invalid input
        if (sOpt.length() == 1 && isdigit(sOpt.at(0)))
        {
            option = sOpt.at(0);
            switch (option)
            {
                case '1':
                    viewStats();
                    valid = true;
                    break;
                case '2':
                    viewInventory();
                    valid = true;
                    break;
                case '3':
                    visitStore();
                    valid = true;
                    break;
                case '4':
                    randomEncounter();
                    valid = true;
                    break;
                case '5':
                    loadGame();
                    valid = true;
                    break;
                case '6':
                    saveGame();
                    valid = true;
                    break;
                case '7':
                    quitGame();
                    valid = true;
                    break;
                default:
                    cout << inval;
                    break;
            }
        }
    }
}

void World::viewStats()
{
    hero.viewStats();
    displayMenu();
}

void World::viewInventory()
{
    hero.displayInventory(SHOW_ALL);
    displayMenu();
}

void World::visitStore()
{
    string prompt, dummy;
    int index;
    char opt, yn;
    Weapon w;

    prompt = "\r\n1) Buy\r\n2) Sell\r\n>>";

    cout << prompt;
    cin >> opt;

    cout << "\r\nGold: " << hero.getGold() << "\r\n\n";

    switch (opt)
    {
        case '1':
            store.displayInventory();
            prompt = "Would you like to purchase an item? (y/n)\r\n>>";

            cout << prompt;
            cin >> yn;
            //clear rest of line, if more than one word was entered
            getline(cin, dummy);

            if (yn == 'y' || yn == 'Y')
            {
                prompt = "Enter the number of the item you wish to purchase.\r\n>>";

                cout << prompt;
                cin >> index;
                //clear rest of line, if more than one word was entered
                getline(cin, dummy);

                w = store.purchaseItem(index, hero.getGold());
                hero.addItem(w);
                hero.modifyGold(w.getCost() * -1);
            }
            break;
        case '2':
            hero.displayInventory(SHOW_SELL);
            break;
    }

    displayMenu();
}

void World::randomEncounter()
{
    string prompt, dummy;
    bool victory;
    int type;

    prompt = "Select (0,1,2) for (Easy ,Medium, Hard)\r\n>>";

    cout << prompt;
    cin >> type;

    srand(hero.getGold() + hero.getHealth() + hero.getXP() + type);

    Battle battle(hero, type, rand() % NUM_MONSTER_TYPES);

    victory = battle.initiate();

    if (victory)
    {
        cout << hero.getName() << " gained " << battle.getPrize() * GOLD_MOD
            << " gold, and " << battle.getPrize() << " xp!";
        hero.modifyExperience(battle.getPrize());
        hero.modifyGold(battle.getPrize() * GOLD_MOD);
    }

    if (hero.isDead())
    {
        quitGame();
    }
    else
    {
        //restore mana after each battle
        hero.restoreMana();
        displayMenu();
    }
}

void World::saveGame()
{
    hero.save();

    displayMenu();
}

void World::loadGame()
{
    string nameIn;
    vector<string> items;

    cout << "\r\nEnter Character Name to Load >>";
    cin >> nameIn;

    if (!hero.load(nameIn))
    {
        cout << "\r\nNo such Character found.\r\n";
    }
    else
    {
        items = hero.getItems();

        for (int i = 0; i < items.size(); i++)
        {
            hero.addItem(store.getItem(items.at(i)));
        }
        cout << "\r\n" << nameIn << " loaded!\r\n";

        hero.displayInventory(SHOW_ALL);
    }

    displayMenu();
}

void World::quitGame()
{
    if (hero.isDead())
    {
        cout << "\r\nOh no! You died!";
    }
    cout << "\r\nThanks for Playing!";
}
