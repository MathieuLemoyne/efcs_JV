#include "MageTower.h"
#include "ContentPipeline.h"
#include <cstdlib>

bool MageTower::init()
{
    Tower::init(false);
    setTexture(ContentPipeline::getInstance().getMageTowerTexture());
    setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    setOrigin(frameWidth / 2, frameHeight - 45);
    setCollisionCircleRadius(getGlobalBounds().width / 4.f);

    attackCooldown = 1.5f;
    barOffsetY = 95.f;
    activate();
    return true;
}

bool MageTower::shoot()
{
    if (fireReady)
    {
        fireReady = false;
        return true;
    }

    if (state == Idle && timeSinceLastAttack >= attackCooldown)
    {
        state = Animating;
        animationTime = 0.f;
        currentFrame = 0;
    }
    return false;
}

void MageTower::update(float deltaTime)
{
    Tower::update(deltaTime);

    if (state == Animating)
    {
        animationTime += deltaTime;
        if (animationTime >= frameDuration)
        {
            animationTime -= frameDuration;
            ++currentFrame;

            if (currentFrame < frameCount)
            {
                setTextureRect(
                    sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight)
                );
            }
            else
            {
                state = Idle;
                currentFrame = 0;
                setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

                Tower::shoot();
                fireReady = true;
            }
        }
    }
}

void MageTower::draw(sf::RenderWindow& window)
{
    if (!isAlive()) return;
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(*this);
}

int MageTower::getDamage() const
{
    return 1 + (std::rand() % 12);
}
