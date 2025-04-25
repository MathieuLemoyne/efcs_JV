#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "CollisionCircle.h"

using namespace sf;

class GameObject : public Sprite
{
public:
	GameObject();
	void setTexture(const Texture& texture, const bool resetRect = false);

	void activate();
	void deactivate();
	bool isActive() const;

	float getRotationRadians() const;
	float getRotationRadianInDegrees() const;
	void setRotationRadians(const float angle);
	void rotateInRadians(const float angle);

	const CollisionCircle& getCollisionCircle() const;
	bool isCircleColliding(const GameObject& other) const;
	float getCollisionCircleRadius() const;
	void setCollisionCircleRadius(const float radius);

	void setPosition(const float x, const float y);
	void setPosition(const Vector2f& position);

	bool isClicked(const Vector2f& position) const;

	void move(const float offsetX, const float offsetY);
	void move(const Vector2f& offset);
	void moveBack();

	void draw(RenderWindow& renderWindow) const;

protected:
	static const float PI;
	static const float RADIANS_TO_DEGREE;

	CollisionCircle collisionCircle;

private:
	bool active;
	float rotationRadians;
	Vector2f backupPosition;
};