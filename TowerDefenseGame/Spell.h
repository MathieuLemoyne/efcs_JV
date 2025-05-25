#pragma once
#include "GameObject.h"
#include "Subject.h"
#include "Constants.h"

/*
Metrics du Spell (� effacer � la fin)
- Port�e des deux types de Spell: 400
- Dur�e de vie des deux spells: 5 secondes
- Vitesse de rotation:
  augmente de 

va de 0 � 20, progressivement en avancant dans le temps
- Ajoute 1/15 � la rotation � chaque rafraichissement d'�cran.
- Ajoute deltaTime / 15 � chaque update si on utilise le deltaTime

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
