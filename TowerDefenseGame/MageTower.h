#pragma once
#include "Tower.h"
class MageTower : public Tower
{
public:
	bool init();
	void draw(sf::RenderWindow& window) override;

	void update(float dt) override;
	void shoot() override;
private:
	int currentFrame = 0;
	float animationTimer = 0.f;
	const float frameDuration = 0.2f;
	const int frameCount = 3;
	const int frameWidth = 64;
	const int frameHeight = 64;
};

