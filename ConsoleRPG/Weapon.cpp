#include "Weapon.h"

const int SPELL = 0;
const int WEAPON = 1;
const int POTION = 2;
const int GOLD_COST = 0;
const int MANA_COST = 1;
const int STRENGTH = 2;
const int ONGOING_TIME = 3;
const int ONGOING_DMG = 4;
const int TURNS_WAIT = 5;
const int HEALTH_DRAIN = 6;
const int HEALTH_REGAIN = 7;
const int MANA_REGAIN = 8;

Weapon::Weapon()
{
    name = "none";
    attributes = {0,0,0,0,0,0,0,0,0};

    magic = false;
}

Weapon::Weapon(string nameIn, int typeIn, int attIn[9])
{
    name = nameIn;
    type = typeIn;
    attributes[GOLD_COST] = attIn[GOLD_COST];
    attributes[MANA_COST] = attIn[MANA_COST];
    attributes[STRENGTH] = attIn[STRENGTH];
    attributes[ONGOING_TIME] = attIn[ONGOING_TIME];
    attributes[ONGOING_DMG] = attIn[ONGOING_DMG];
    attributes[TURNS_WAIT] = attIn[TURNS_WAIT];
    attributes[HEALTH_DRAIN] = attIn[HEALTH_DRAIN];
    attributes[HEALTH_REGAIN] = attIn[HEALTH_REGAIN];
    attributes[MANA_REGAIN] = attIn[MANA_REGAIN];

    attIn[MANA_COST] > 0 ? magic = true : magic = false;
}

void Weapon::printInfo()
{
    cout << "\r\n" + name + "\r\n";
    if (type != SPELL)
    {
        cout << "Damage: " << attributes[STRENGTH] * 3 << "\n";
        cout << "Cost: " << attributes[GOLD_COST] << "\n\n";
        //magic ? (cout << "magic: yes" << "") : (cout << "magic: no") << "\n\n";
    }
    else
    {
        if (attributes[GOLD_COST] > 0)
        {
            cout << "Cost: " << attributes[GOLD_COST] << "\n";
        }
        if (attributes[MANA_COST] > 0 || type == SPELL)
        {
            cout << "Mana Cost: " << attributes[MANA_COST] << "\n";
        }
        if (attributes[STRENGTH] > 0)
        {
            cout << "Damage Strength: " << attributes[STRENGTH] << "\n";
        }
        if (attributes[ONGOING_TIME] > 0)
        {
            cout << "Turns Ongoing: " << attributes[ONGOING_TIME] << "\n";
        }
        if (attributes[ONGOING_DMG] > 0)
        {
            cout << "Damage Strength Ongoing: " << attributes[ONGOING_DMG] << "\n";
        }
        if (attributes[TURNS_WAIT] > 0)
        {
            cout << "Opponent Turns Lost: " << attributes[TURNS_WAIT] << "\n";
        }
        if (attributes[HEALTH_DRAIN] > 0)
        {
            cout << "Health Drain Strength: " << attributes[HEALTH_DRAIN] << "\n";
        }
        if (name.compare("Sacrifice") == 0)
        {
            cout << "Sacrifice: 1/" << (attributes[HEALTH_REGAIN] * -1) << " Health\n";
            cout << "Reward: Full Mana Restore\n";
        }
        else
        {
            if (attributes[HEALTH_REGAIN] > 0)
            {
                cout << "Health Restore Strength: " << attributes[HEALTH_REGAIN] << "\n";
            }
            if (attributes[MANA_REGAIN] > 0)
            {
                cout << "Mana Restore Strength: " << attributes[MANA_REGAIN] << "\n";
            }

        }
    }
}

int Weapon::getCost()
{
    return attributes[GOLD_COST];
}

int Weapon::getManaCost()
{
    return attributes[MANA_COST];
}

int Weapon::getStrength()
{
    return attributes[STRENGTH];
}

int Weapon::getOnDmgTm()
{
    return attributes[ONGOING_TIME];
}

int Weapon::getOnDmg()
{
    return attributes[ONGOING_DMG];
}

int Weapon::getWait()
{
    return attributes[TURNS_WAIT];
}

int Weapon::getDrain()
{
    return attributes[HEALTH_DRAIN];
}

int Weapon::getHealth()
{
    return attributes[HEALTH_REGAIN];
}

int Weapon::getMana()
{
    return attributes[MANA_REGAIN];
}

string Weapon::getName()
{
    return name;
}

int Weapon::getType()
{
    return type;
}
