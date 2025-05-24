#pragma once

#include "Constants.h"
#include "GameObject.h"
#include "Damageable.h"
#include "Demon.h"
#include <SFML/Graphics.hpp>

/*
Metrics du Projectile (à effacer à la fin)

- Vitesse de déplacement (framerate 60 fps): Arrow: 15, blast 8, fireball: 10
(Si utilisation du DeltaTime, faire x60 à ces valeurs)

- Dommages: les projectiles font les dommages random suivants
  Arrow: 1 à 4, blast 1 à 12, fireball: 1 à 10

- Cadence de tir: un tir à tous les
  Arrow: 48 rafraichissements / 0.8 secondes
  Blast: 90 rafraichissements / 1.5 secondes
  Fireball: 63 - 3 * wave / 1.05 secondes - 0.05 * wave
*/

class Projectile : public GameObject {
public:
    Projectile();

    void init(ProjectileType type,
              const sf::Vector2f& start,
              const sf::Vector2f& target,
              int damage);

    void setTarget(Demon* target);

    void update(float deltaTime);
    void applyDamage(Damageable* target);

    bool isOffscreen() const;

private:
    ProjectileType type;
    Demon* targetPtr = nullptr;
    sf::Vector2f velocity;
    float speed;
    int damage;
};
