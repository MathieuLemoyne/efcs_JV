#pragma once
#include <SFML/Graphics.hpp>

struct Inputs
{
	Inputs()
	{
		reset();
	}

	void reset()
	{
		mousePosition = Vector2f(0.0f, 0.0f);
		mouseLeftButtonClicked = false;
		keyPPressed = false;
		keyZPressed = false;
		keyXPressed = false;
	}

	Vector2f mousePosition;
	bool mouseLeftButtonClicked;
	bool keyPPressed;
	bool keyZPressed;
	bool keyXPressed;
};
