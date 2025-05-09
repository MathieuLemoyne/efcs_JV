#include "KingTower.h"
#include "Constants.h"
#include "ContentPipeline.h"
KingTower::KingTower()
{
}

bool KingTower::init()
{
    setPosition(KING_POS); 

    activate();

    setTexture(ContentPipeline::getInstance().getKingTowerTexture());

    // set le collision circle ou hitbox ou whatever

    return true;
}
void KingTower::draw(RenderWindow& renderWindow)
{
    renderWindow.draw(*this);
}
void KingTower::update(float dt)
{
}