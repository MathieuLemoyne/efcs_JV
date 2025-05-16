#include "ArcherTower.h"
#include "ContentPipeline.h"
#include <cstdlib> 

bool ArcherTower::init()
{
    GameObject::setTexture(ContentPipeline::getInstance().getArcherTowerTexture());
    GameObject::setOrigin(75, 75);
    attackCooldown = 0.8f;
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


int ArcherTower::getDamage() const {
    return 1 + (std::rand() % 4);
}

