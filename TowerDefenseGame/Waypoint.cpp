#include "Waypoint.h"

//Waypoint::Waypoint()
//{
//}
Waypoint::~Waypoint()
{
}
void draw(RenderWindow& renderWindow) 
{
	// Quand W est appuyé, on appelle la fonction draw dans une loop foreach de waypoints
	draw(renderWindow);
}

Waypoint::Waypoint(const sf::Vector2f& position)
{
	setPosition(position);
}

void Waypoint::setNext(Waypoint* next)
{
	this->next = next;
}

Waypoint* Waypoint::getNext() const
{
	return next;
}
