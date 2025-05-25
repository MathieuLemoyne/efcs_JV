#include "Tower.h"
#include "Spell.h"
#include <iostream>

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
	if (sacredLightTimer > 0.f) {
		sacredLightTimer -= deltaTime;
		if (sacredLightTimer <= 0.f) {
			sacredLightAttackSpeedMultiplier = 1.f;
		}
	}

	if (plagueTimer > 0.f) {
		plagueTimer -= deltaTime;
		if (plagueTimer <= 0.f) {
			plagueDamageMultiplier = 1.f;
		}
	}
	if (!isAffectedBySpell()) {
		sacredLightAttackSpeedMultiplier = 1.f;
		plagueDamageMultiplier = 1.f;
	}
	timeSinceLastAttack += deltaTime;

	sf::Vector2f pos = getPosition();
	float pct = float(health) / float(maxHealth);
	float offsetX = (1.f - pct) * (BAR_WIDTH / 2.f);

	healthBarBackground.setPosition(pos.x, pos.y - barOffsetY);
	healthBar.setScale(pct, 1.f);
	healthBar.setPosition(pos.x - offsetX, pos.y - barOffsetY);

	if (!isAffectedBySpell()) {
		setColor(sf::Color::White);
	}
}
bool Tower::canAttack() const {
	if (!isAlive()) return false;
	return timeSinceLastAttack >= (attackCooldown / sacredLightAttackSpeedMultiplier);
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
	health -= amount * plagueDamageMultiplier;
	if (health < 0)
	{
		health = 0;
		deactivate();
	}

}
void Tower::heal(int amount) {
	health += amount;
	if (health > maxHealth) health = maxHealth;
}

bool Tower::isAffectedBySpell() const {
	return sacredLightTimer > 0.f || plagueTimer > 0.f;
}

void Tower::notify(Subject* subject) {
	Spell* spell = dynamic_cast<Spell*>(subject);
	if (!spell || !isActive()) return;
	sf::Vector2f posA = getPosition();
	sf::Vector2f posB = spell->getPosition();

	float dx = posB.x - posA.x;
	float dy = posB.y - posA.y;

	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance > spell->getRange()) return;

	switch (spell->getType()) {
	case SpellType::sacredLight:
	{
		int heal = rand() % 10 + 6; // [6, 15]
		this->heal(heal);
		sacredLightAttackSpeedMultiplier = 2.f;
		std::cout << "[DEBUG] Tower healed by " << heal << " points.\n";
		sacredLightTimer = 5.f;
		setColor(Color(214, 172, 2, 255));
	}
	break;
	case SpellType::plague:
	{
		int damage = 1 + (std::rand() % 10);
		takeDamage(damage);
		plagueDamageMultiplier = 2.f;
		plagueTimer = 5.f;
		setColor(Color(96, 241, 76, 255));
	}
	break;
	default:
		break;
	}
}
