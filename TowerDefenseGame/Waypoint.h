#pragma once
#include "GameObject.h"
class Waypoint : public GameObject
{
public:
	Waypoint();

    void setNext(Waypoint* next);
    Waypoint* getNext() const;

    bool init();

    void draw(RenderWindow& renderWindow);
private:
    Waypoint* next = nullptr;
};

