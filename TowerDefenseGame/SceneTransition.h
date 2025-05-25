#pragma once
#include "Scene.h"

using namespace sf;

class SceneTransition : public Scene
{
public:
	SceneTransition(RenderWindow& renderWindow, Event& event);
	scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;

	Font font;
	Text message;

	int phase = 0;

	Clock timer;
};