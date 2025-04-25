#pragma once
#include "Scene.h"

using namespace sf;

class SceneTitle : public Scene
{
public:
	SceneTitle(RenderWindow& renderWindow, Event& event);
	scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;

	Sprite titleScreen;
	Text instructions;
	Music themeSong;
};