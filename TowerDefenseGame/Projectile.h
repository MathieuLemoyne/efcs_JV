#pragma once

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

class Projectile
{
public:
	Projectile();

private:
};

