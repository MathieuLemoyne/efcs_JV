#pragma once
#include "GameObject.h"

class Shooter : public GameObject
{
public:
    virtual void shoot() = 0; 
};
