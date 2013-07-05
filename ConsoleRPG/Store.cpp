#include "Store.h"

const int SPELL = 0;
const int WEAPON = 1;
const int POTION = 2;

//if inventory is expanded, consider loading from a file instead
Store::Store()
{
    Weapon weapon;
    string name;
    int type;
    int attributes[9];//{gold cost, mana cost, strength, ngng dmg tm, ngng dmg, #turns wait, health drain, health regain, mana regain}

    //spells

    name = "Heal";
    type = SPELL;
    attributes = {10,10,0,0,0,0,0,1,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Meditate";
    type = SPELL;
    attributes = {10,0,0,0,0,0,0,0,1};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Magic Missile";
    type = SPELL;
    attributes = {20,10,1,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Health Drain";
    type = SPELL;
    attributes = {80,60,0,0,0,0,2,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Flame Vortex";
    type = SPELL;
    attributes = {250,60,0,3,1,3,0,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Lightning Bolt";
    type = SPELL;
    attributes = {450,150,5,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Sacrifice";
    type = SPELL;
    attributes = {500,0,0,0,0,0,0,-2,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Fountain of Youth";
    type = SPELL;
    attributes = {800,200,0,0,0,0,0,10,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Dark Swarm";
    type = SPELL;
    attributes = {1500,300,10,3,2,2,0,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    name = "Wrath of the Gods";
    type = SPELL;
    attributes = {3000,400,15,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    spells.push_back(weapon);

    //weapons

    name = "Club";
    type = WEAPON;
    attributes = {10,0,1,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Dagger";
    type = WEAPON;
    attributes = {40,0,2,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Short Sword";
    type = WEAPON;
    attributes = {100,0,3,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Long Sword";
    type = WEAPON;
    attributes = {200,0,4,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Battle Axe";
    type = WEAPON;
    attributes = {300,0,5,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Morning Star";
    type = WEAPON;
    attributes = {400,0,6,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "War Hammer";
    type = WEAPON;
    attributes = {550,0,8,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Great Sword";
    type = WEAPON;
    attributes = {700,0,10,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Great Axe";
    type = WEAPON;
    attributes = {900,0,12,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    name = "Light Saber";
    type = WEAPON;
    attributes = {1000,0,15,0,0,0,0,0,0};
    weapon = Weapon(name, type, attributes);
    weapons.push_back(weapon);

    //potions

    name = "+20 Health Potion";
    type = POTION;
    attributes = {10,0,0,0,0,0,0,20,0};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "+50 Health Potion";
    type = POTION;
    attributes = {20,0,0,0,0,0,0,50,0};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "Health Restore Potion";
    type = POTION;
    attributes = {100,0,0,0,0,0,0,999999,0};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "+20 Mana Potion";
    type = POTION;
    attributes = {10,0,00,0,0,0,0,0,20};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "+50 Mana Potion";
    type = POTION;
    attributes = {20,0,00,0,0,0,0,0,50};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "Mana Restore Potion";
    type = POTION;
    attributes = {150,0,0,0,0,0,0,0,999999};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);

    name = "Full Restore Potion";
    type = POTION;
    attributes = {250,0,0,0,0,0,0,999999,999999};
    weapon = Weapon(name, type, attributes);
    potions.push_back(weapon);
}

void Store::displayInventory()
{
    int s;

    cout << "\r\n1) View Weapons\r\n2) View Spells\r\n3) View Potions\r\n>>";
    cin >> option;

    switch (option)
    {
        case 1:
            s = weapons.size();

            for (int i = 0; i < s; i++)
            {
                cout << "\r\n" << (i + 1) << ") ";
                weapons.at(i).printInfo();
            }
            break;
        case 2:
            s = spells.size();

            for (int i = 0; i < s; i++)
            {
                cout << "\r\n" << (i + 1) << ") ";
                spells.at(i).printInfo();
            }
            break;
        case 3:
            s = potions.size();

            for (int i = 0; i < s; i++)
            {
                cout << "\r\n" << (i + 1) << ") ";
                potions.at(i).printInfo();
            }
            break;
    }

}

Weapon Store::purchaseItem(int i, int gold)
{
    if (i > 0)
    {
        switch (option)
        {
            case 1:
                if (i <= weapons.size() && weapons.at(i - 1).getCost() <= gold)
                {
                    return weapons.at(i - 1);
                }
                else if (weapons.at(i - 1).getCost() <= gold)
                {
                    cout << "\r\nNot enough gold!\r\n";
                }
                break;
            case 2:
                if (i <= spells.size() && spells.at(i - 1).getCost() <= gold)
                {
                    return spells.at(i - 1);
                }
                else if (spells.at(i - 1).getCost() <= gold)
                {
                    cout << "\r\nNot enough gold!\r\n";
                }
                break;
            case 3:
                if (i <= potions.size() && potions.at(i - 1).getCost() <= gold)
                {
                    return potions.at(i - 1);
                }
                else if (potions.at(i - 1).getCost() <= gold)
                {
                    cout << "\r\nNot enough gold!\r\n";
                }
                break;
        }
    }
    return Weapon();
}

Weapon Store::getItem(string name)
{
    int s, i;

    s = weapons.size();
    for (i = 0; i < s; i++)
    {
        if (weapons.at(i).getName().compare(name) == 0)
        {
            return weapons.at(i);
        }
    }
    s = potions.size();
    for (i = 0; i < s; i++)
    {
        if (potions.at(i).getName().compare(name) == 0)
        {
            return potions.at(i);
        }
    }
    s = spells.size();
    for (i = 0; i < s; i++)
    {
        if (spells.at(i).getName().compare(name) == 0)
        {
            return spells.at(i);
        }
    }
    return Weapon();
}
