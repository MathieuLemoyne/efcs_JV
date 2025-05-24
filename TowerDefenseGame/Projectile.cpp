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
        setTexture(ContentPipeline::getInstance().getArrowTexture());
        speed = 15.f * 60.f;
        break;
    case ProjectileType::blast:
        setTexture(ContentPipeline::getInstance().getBlastTexture());
        speed = 8.f * 60.f;
        break;
    case ProjectileType::fireball:
        setTexture(ContentPipeline::getInstance().getFireballTexture());
        speed = 10.f * 60.f;
        break;
    }

    {
        Vector2u size = getTexture()->getSize();
        setOrigin(size.x / 2.f, size.y / 2.f);
    }

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


void Projectile::setTarget(Demon* target)
{
    targetPtr = target;
}

void Projectile::update(float deltaTime)
{
    if (!isActive())
        return;

    if (targetPtr && !targetPtr->isActive()) {
        deactivate();
        return;
    }

    move(velocity * deltaTime);

    if (isOffscreen())
        deactivate();
}

void Projectile::applyDamage(Damageable* target)
{
    if (target && isActive()) {
        target->takeDamage(damage);
        deactivate();
    }
}

bool Projectile::isOffscreen() const
{
    auto pos = getPosition();
    return (pos.x < 0.f || pos.x > SCREEN_WIDTH ||
            pos.y < 0.f || pos.y > SCREEN_HEIGHT);
}
