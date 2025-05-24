#include "Demon.h"
#include <cmath>
#include <iostream>

Demon::Demon() : waveNumber(1), speed(90.f + 10.f * waveNumber) {
    init();
}

Demon::Demon(int waveNumber, const Vector2f& spawnPosition)
    : waveNumber(waveNumber), speed(90.f + 10.f * waveNumber) {
    setPosition(spawnPosition);
    init();
}

void Demon::init() {
    setTexture(ContentPipeline::getInstance().getDemonTexture());

    animationRect = IntRect(0, 0, 100, 50);
    setTextureRect(animationRect);
    setOrigin(animationRect.width / 2.f, animationRect.height / 2.f);
    animationTime = 0.f;
    currentFrame = 0;

    health = MAX_HEALTH;
    healthBarBackground.setTexture(ContentPipeline::getInstance().getRedBarTexture());
    healthBar.setTexture(ContentPipeline::getInstance().getGreenBarTexture());

    Vector2u bgSize = healthBarBackground.getTexture()->getSize();
    Vector2u fgSize = healthBar.getTexture()->getSize();
    healthBarBackground.setOrigin(bgSize.x / 2.f, bgSize.y / 2.f);
    healthBar.setOrigin(fgSize.x / 2.f, fgSize.y / 2.f);

    attackCooldown = 1.05f - 0.05f * waveNumber;
    timeSinceLastAttack = 0.f;

    FloatRect bounds = getGlobalBounds();
    float radius = bounds.width / 4.f;
    setCollisionCircleRadius(radius);

    activate();
}

void Demon::reset(int waveNumber, const Vector2f& spawnPosition) {
    this->waveNumber = waveNumber;
    this->speed = 90.f + 10.f * waveNumber;
    this->health = MAX_HEALTH;
    this->state = DemonState::Moving;
    this->currentWaypoint = nullptr;
    this->setPosition(spawnPosition);
    this->animationTime = 0.f;
    this->currentFrame = 0;
}

void Demon::update(float deltaTime) {
    if (!isAlive()) return;

    timeSinceLastAttack += deltaTime;

    if (state == DemonState::Dying) {
        updateDyingAnimation(deltaTime);
        return;
    }

    updateHealthBar();
    updateAnimation(deltaTime);
    updateMovement(deltaTime);


}

void Demon::draw(RenderWindow& window) {
    if (!isAlive()) return;
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(*this);
}

bool Demon::canAttack() const {
    return timeSinceLastAttack >= attackCooldown;
}

bool Demon::shoot() {
    if (!canAttack()) return false;
    timeSinceLastAttack = 0.f;
    return true;
}


int Demon::getDamage() const {
    return 5 + waveNumber;
}

float Demon::getAttackFrequency() const {
    return attackCooldown;
}

float Demon::getAttackRange() const {
    return attackRange;
}

void Demon::takeDamage(int amount) {
    health -= amount;
    if (health <= 0 && state != DemonState::Dying) {
        health = 0;
    
        startDyingAnimation();
    }
}

void Demon::startDyingAnimation() {
    state = DemonState::Dying;
    currentFrame = 0;
    animationTime = 0.f;
    animationRect = IntRect(0, 50, 100, 50);
    setTextureRect(animationRect);
}

void Demon::updateDyingAnimation(float deltaTime) {
    animationTime += deltaTime;
    if (animationTime >= animationSpeed) {
        animationTime = 0.f;
        currentFrame++;

        if (currentFrame < 5) {
            animationRect.left = currentFrame * 100;
            setTextureRect(animationRect);
        }
        else {
            deactivate();
        }
    }
}

void Demon::updateAnimation(float deltaTime) {
    animationTime += deltaTime;
    if (animationTime >= animationSpeed) {
        animationTime = 0.f;
        currentFrame = (currentFrame + 1) % 4;
        animationRect.left = currentFrame * 100;
        animationRect.top = 0;
        setTextureRect(animationRect);
    }
}

void Demon::updateHealthBar() {
    Vector2f spritePos = getPosition();
    float healthPercent = static_cast<float>(health) / MAX_HEALTH;
    float offsetX = (1.f - healthPercent) * (BAR_WIDTH / 2.f);

    healthBarBackground.setPosition(spritePos.x, spritePos.y - 40.f);
    healthBar.setScale(healthPercent, 1.f);
    healthBar.setPosition(spritePos.x - offsetX, spritePos.y - 40.f);
}

void Demon::updateMovement(float deltaTime) {
    if (!currentWaypoint) return;

    Vector2f pos = getPosition();
    Vector2f target = currentWaypoint->getPosition();
    Vector2f dir = target - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < 5.f) {
        Waypoint* next = currentWaypoint->getNext();
        currentWaypoint = next;
        if (!next) state = DemonState::AtEnd;
    }
    else {
        dir /= dist;
        setScale(dir.x < 0.f ? -1.f : 1.f, 1.f);
        move(dir * speed * deltaTime);
    }
}

void Demon::setFirstWaypoint(Waypoint* first) {
    currentWaypoint = first;
}

bool Demon::isAlive() const {
    return isActive();
}
