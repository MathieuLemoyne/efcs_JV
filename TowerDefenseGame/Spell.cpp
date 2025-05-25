#include "Spell.h"
#include "ContentPipeline.h"
#include "Constants.h"


Spell::Spell()
{
    duration = 5.f;
    lifeTime = 0.f;
    rotationSpeed = 0.f;
    setOrigin(50.f, 50.f);
}

bool Spell::init()
{
    return true;
}

void Spell::activateSpell(const sf::Vector2f& position, SpellType type)
{
    this->type = type;
    setPosition(position);

    switch (type)
    {
    case plague:
        setTexture(ContentPipeline::getInstance().getPlagueTexture());
        setColor(sf::Color(96, 241, 76, 255));
        break;
    case sacredLight:
        setTexture(ContentPipeline::getInstance().getSacredLightTexture());
        setColor(sf::Color(214, 172, 2, 255));
        break;
    }

    lifeTime = 0.f;
    rotationSpeed = 0.f;
    activate();
	notifyAllObservers();
}

void Spell::update(float deltaTime)
{
    if (!isActive()) return;

    lifeTime += deltaTime;
    rotationSpeed += deltaTime / 15.f;
    if (rotationSpeed > 20.f)
        rotationSpeed = 20.f;

    rotate(rotationSpeed);

    if (lifeTime >= duration)
        deactivate();
}

SpellType Spell::getType()
{
    return this->type;
}

