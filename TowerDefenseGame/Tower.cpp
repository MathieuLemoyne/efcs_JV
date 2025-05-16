#include "Tower.h"

Tower::Tower() {}

bool Tower::canAttack() const {
	return timeSinceLastAttack >= attackCooldown;
}

void Tower::shoot(Damageable* target) {
	if (!canAttack() || !target) return;

	target->takeDamage(getDamage());
	timeSinceLastAttack = 0.f;
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

bool Tower::isAlive() const {
	return health > 0;
}
