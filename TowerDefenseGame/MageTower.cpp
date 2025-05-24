#include "MageTower.h"
#include "ContentPipeline.h"
#include <cstdlib>
#include <iostream>

bool MageTower::init() {
    Tower::init(false);
    setTexture(ContentPipeline::getInstance().getMageTowerTexture());
    setTextureRect(sf::IntRect(0, 0, 150, 150));
    setOrigin(75, 105);
    setCollisionCircleRadius(getGlobalBounds().width / 4.f);
    attackCooldown = 1.5f;
	barOffsetY = 95.f;
    return true;
}

void MageTower::draw(RenderWindow& window) {
    if (!isAlive()) return;
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(*this);
}

void MageTower::update(float dt)
{
    timeSinceLastAttack += dt;
    Tower::update(dt);
    // mettre lanimation
}

int MageTower::getDamage() const {
    return 1 + (std::rand() % 12);
}
