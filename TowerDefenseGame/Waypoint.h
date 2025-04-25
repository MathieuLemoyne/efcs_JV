#pragma once
#include "GameObject.h"
class Waypoint : public GameObject
{
public:
	//Waypoint();
    Waypoint(const sf::Vector2f& position);

    void setNext(Waypoint* next);
    Waypoint* getNext() const;

    bool init();

private:
    Waypoint* next = nullptr;
};

