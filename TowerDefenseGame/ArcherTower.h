#pragma once
#include "Tower.h"
class ArcherTower : public Tower
{
public:
	bool init();
	void draw(sf::RenderWindow& window) override;
	void update(float dt) override;
	int getDamage() const;
};

