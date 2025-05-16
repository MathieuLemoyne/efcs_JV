#include "MageTower.h"
#include "ContentPipeline.h"

bool MageTower::init()
{
	setTexture(ContentPipeline::getInstance().getMageTowerTexture());
	setTextureRect(sf::IntRect(0, 0, 150, 250));
	setOrigin(75, 125);

	attackCooldown = 1.5f;
	damage = 20;
	return true;
}

void MageTower::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*this);
}

void MageTower::update(float dt)
{
	timeSinceLastAttack += dt;

	// mettre lanimation

}
