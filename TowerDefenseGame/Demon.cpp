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
	health = 60;
	activate();
	setTexture(ContentPipeline::getInstance().getDemonTexture());

	animationRect = IntRect(0, 0, 100, 50);
	setTextureRect(animationRect);
	setOrigin(animationRect.width / 2.f,
		animationRect.height / 2.f);
	animationTime = 0.f;
	animationSpeed = 0.1f;
	currentFrame = 0;
}

void Demon::setFirstWaypoint(Waypoint* first)
{
	currentWaypoint = first;
}

bool Demon::isDemonAlive() const
{
	return isActive();
}

void Demon::update(float deltaTime)
{
    if (!isDemonAlive()) return;

    animationTime += deltaTime;
    if (animationTime >= animationSpeed)
    {
        animationTime = 0.f;
        currentFrame = (currentFrame + 1) % 4;
        animationRect.left = currentFrame * 100;
        animationRect.top = 0;
        setTextureRect(animationRect);
    }

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
            move(dir * speed * deltaTime);
        }
    }
}


void Demon::draw(RenderWindow& window)
{
	window.draw(*this);
}
