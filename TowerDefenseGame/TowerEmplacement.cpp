#include "TowerEmplacement.h"
#include "ContentPipeline.h"

TowerEmplacement::TowerEmplacement()
{
}
bool TowerEmplacement::init()
{
	setTexture(ContentPipeline::getInstance().getTowerEmplacementTexture());

	return true;
}

void TowerEmplacement::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*this);
}
bool TowerEmplacement::isMouseOver(sf::Vector2f mousePos) const {
	return getGlobalBounds().contains(mousePos);
}
