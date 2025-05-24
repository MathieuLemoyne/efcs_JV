#include "Projectile.h"
#include "ContentPipeline.h"
#include <cmath>

Projectile::Projectile()
	: speed(0.f), damage(0), targetPtr(nullptr)
{
	deactivate();
}

void Projectile::init(ProjectileType type,
	const sf::Vector2f& start,
	const sf::Vector2f& target,
	int damage)
{
	this->type = type;
	this->damage = damage;
	setPosition(start);
	activate();

	switch (type) {
	case ProjectileType::arrow:
		setTexture(ContentPipeline::getInstance().getArrowTexture(), true);
		sound.setBuffer(ContentPipeline::getInstance().getArcherAttackSoundBuffer());
		speed = 15.f * 60.f;
		break;
	case ProjectileType::blast:
		setTexture(ContentPipeline::getInstance().getBlastTexture(), true);
		sound.setBuffer(ContentPipeline::getInstance().getMageAttackSoundBuffer());
		speed = 8.f * 60.f;
		break;
	case ProjectileType::fireball:
		setTexture(ContentPipeline::getInstance().getFireballTexture(), true);
		sound.setBuffer(ContentPipeline::getInstance().getDemonAttackSoundBuffer());
		speed = 10.f * 60.f;
		break;
	}


	Vector2u size = getTexture()->getSize();
	setOrigin(size.x / 2.f, size.y / 2.f);

	FloatRect bounds = getGlobalBounds();
	float radius = bounds.width / 4.f;
	setCollisionCircleRadius(radius);

	sf::Vector2f dir = target - start;
	float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (dist > 0.f) {
		dir /= dist;
		velocity = dir * speed;
	}
	else {
		velocity = { 0.f,0.f };
	}

	float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
	setRotation(angle);
}


void Projectile::setTarget(Damageable* target)
{
	targetPtr = target;
}

void Projectile::update(float dt)
{
	if (!isActive()) return;

	if (targetPtr)
	{
		auto go = dynamic_cast<GameObject*>(targetPtr);
		if (!go || !go->isActive())
		{
			deactivate();
			return;
		}
		sf::Vector2f toTarget = go->getPosition() - getPosition();
		float dist2 = toTarget.x * toTarget.x + toTarget.y * toTarget.y;
		if (dist2 > 0.f)
		{
			float invLen = 1.f / std::sqrt(dist2);
			sf::Vector2f dir = toTarget * invLen;
			velocity = dir * speed;
			setRotation(std::atan2(dir.y, dir.x) * 180.f / PI);
		}
	}

	move(velocity * dt);
	if (isOffscreen()) deactivate();
}



void Projectile::applyDamage(Damageable* target)
{
	if (target && isActive()) {
		sound.play();
		target->takeDamage(damage);
		deactivate();
	}
}

bool Projectile::isOffscreen() const
{
	Vector2f pos = getPosition();
	return (pos.x < 0.f || pos.x > SCREEN_WIDTH ||
		pos.y < 0.f || pos.y > SCREEN_HEIGHT);
}
