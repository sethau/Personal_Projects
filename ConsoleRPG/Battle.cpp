#include "Battle.h"

const int SHOW_ALL = 0;
const int SHOW_SPELLS = 1;
const int MED_MOD = 1;
const int HARD_MOD = 3;
const int FIGHT = 1;
const int SPELL = 2;
const int INVENTORY = 3;
const int ATK_MOD = 2;
const int DRAIN_MOD = 1.5;

Battle::Battle(Hero &heroIn, int battleType, int monsterType)
{
    hero = &heroIn;
    timeCount = 0;

    switch (battleType)
    {
        case 0:
            monster = Monster(hero->getLevel(), monsterType);
            break;
        case 1:
            monster = Monster(hero->getLevel() + MED_MOD, monsterType);
            break;
        case 2:
            monster = Monster(hero->getLevel() + HARD_MOD, monsterType);
            break;
        default:
            monster = Monster(hero->getLevel(), monsterType);
            break;
    }

    //speed modulo(modulos?)(moduli?)(modulo?)(modulen?) to be used in turn allocation
    hMod = (1.0 / hero->getSpeed()) * (hero->getLevel() * 100);
    mMod = (1.0 / monster.getSpeed()) * (hero->getLevel() * 100);

}
bool Battle::initiate()
{
    option = 1;
    onWaitLeft = 0;
    onDmgLeft = 0;

    cout << "You have encountered a Level " << monster.getLevel() << " monster!\r\n";

    cout << "\nMonster Stats: Strength = " << monster.getStrength() << "  Speed = " << monster.getSpeed() << "  Armor = " << monster.getArmor() << "\r\n\n";

    cout << "What do you want to do?\r\n";
    cout << "1) Fight\r\n2) Cast a Spell\r\n3) Inventory\r\n4) Flee\r\n>>";
    cin >> option;

    if (option == FIGHT || option == SPELL || option == INVENTORY)
    {
        fight();
    }
    else
    {
        cout << "\r\nYou fled the battle!\r\n\n";
    }

    timeCount = 0;

    return (hero->getHealth() > 0 && monster.getHealth() <= 0);
}
bool Battle::fight()
{
    bool attack = false, display = true;

    if (option == INVENTORY)
    {
        display = false;
    }

    spell = &Weapon();

    srand(timeCount + hero->getHealth() + monster.getHealth() + monster.getPrize() + monster.getStrength());

    while (option == FIGHT || option == SPELL || option == INVENTORY)
    {
        timeCount++;

        if (option == INVENTORY && !display)
        {
            hero->displayInventory(SHOW_ALL);
            display = true;

            cout << "\r\n\n\tHP: " << hero->getHealth();
            cout << "\tMana: " << hero->getMana();
            cout << "\tMonster HP: " << monster.getHealth();
            cout << "\r\n1) Fight\r\n2) Cast a Spell\r\n3) Inventory\r\n4) Flee\r\n>>";
            cin >> option;

            if (option != FIGHT && option != SPELL && option != INVENTORY)
            {
                cout << "\r\nYou fled the battle!\r\n\n";
                return false;
            }
            else if (option == 3)
            {
                display = false;
            }
        }

        if (timeCount % hMod == 0
            && timeCount % mMod != 0)
        {
            attack = heroAttack();
        }
        else if (timeCount % mMod == 0
            && timeCount % hMod != 0)
        {
            spellOngoing(0, 0);

            attack = monsterAttack();
        }
        else if (timeCount % mMod == 0
            && timeCount % hMod == 0)
        {
            if (hMod <= mMod)
            {
                attack = heroAttack();

                if (attack)
                {
                    spellOngoing(0, 0);

                    attack = monsterAttack();
                }
            }
            else if (hMod >= mMod)
            {
                spellOngoing(0, 0);

                attack = monsterAttack();

                if (attack)
                {
                    attack = heroAttack();
                }
            }
        }

        if (hero->isDead() || monster.isDead())
        {
            return hero->isDead();
        }

        if (attack)
        {
            cout << "\r\n\n\tHP: " << hero->getHealth();
            cout << "\tMana: " << hero->getMana();
            cout << "\tMonster HP: " << monster.getHealth();
            cout << "\r\n1) Fight\r\n2) Cast a Spell\r\n3) Inventory\r\n4) Flee\r\n>>";
            cin >> option;

            if (option != FIGHT && option != SPELL && option != INVENTORY)
            {
                cout << "\r\nYou fled the battle!\r\n\n";
                return false;
            }
            else if (option == INVENTORY)
            {
                display = false;
            }
        }

        attack = false;
    }
}

bool Battle::heroAttack()
{
    int damage;
    bool valid;

    if (hero->isDead())
    {
        return false;
    }
    else
    {
        if (option == SPELL)
        {
            valid = hero->displayInventory(SHOW_SPELLS);

            if (valid)
            {
                cout << hero->getName() << " used " << hero->getWeaponName() << "!\r\n";
                spellDamage(hero->getWeaponStrength());
                spellOngoing(hero->getWeaponOnDmg(), hero->getWeaponOnDmgTm());
                spellWait(hero->getWeaponWait());
                spellDrain(hero->getWeaponDrain());
                spellHealth(hero->getWeaponHealth());
                spellMana(hero->getWeaponMana());
                hero->modifyMana(hero->getWeaponManaCost() * -1);
                hero->restoreWeapon();
            }

            return true;
        }

        if (option == FIGHT)
        {
            damage = rand() % (hero->getStrength() * ATK_MOD) + hero->getAttackBonus() - monster.getArmor();

            if (damage <= 0)
            {
                damage = 0;
            }

            cout << hero->getName() << " dealt " << damage << " damage to the monster!\r\n";
            monster.modifyHealth(-1 * damage);

            if (monster.isDead())
            {
                cout << "\r\n" << hero->getName() << " killed the monster!\r\n";
            }
            return true;
        }
    }
}

bool Battle::monsterAttack()
{
    int damage;

    if (monster.isDead())
    {
        return false;
    }
    else if (!spellWait(0))
    {                                                     //if monsters have weapons one day
        damage = rand() % ((monster.getStrength()) * ATK_MOD) /*+ monster.getAttackBonus()*/ - hero->getArmor();

        if (damage <= 0)
        {
            damage = 0;
        }

        cout << "Monster dealt " << damage << " damage to " << hero->getName() << "!\r\n";
        hero->modifyHealth(-1 * damage);

        if (hero->isDead())
        {
            cout << "\r\nOh no! " << hero->getName() << " was killed by the monster!\r\n";
        }
        return true;
    }
    else
    {
        return true;
    }
}

int Battle::getPrize()
{
    return monster.getPrize();
}

void Battle::spellDamage(int strength)
{
    int damage;

    if (strength > 0)
    {
        damage = (hero->getIntelligence() * ATK_MOD) * strength - monster.getArmor();

        if (damage <= 0)
        {
            damage = 0;
        }

        cout << hero->getName() << " dealt " << damage << " damage to the monster!\r\n";
        monster.modifyHealth(-1 * damage);

        if (monster.isDead())
        {
            cout << "\r\n" << hero->getName() << " killed the monster!\r\n";
        }
    }
}

void Battle::spellOngoing(int strength, int duration)
{
    if (strength > 0 && duration > 0)
    {
        dmg = (hero->getIntelligence()) * strength;
        onDmgLeft = duration;
    }
    else if (onDmgLeft > 0 && !monster.isDead())
    {
        if (dmg <= 0)
        {
            dmg = 0;
        }

        cout << "The monster took " << dmg << " ongoing damage!\r\n";
        monster.modifyHealth(-1 * dmg);

        if (monster.isDead())
        {
            cout << "\r\n" << hero->getName() << " killed the monster!\r\n";
        }

        onDmgLeft--;
    }
}

bool Battle::spellWait(int duration)
{
    if(duration > 0)
    {
        onWaitLeft = duration;
        return false;
    }
    else if (onWaitLeft > 0 && !monster.isDead())
    {
        cout << "The monster is unable to attack!\r\n";

        onWaitLeft--;

        return true;
    }

    return false;
}

void Battle::spellDrain(int strength)
{
    int damage;

    if (strength > 0 && !monster.isDead())
    {
        damage = (hero->getIntelligence() * DRAIN_MOD) * strength;
        cout << hero->getName() << " drained " << damage << " health from the monster!\r\n";
        monster.modifyHealth(-1 * damage);
        hero->modifyHealth(damage);
    }

    if (monster.isDead())
    {
        cout << "\r\n" << hero->getName() << " killed the monster!\r\n";
    }
}

void Battle::spellHealth(int strength)
{
    int mod;

    if (strength > 0)
    {
        mod = (hero->getIntelligence() * DRAIN_MOD) * strength;

        cout << hero->getName() << " restored " << mod << " health!\r\n";

        hero->modifyHealth(mod);
    }
    if (strength < 0)
    {
        mod = hero->getHealth() / (-1 * strength) - 1;

        cout << hero->getName() << " sacrificed " << mod << " health to restore mana!\r\n";

        hero->modifyHealth(-1 * mod);
        hero->restoreMana();
    }
}

void Battle::spellMana(int strength)
{
    int mod;

    if (strength > 0)
    {
        mod = (hero->getIntelligence() * DRAIN_MOD) * strength;

        cout << hero->getName() << " restored " << mod << " mana!\r\n";

        hero->modifyMana(mod);
    }
}
