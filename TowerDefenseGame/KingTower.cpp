#include "KingTower.h"
#include "Constants.h"
#include "ContentPipeline.h"
KingTower::KingTower()
{
}

bool KingTower::init()
{
    Tower::init(true);
    setTexture(ContentPipeline::getInstance().getKingTowerTexture());
    setCollisionCircleRadius(getGlobalBounds().width / 4.f);
    health = 500;
	barOffsetY = 75.f;
    activate();
    return true;
}

void KingTower::draw(RenderWindow& window) {
    if (!isAlive()) return;
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(*this);
}

void KingTower::update(float dt)
{
    Tower::update(dt);
}
