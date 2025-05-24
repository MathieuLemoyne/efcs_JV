#include "GameObject.h"

const float GameObject::RADIANS_TO_DEGREE = 57.295779513f;
const float GameObject::PI = 3.141592653f;

GameObject::GameObject() : active(false), rotationRadians(0.0f)
{
}

void GameObject::setTexture(const Texture& texture, const bool resetRect)
{
	Sprite::setTexture(texture, resetRect);
	setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	collisionCircle.setRadius(texture.getSize().x / 2);
}

void GameObject::activate()
{
	active = true;
}

void GameObject::deactivate()
{
	active = false;
}

bool GameObject::isActive() const
{
	return active;
}

float GameObject::getRotationRadians() const
{
	return rotationRadians;
}

float GameObject::getRotationRadianInDegrees() const
{
	return rotationRadians * RADIANS_TO_DEGREE;
}

void GameObject::setRotationRadians(const float angle)
{
	rotationRadians = angle;
}
void GameObject::rotateInRadians(const float angle)
{
	rotationRadians += angle;
}

const CollisionCircle& GameObject::getCollisionCircle() const
{
	return collisionCircle;
}

bool GameObject::isCircleColliding(const GameObject& other) const
{
	if (active && other.active)
		return this->getCollisionCircle().checkCollision(other.getCollisionCircle());

	return false;
}

float GameObject::getCollisionCircleRadius() const
{
	return collisionCircle.getRadius();
}

void GameObject::setCollisionCircleRadius(const float radius)
{
	collisionCircle.setRadius(radius);
	collisionCircle.setPosition(getPosition().x, getPosition().y);
}

void GameObject::setPosition(const float x, const float y)
{
	Sprite::setPosition(x, y);
	collisionCircle.setPosition(x, y);
}

void GameObject::setPosition(const Vector2f& position)
{
	setPosition(position.x, position.y);
}

bool GameObject::isClicked(const Vector2f& position) const
{
	if (getGlobalBounds().contains(position)) return true;

	return false;
}

void GameObject::move(const float offsetX, const float offsetY)
{
	if (active)
	{
		backupPosition = getPosition();
		Sprite::move(offsetX, offsetY);
		collisionCircle.setPosition(getPosition().x, getPosition().y);
	}
}

void GameObject::move(const Vector2f& offset)
{
	move(offset.x, offset.y);
}

void GameObject::moveBack()
{
	setPosition(backupPosition);
}

void GameObject::draw(RenderWindow& renderWindow) const
{
	if (active) renderWindow.draw(*this);
}