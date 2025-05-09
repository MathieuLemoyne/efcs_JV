#pragma once
#include "GameObject.h"

class TowerEmplacement : public GameObject
{
public:
	TowerEmplacement();

	bool init();

	void draw(RenderWindow& renderWindow);

	bool isMouseOver(sf::Vector2f mousePos) const;

private:
};

