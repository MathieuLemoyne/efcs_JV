#include "Hud.h"
#include "SceneGame.h"
void Hud::hudInit(const Texture& hudMaskTexture, const Font& font)
{
	hudMask.setTexture(hudMaskTexture);

	specialStateText.setFont(font);
	specialStateText.setCharacterSize(25);
	specialStateText.setFillColor(Color::White);
	specialStateText.setPosition(TEXT_POSITION_X, 10);
	specialStateText.setString("Instructions diverses ici!");

	//Utilisation sans honte du constructeur de copie
	manaText = specialStateText;
	scoreText = specialStateText;
	killsText = specialStateText;
	waveText = specialStateText;
	highScoreText = specialStateText;

	waveText.setPosition(TEXT_POSITION_X, 110);
	manaText.setPosition(TEXT_POSITION_X, 210);
	killsText.setPosition(TEXT_POSITION_X, 250);
	scoreText.setPosition(TEXT_POSITION_X, 290);
	highScoreText.setPosition(TEXT_POSITION_X, 330);


	waveText.setString("Wave - 1");
	manaText.setString("Mana - 0");
	killsText.setString("Kills - 0");
	scoreText.setString("Score - 0");
	highScoreText.setString("HighScore - 0");

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
	{
		instructionTexts[i] = specialStateText;
		instructionTexts[i].setPosition(TEXT_POSITION_X, 430 + i * 40);
	}

	instructionTexts[0].setStyle(Text::Underlined);
	instructionTexts[0].setString("Instructions");
	instructionTexts[1].setString("Z: Create archer tower (70 manas)");
	instructionTexts[2].setString("X: Create mage tower (100 manas)");
	instructionTexts[3].setString("A: Plague spell (20 manas)");
	instructionTexts[4].setString("S: Sacred light spell (60 manas)");
	instructionTexts[5].setString("P: Pause");

	specialStateText.setCharacterSize(35);
}

void Hud::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(hudMask);
	renderWindow.draw(specialStateText);
	renderWindow.draw(manaText);
	renderWindow.draw(scoreText);
	renderWindow.draw(killsText);
	renderWindow.draw(highScoreText);
	renderWindow.draw(waveText);
	renderWindow.draw(highScoreText);

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
		renderWindow.draw(instructionTexts[i]);
}

void Hud::updateHud(int mana, int score, int kills, int wave, int highScore, String action)
{
	manaText.setString("Mana - " + std::to_string(mana));
	scoreText.setString("Score - " + std::to_string(score));
	killsText.setString("Kills - " + std::to_string(kills));
	waveText.setString("Wave - " + std::to_string(wave));
	highScoreText.setString("HighScore - " + std::to_string(highScore));

	for (int i = 1; i < INSTRUCTIONS_NUMBER; ++i)
	{
		instructionTexts[i].setFillColor(sf::Color::White);
	}

	if (action == "CreateArcherTower")
	{
		instructionTexts[1].setFillColor(sf::Color(212, 175, 55, 255));
		specialStateText.setString("Placing an Archer Tower");
		specialStateText.setFillColor(sf::Color::Yellow);
	}
	else if (action == "CreateMageTower")
	{
		instructionTexts[2].setFillColor(sf::Color(212, 175, 55, 255));
		specialStateText.setString("Placing a Mage Tower");
		specialStateText.setFillColor(sf::Color::Yellow);
	}
	else if (action == "PlagueSpell")
	{
		instructionTexts[3].setFillColor(sf::Color(212, 175, 55, 255));
		specialStateText.setString("Casting Plague Spell");
		specialStateText.setFillColor(sf::Color::Yellow);
	}
	else if (action == "SacredLight")
	{
		instructionTexts[4].setFillColor(sf::Color(212, 175, 55, 255));
		specialStateText.setString("Casting Sacred Light");
		specialStateText.setFillColor(sf::Color::Yellow);
	}
	else if (action == "Pause")
	{
		instructionTexts[5].setFillColor(sf::Color(212, 175, 55, 255));
		specialStateText.setString("Game Paused");
		specialStateText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		specialStateText.setString("Instructions diverses ici!");
		specialStateText.setFillColor(sf::Color::White);
	}
}
