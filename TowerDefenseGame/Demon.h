#pragma once
#include "Shooter.h"
#include "Waypoint.h"
#include "ContentPipeline.h"
#include "Subject.h"

//#include <SFML/Graphics.hpp>

/*
Metrics du Demon (à effacer à la fin)
- Rectangle d'animation: 100 X 50
- Rayon du cercle de collision: hauteur du rectangle d'animation (50) divisé par 2
- Vitesse d'animation: 1 frame par 0.1 /secs
- Vitesse de déplacement: 0.9 + 0.1 X Numéro de vague (Si utilisation du DeltaTime, faire x60)
- Points de vie: 60 (50 pour la version à un développeur);
- Cadence de tir: un tir par (1.05 - 0.05 * numéro de vague) seconde.
- Portée du tir: 250
- Position de départ des démons au niveau 1: 610, -100
- Position de départ des démons au niveau 2: -100, 410
*/

enum class DemonState {
    Moving,
    AtEnd,
    Dying
};

class Demon : Shooter, Subject
{
public:
    Demon();
    Demon(int waveNumber, const Vector2f& spawnPosition);

    void init();

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    void setFirstWaypoint(Waypoint* first);
    bool isDemonAlive() const;

    DemonState state = DemonState::Moving;

    void reset(int waveNumber, const Vector2f& spawnPosition);

    void shoot() override;
    bool canAttack();

    void startDyingAnimation();

private:
	// Variables de la classe
    float speed;
    int waveNumber;
    int health;

	// Rectangle d'animation
    IntRect animationRect;
    float animationTime = 0.f;
    float animationSpeed = 0.1f;
    int currentFrame = 0;

	// Cadence de tir
    float attackCooldown;
    float attackRange = 250.f;
    float timeSinceLastAttack = 0.f;


    Waypoint* currentWaypoint;

	// Health bar
	Sprite healthBar;
	Sprite healthBarBackground;

    const int MAX_HEALTH = 60;
    const float BAR_WIDTH = 60;
};
