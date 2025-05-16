#include "ArcherTower.h"  
#include "ContentPipeline.h"  

bool ArcherTower::init()
{
	GameObject::setTexture(ContentPipeline::getInstance().getArcherTowerTexture());
	GameObject::setOrigin(75, 75);
	attackCooldown = 0.8f;
	damage = 10;
	return true;
}

void ArcherTower::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*this);
}


void ArcherTower::update(float dt)
{
	timeSinceLastAttack += dt;
}
