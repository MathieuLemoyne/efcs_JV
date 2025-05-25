#pragma once
#include "GameObject.h"
#include "Subject.h"
#include "Constants.h"

/*
Metrics du Spell (à effacer à la fin)
- Portée des deux types de Spell: 400
- Durée de vie des deux spells: 5 secondes
- Vitesse de rotation:
  augmente de 

va de 0 à 20, progressivement en avancant dans le temps
- Ajoute 1/15 à la rotation à chaque rafraichissement d'écran.
- Ajoute deltaTime / 15 à chaque update si on utilise le deltaTime

- Couleur transmise par la peste: Color(96, 241, 76, 255);
- Couleur transmise par la sacred light: Color(214, 172, 2, 255);

*/

class Spell : public GameObject, public Subject
{
public:
    Spell();
    bool init();
    void activateSpell(const sf::Vector2f& position, SpellType type);
    void update(float deltaTime);
    SpellType getType();
    float getDuration() const { return duration; }
    float getRange() const { return 400.f; }
private:
    float lifeTime;
    float duration;
    float rotationSpeed;
	SpellType type;
    Sprite rune;

};
