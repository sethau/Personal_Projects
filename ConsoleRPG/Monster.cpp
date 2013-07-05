#include "Monster.h"

const int STRENGTH = 0;
const int SPEED = 1;
const int ARMOR = 2;
const int BASE_HEALTH = 20;
const int HEALTH_FACTOR = 30;
const int PRIMARY_BASE = 6;
const int SECONDARY_BASE = 6;
const int PRIZE_FACTOR = 3;

Monster::Monster()
{
    //necessary
}

Monster::Monster(int levelIn, int type)
{
    level = levelIn;

    switch (type)
    {
        case STRENGTH:
            strength = PRIMARY_BASE + level * 2.5;
            speed = SECONDARY_BASE + level;
            armor = SECONDARY_BASE + level / 2;
            health = BASE_HEALTH + HEALTH_FACTOR * level;
            break;
        case SPEED:
            strength = SECONDARY_BASE + level * 1.5;
            speed = PRIMARY_BASE + level * 1.5;
            armor = SECONDARY_BASE + level / 1.5;
            health = BASE_HEALTH + HEALTH_FACTOR * level;
            break;
        case ARMOR:
            strength = SECONDARY_BASE + level * 1.5;
            speed = SECONDARY_BASE + level / 2;
            armor = PRIMARY_BASE + level * 2.5;
            health = BASE_HEALTH + HEALTH_FACTOR * level;
            break;
    }
}

int Monster::getSpeed()
{
    return speed;
}

int Monster::getStrength()
{
    return strength;
}

int Monster::getAttackBonus()
{
    return strength;
}

int Monster::getArmor()
{
    return armor;
}

int Monster::getLevel()
{
    return level;
}

int Monster::getHealth()
{
    return health;
}

void Monster::modifyHealth(int mod)
{
    health += mod;
}

int Monster::getPrize()
{
    return (strength + speed + armor) * level / PRIZE_FACTOR;
}

bool Monster::isDead()
{
    return health <= 0;
}
