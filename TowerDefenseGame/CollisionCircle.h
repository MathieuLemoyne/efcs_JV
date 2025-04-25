#pragma once

class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(const float radius, const float x, const float y);
	bool checkCollision(const CollisionCircle& otherCircle) const;

	float getRadius() const;
	float getX() const;
	float getY() const;

	void setPosition(const float x, const float y);
	void setRadius(const float radius);

private:
	float radius;
	float x;
	float y;
};