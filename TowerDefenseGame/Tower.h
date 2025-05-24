#pragma once
#include "GameObject.h"
#include "Shooter.h"
#include "Damageable.h"
#include "ContentPipeline.h"
/*
Metrics des tours  (à effacer à la fin)

- Rayon du cercle de collision: taille en X du sprite ou du rectangle d'animation divisé par 4
- Points de vie: Tour du roi: 500, autres: 250.
- Cadence de tir: tour d'archer: 0.8 secondes entre les tirs (48 rafraichissements d'écran à 60 fps), tour de mage, 1.5 seconde et demi (90 rafraichissements)
- Portée de tir: 300 chacunes

- Rectangle d'animation de la tour du mage: 150 X 250
- Vitesse d'animation de la tour du mage: 0.15 Secs
- L'image 0 est l'image idle. Quand on initie le tir, on passe tout de suite à l'image 1.
*/


class Tower : public GameObject, public Shooter, public Damageable
{
public:
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update(float deltaTime);

	Tower();

	void init(bool isKing = false);


	bool canAttack() const override;
	bool shoot() override;
	void takeDamage(int amount) override;

	float getAttackRange() const;
	virtual int getDamage() const override;
	float getAttackFrequency() const override;

	bool isAlive() const override;

protected:
	int maxHealth = 0;
	int health = 250;
	int damage = 10;
	float attackRange = 300.f;
	float attackCooldown = 1.f;
	float timeSinceLastAttack = 0.f;

	Sprite healthBar;
	Sprite healthBarBackground;

	const int MAX_HEALTH = 250;
	const float BAR_WIDTH = 60;
	float barOffsetY = 70.f;
};
