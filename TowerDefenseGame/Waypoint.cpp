#include "Waypoint.h"
#include "ContentPipeline.h"
Waypoint::Waypoint()
{
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

void Waypoint::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*this);
}