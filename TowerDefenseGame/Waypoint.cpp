#include "Waypoint.h"
#include "ContentPipeline.h"
//Waypoint::Waypoint()
//{
//}
void draw(RenderWindow& renderWindow) 
{
	// Quand W est appuyé, on appelle la fonction draw dans une loop foreach de waypoints
	draw(renderWindow);
}

Waypoint::Waypoint(const sf::Vector2f& position)
{
	setPosition(position);
	activate();
	init();
}

void Waypoint::setNext(Waypoint* next)
{
	this->next = next;
}

Waypoint* Waypoint::getNext() const
{
	return next;
}

bool Waypoint::init()
{
	setTexture(ContentPipeline::getInstance().getWaypointTexture());

	return true;
}
