#include "MageTower.h"
#include "ContentPipeline.h"
bool MageTower::init()
{
	setTexture(ContentPipeline::getInstance().getMageTowerTexture());
	setTextureRect(sf::IntRect(0, 0, 150, 150)); // a voir
    setOrigin(75, 75);

	return true;
}

void MageTower::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*this);
}
void MageTower::update(float dt)
{
// mettre lanimation
}
void MageTower::shoot()
{
}