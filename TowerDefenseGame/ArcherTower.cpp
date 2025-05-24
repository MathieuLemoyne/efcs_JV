#include "ArcherTower.h"
#include "ContentPipeline.h"
#include <cstdlib> 
#include <iostream>

bool ArcherTower::init()
{
    Tower::init(false);
    GameObject::setTexture(ContentPipeline::getInstance().getArcherTowerTexture());
    GameObject::setOrigin(75, 75);
    Vector2u texSize = getTexture()->getSize();
    float radius = texSize.x / 4.f;
	std::cout << "Archer Tower radius: " << radius << " Collision circle Radius: " << getCollisionCircleRadius() << std::endl;
    setCollisionCircleRadius(radius);
    std::cout << " Collision circle Radius: " << getCollisionCircleRadius() << std::endl;
    attackCooldown = 0.8f;
    activate();
    return true;
}

void ArcherTower::draw(RenderWindow& window) {
    if (!isAlive()) return;
    window.draw(*this);
    window.draw(healthBarBackground);
    window.draw(healthBar);
}

void ArcherTower::update(float dt)
{
    timeSinceLastAttack += dt;
    Tower::update(dt);
}


int ArcherTower::getDamage() const {
    return 1 + (std::rand() % 4);
}

