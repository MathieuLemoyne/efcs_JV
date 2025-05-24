#include "MageTower.h"
#include "ContentPipeline.h"
#include <cstdlib>
#include <iostream>

bool MageTower::init()
{
	std::cout << "MageTower::init()" << std::endl;
    setTexture(ContentPipeline::getInstance().getMageTowerTexture());
    setTextureRect(sf::IntRect(0, 0, 150, 150));
    setOrigin(75, 125);
    attackCooldown = 1.5f;
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

int MageTower::getDamage() const {
    return 1 + (std::rand() % 12);
}
