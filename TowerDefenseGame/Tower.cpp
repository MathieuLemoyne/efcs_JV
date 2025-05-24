#include "Tower.h"

Tower::Tower() {}

void Tower::init(bool isKing)
{
    maxHealth = isKing ? 500 : 250;
    health = maxHealth;
    healthBarBackground.setTexture(ContentPipeline::getInstance().getRedBarTexture());
    healthBar.setTexture(ContentPipeline::getInstance().getGreenBarTexture());
    auto bgSize = healthBarBackground.getTexture()->getSize();
    auto fgSize = healthBar.getTexture()->getSize();
    healthBarBackground.setOrigin(bgSize.x / 2.f, bgSize.y / 2.f);
    healthBar.setOrigin(fgSize.x / 2.f, fgSize.y / 2.f);
}

void Tower::update(float deltaTime)
{
    timeSinceLastAttack += deltaTime;

    sf::Vector2f pos = getPosition();
    float pct = float(health) / float(maxHealth);
    float offsetX = (1.f - pct) * (BAR_WIDTH / 2.f);

    healthBarBackground.setPosition(pos.x, pos.y - barOffsetY);
    healthBar.setScale(pct, 1.f);
    healthBar.setPosition(pos.x - offsetX, pos.y - barOffsetY);
}
bool Tower::canAttack() const {
    if (!isAlive()) return false;
	return timeSinceLastAttack >= attackCooldown;
}

bool Tower::shoot() {
	if (!canAttack()) return false;
	timeSinceLastAttack = 0.f;
	return true;
}

bool Tower::isAlive() const {
    return health > 0;
}

int Tower::getDamage() const {
	return damage;
}

float Tower::getAttackFrequency() const {
	return attackCooldown;
}

float Tower::getAttackRange() const {
	return attackRange;
}

void Tower::takeDamage(int amount) {
	health -= amount;
	if (health < 0) health = 0;
}
