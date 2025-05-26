#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Game
{
public:
	Game();
	int run();

private:
	//Ces deux attributs viennent de Windows
	//il faut les garder unique et constant durant tout le jeu
	//et les passer aux différentes scènes.
	RenderWindow renderWindow;
	Event event;

	Image icon;

	bool victory;
	int savedScore = 0;
};