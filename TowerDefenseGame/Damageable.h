#pragma once
class Damageable {
public:
    virtual void takeDamage(int amount) = 0;
    virtual bool isAlive() const = 0;
    virtual ~Damageable() = default;
};
