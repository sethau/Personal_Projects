#include "Hero.h"

const int HEALTH_BASE_FACTOR = 20;
const int HEALTH_LEVEL_FACTOR = 30;
const int HEALTH_MOD = 60;
const int MANA_BASE_FACTOR = 30;
const int MANA_LEVEL_FACTOR = 20;
const int MANA_MOD = 130;
const int EXP_BASE = 50;
const double EXP_LEVEL_POW = 1.5;
const int STR_FACTOR = 3;
const int BASE_GOLD = 20;
const int BASE_HEALTH = 50;
const int BASE_MANA = 100;
const int SPELL = 0;
const int WEAPON = 1;
const int POTION = 2;
const int SHOW_ALL = 0;
const int SHOW_SPELLS = 1;
const int SHOW_SELL = 2;
const int BALANCED[] = {6, 6, 3, 5};

Hero::Hero()
{
    strength = BALANCED[0];
    armor = BALANCED[1];
    speed = BALANCED[2];
    intelligence = BALANCED[3];

    gold = BASE_GOLD;
    experience = 0;
    health = BASE_HEALTH;
    mana = BASE_MANA;
    level = 1;
    nextLevel = calculateNextLevel();
}

Hero::Hero(const int stats[])
{
    strength = stats[0];
    armor = stats[1];
    speed = stats[2];
    intelligence = stats[3];

    gold = BASE_GOLD;
    experience = 0;
    level = 1;
    nextLevel = calculateNextLevel();

    maxHealth = calculateMaxHealth();
    health = maxHealth;

    maxMana = calculateMaxMana();
    mana = maxMana;
    if (mana > 0)
    {
        mana = 0;
    }
}

void Hero::viewStats()
{
    cout << "Name: " << name << "\r\n";
    cout << "Level: " << level << "\r\n";
    cout << "Health: " << health << "\r\n";
    cout << "Mana: " << mana << "\r\n";
    cout << "Experience: " << experience << "\r\n";
    cout << "To Next Level: " << nextLevel << "\r\n\n";
    cout << "Strength: " << strength << "\r\n";
    cout << "Speed: " << speed << "\r\n";
    cout << "Armor: " << armor << "\r\n";
    cout << "Intelligence: " << intelligence << "\r\n";
}

void Hero::setName(string nameIn)
{
    name = nameIn;
}

string Hero::getName()
{
    return name;
}

int Hero::getStrength()
{
    return strength;
}

int Hero::getArmor()
{
    return armor;
}

int Hero::getSpeed()
{
    return speed;
}

int Hero::getIntelligence()
{
    return intelligence;
}

int Hero::getHealth()
{
    return health;
}

int Hero::getMana()
{
    return mana;
}

int Hero::getAttackBonus()
{
    return weapon.getStrength() * STR_FACTOR;
}

void Hero::addItem(Weapon weaponIn)
{
    if (weaponIn.getName().compare("none") != 0)
    {
        inventory.push_back(weaponIn);
        if (weaponIn.getType() == WEAPON)
        {
            weapon = weaponIn;
        }
    }
}

int Hero::getGold()
{
    return gold;
}

bool Hero::displayInventory(int type)
{
    int option;

    if (type == SHOW_ALL)
    {
        cout << "Gold: " << gold << "\r\n\n";

        for (int i = 0; i < inventory.size(); i++)
        {
            cout << "\r\n" << (i + 1) << ") ";
            if (weapon.getName().compare(inventory.at(i).getName()) == 0)
            {
                cout << "Currently Equipped";
            }
            inventory.at(i).printInfo();
        }

        cout << "\r\nTo equip/use an item, enter item number. If not, enter 0.\r\n>>";
        cin >> option;

        if (option > 0 && option <= inventory.size())
        {
            equip(option - 1);
        }
    }
    else if (type == SHOW_SPELLS)
    {
        for (int i = 0; i < inventory.size(); i++)
        {
            if (inventory.at(i).getType() == SPELL
                && inventory.at(i).getManaCost() <= mana)
            {
                cout << "\r\n" << (i + 1) << ") ";
                inventory.at(i).printInfo();
            }
        }

        cout << "\r\nTo use a spell, enter spell number. If not, enter 0.\r\n>>";
        cin >> option;

        if (option > 0 && option <= inventory.size()
            && inventory.at(option - 1).getType() == SPELL
                && inventory.at(option - 1).getManaCost() <= mana)
        {
            tempWeapon = weapon;
            weapon = inventory.at(option - 1);
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (type == SHOW_SELL)
    {
        cout << "Gold: " << gold << "\r\n\n";

        for (int i = 0; i < inventory.size(); i++)
        {
            cout << "\r\n" << (i + 1) << ") ";
            if (weapon.getName().compare(inventory.at(i).getName()) == 0)
            {
                cout << "Currently Equipped";
            }
            inventory.at(i).printInfo();
        }

        cout << "\r\nEnter number of item to be sold (or 0 to cancel)\r\n>>";
        cin >> option;

        if (option > 0 && option <= inventory.size())
        {
            modifyGold(inventory.at(option - 1).getCost());
            inventory.erase(inventory.begin() + option - 1);
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Hero::equip(int i)
{
    Weapon w = inventory.at(i);

    if (w.getType() == POTION)
    {
        if (w.getName().find("Mana") < string::npos
            && !fullMana())
        {
            modifyMana(w.getMana());
            inventory.erase(inventory.begin() + i);
        }
        else if (w.getName().find("Health") < string::npos
            && !fullHealth())
        {
            modifyHealth(w.getHealth());
            inventory.erase(inventory.begin() + i);
        }
        else if (!(fullMana() && fullHealth()))
        {
            modifyHealth(w.getHealth());
            modifyMana(w.getMana());
            inventory.erase(inventory.begin() + i);
        }
    }
    else if (w.getType() == WEAPON)
    {
        weapon = w;
    }
}

void Hero::modifyHealth(int mod)
{
    health += mod;

    if (health > maxHealth)
    {
        restoreHealth();
    }
}

void Hero::modifyMana(int mod)
{
    mana += mod;

    if (mana > maxMana)
    {
        restoreMana();
    }
}

void Hero::restoreHealth()
{
    health = maxHealth;
}

void Hero::restoreMana()
{
    mana = maxMana;
    if (mana < 0)
    {
        mana = 0;
    }
}

void Hero::modifyGold(int mod)
{
    gold += mod;
}

void Hero::modifyExperience(int mod)
{
    experience += mod;
    nextLevel -= mod;

    if (nextLevel <= 0)
    {
        levelUp();
    }
}
int Hero::getLevel()
{
    return level;
}

bool Hero::fullHealth()
{
    if (health == maxHealth)
    {
        return true;
    }

    return false;
}

bool Hero::fullMana()
{
    if (mana == maxMana)
    {
        return true;
    }

    return false;
}

bool Hero::isDead()
{
    return health <= 0;
}

void Hero::levelUp()
{
    int option;

    level++;

    maxHealth = calculateMaxHealth();
    health = maxHealth;

    maxMana = calculateMaxMana();
    mana = maxMana;

    if (mana < 0)
    {
        mana = 0;
    }

    experience = 0;
    nextLevel = calculateNextLevel();

    cout << "\r\nCongratulations! You've leveled up! ";
    cout << "You are now LEVEL " << level << "!\r\n";

    cout << "\r\n\nChoose which attribute to increase!\r\n";
    cout << "1) Strength\r\n";
    cout << "2) Speed\r\n";
    cout << "3) Armor\r\n";
    cout << "4) Intelligence\r\n";

    cin >> option;

    switch (option)
    {
        case 1:
            strength++;
            cout << "\r\nStrength is now " << strength << "!\r\n";
            break;
        case 2:
            speed++;
            cout << "\r\nSpeed is now " << speed << "!\r\n";
            break;
        case 3:
            armor++;
            cout << "\r\nArmor is now " << armor << "!\r\n";
            break;
        case 4:
            intelligence++;
            cout << "\r\nIntelligence is now " << intelligence << "!\r\n";
            break;
        default:
            strength++;
            cout << "\r\nStrength is now " << strength << "!\r\n";
            break;
    }
}

int Hero::getXP()
{
    return experience;
}

void Hero::save()
{
    ofstream fileW;
    string fileName;

    fileName = name + ".txt";

    fileW.open(fileName.c_str());

    fileW << name + "\r\n";
    fileW << gold << "\r\n";
    fileW << health << "\r\n";
    fileW << mana << "\r\n";
    fileW << level << "\r\n";
    fileW << experience << "\r\n";
    fileW << nextLevel << "\r\n";
    fileW << strength << "\r\n";
    fileW << armor << "\r\n";
    fileW << speed << "\r\n";
    fileW << intelligence << "\r\n";

    for (int i = 0; i < inventory.size(); i++)
    {
        fileW << inventory.at(i).getName() + "\r\n";
    }

    fileW.close();

    cout << "\r\nGame Saved!\r\n\n";
}
bool Hero::load(string nameIn)
{
    ifstream fileR;
    string fileName, line;

    fileName = nameIn + ".txt";

    fileR.open(fileName.c_str());

    if (!fileR.is_open())
    {
        return false;
    }

    getline(fileR, line);
    name = line.substr(0, line.size() - 1);

    fileR >> gold;
    getline(fileR, line);

    fileR >> health;
    getline(fileR, line);

    fileR >> mana;
    getline(fileR, line);

    fileR >> level;
    getline(fileR, line);

    fileR >> experience;
    getline(fileR, line);

    fileR >> nextLevel;
    getline(fileR, line);

    fileR >> strength;
    getline(fileR, line);

    fileR >> armor;
    getline(fileR, line);

    fileR >> speed;
    getline(fileR, line);

    fileR >> intelligence;
    getline(fileR, line);

    loadItems.clear();
    inventory.clear();

    while (fileR.good())
    {
        getline(fileR, line);
        line = line.substr(0, line.size() - 1);
        if (line.size() > 0)
        {
            loadItems.push_back(line);
        }
    }

    fileR.close();

    maxHealth = calculateMaxHealth();
    if (health > maxHealth)
    {
        restoreHealth();
    }

    maxMana = calculateMaxMana();
    restoreMana();

    nextLevel = calculateNextLevel();

    return true;
}

int Hero::calculateMaxHealth()
{
    return strength * HEALTH_BASE_FACTOR + level * HEALTH_LEVEL_FACTOR - HEALTH_MOD;
}

int Hero::calculateMaxMana()
{
    return intelligence * MANA_BASE_FACTOR + level * MANA_LEVEL_FACTOR - MANA_MOD;
}

int Hero::calculateNextLevel()
{
    return EXP_BASE * pow(level, EXP_LEVEL_POW) - experience;
}

vector<string> Hero::getItems()
{
    return loadItems;
}

string Hero::getWeaponName()
{
    return weapon.getName();
}

int Hero::getWeaponCost()
{
    return weapon.getCost();
}

int Hero::getWeaponManaCost()
{
    return weapon.getManaCost();
}

int Hero::getWeaponStrength()
{
    return weapon.getStrength();
}

int Hero::getWeaponOnDmgTm()
{
    return weapon.getOnDmgTm();
}

int Hero::getWeaponOnDmg()
{
    return weapon.getOnDmg();
}

int Hero::getWeaponWait()
{
    return weapon.getWait();
}

int Hero::getWeaponDrain()
{
    return weapon.getDrain();
}

int Hero::getWeaponHealth()
{
    return weapon.getHealth();
}

int Hero::getWeaponMana()
{
    return weapon.getMana();
}

void Hero::restoreWeapon()
{
    weapon = tempWeapon;
}
