#pragma once
#include "GameObject.h"
#include "Damageable.h"

class Shooter
{
public:
    virtual bool shoot() = 0;

    virtual int getDamage() const = 0;
    virtual float getAttackFrequency() const = 0;
    virtual bool canAttack() const = 0;

    virtual ~Shooter() = default;
};
