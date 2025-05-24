#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

/*
À NOTER: La couleur Gold pour le texte de "l'action active" : Color(212, 175, 55, 255)
*/
class Hud
{
public:
	Hud() {}
	void hudInit(const Texture& hudMaskTexture, const Font& font);
	void draw(sf::RenderWindow& renderWindow);

	void updateHud(int mana, int score, int kills, int wave, int highScore, String action);


private:
	static const int INSTRUCTIONS_NUMBER = 6;
	static const int TEXT_POSITION_X = 1250;

	bool endGame = false;

	sf::Text specialStateText;

	sf::Text manaText;
	sf::Text scoreText;
	sf::Text killsText;

	sf::Text instructionTexts[INSTRUCTIONS_NUMBER];

	sf::Text highScoreText;
	sf::Text waveText;

	Sprite hudMask;
};
