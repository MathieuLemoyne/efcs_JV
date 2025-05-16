#pragma once
#include "Tower.h"
class KingTower : public Tower
{

	public:
	KingTower();
	bool init();
	void draw(sf::RenderWindow& window) override;
	void update(float dt) override;
	void shoot() override;

};



