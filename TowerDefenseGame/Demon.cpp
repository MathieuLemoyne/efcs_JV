#include "Demon.h"
#include <cmath>
#include <iostream>

Demon::Demon() : waveNumber(1), speed(90.f + 10.f * waveNumber), currentWaypoint(nullptr)
{
	init();
}

Demon::Demon(int waveNumber, const Vector2f& spawnPosition)
	: waveNumber(waveNumber), speed(90.f + 10.f * waveNumber), currentWaypoint(nullptr)
{
	setPosition(spawnPosition);
	init();
}

void Demon::init()
{
	// Configure la texture sprite
	setTexture(ContentPipeline::getInstance().getDemonTexture());

	// Configure l'animation du sprite
	animationRect = IntRect(0, 0, 100, 50);
	setTextureRect(animationRect);
	setOrigin(animationRect.width / 2.f,
		animationRect.height / 2.f);
	animationTime = 0.f;
	animationSpeed = 0.1f;
	currentFrame = 0;

	// Configure barre de vie
    health = MAX_HEALTH;

    // Configure barre de vie
    float width = 100.f;
    float height = 6.f;

    // Configure les sprites des barres de vie
    healthBarBackground.setTexture(ContentPipeline::getInstance().getRedBarTexture());
    healthBar.setTexture(ContentPipeline::getInstance().getGreenBarTexture());

	// Configure la taille des barres de vie
    sf::Vector2u bgSize = healthBarBackground.getTexture()->getSize();
    healthBarBackground.setOrigin(bgSize.x / 2.f, bgSize.y / 2.f);

    sf::Vector2u fgSize = healthBar.getTexture()->getSize();
    healthBar.setOrigin(fgSize.x / 2.f, fgSize.y / 2.f);

    activate();
}

void Demon::update(float deltaTime)
{
    if (!isDemonAlive()) return;

    // Mettre à jour la barre de vie
    Vector2f spritePos = getPosition();
    healthBarBackground.setPosition(spritePos.x, spritePos.y - 40.f);
    healthBar.setPosition(spritePos.x, spritePos.y - 40.f);

    // Calculer l’échelle horizontale selon la vie restante
    float healthPercent = static_cast<float>(health) / MAX_HEALTH;
    healthBar.setScale(healthPercent, 1.f);

    float offsetX = (1.f - healthPercent) * (BAR_WIDTH / 2.f);
    healthBar.setPosition(spritePos.x - offsetX, spritePos.y - 40.f);

    // Mettre à jour l'animation
    animationTime += deltaTime;
    if (animationTime >= animationSpeed)
    {
        animationTime = 0.f;
        currentFrame = (currentFrame + 1) % 4;
        animationRect.left = currentFrame * 100;
        animationRect.top = 0;
        setTextureRect(animationRect);
    }

    // Mettre à jour la position du démon
    if (state == DemonState::Moving && currentWaypoint)
    {
        Vector2f pos = getPosition();
        Vector2f target = currentWaypoint->getPosition();
        Vector2f dir = target - pos;
        float   dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < 5.f)
        {
            Waypoint* next = currentWaypoint->getNext();
            if (next)
            {
                currentWaypoint = next;
            }
            else
            {
                state = DemonState::AtEnd;
                currentWaypoint = nullptr;
            }
        }
        else
        {
            dir /= dist;

            if (dir.x < 0.f)
                setScale(-1.f, 1.f);
            else
                setScale(1.f, 1.f);

            move(dir * speed * deltaTime);
        }
    }
}

void Demon::setFirstWaypoint(Waypoint* first)
{
	currentWaypoint = first;
}

bool Demon::isDemonAlive() const
{
	return isActive();
}

void Demon::reset(int waveNumber, const Vector2f& spawnPosition)
{
    this->waveNumber = waveNumber;
    this->speed = 90.f + 10.f * waveNumber;
    this->health = MAX_HEALTH;
    this->currentWaypoint = nullptr;
    this->state = DemonState::Moving;
    this->setPosition(spawnPosition);

    animationTime = 0.f;
    currentFrame = 0;
}

void Demon::shoot()
{

}

bool Demon::canAttack()
{
    return false;
}


void Demon::draw(RenderWindow& window)
{
	window.draw(healthBarBackground);
    window.draw(healthBar);
	window.draw(*this);
}
