#include "SceneTitle.h"
#include "ContentPipeline.h"

SceneTitle::SceneTitle(RenderWindow& renderWindow, Event& event) : Scene(renderWindow, event)
{
	view = renderWindow.getDefaultView();
}

Scene::scenes SceneTitle::run()
{
	while (isRunning)
	{
		calculateDeltaTime();
		getInputs();
		update();
		draw();
	}

	if (!unload()) return scenes::FAIL;

	return transitionToScene;
}

bool SceneTitle::init()
{
	if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\TitleTheme.ogg")) return false;

	titleScreen.setTexture(ContentPipeline::getInstance().getTitleScreenTexture());

	instructions.setString("Press Enter to play");
	instructions.setFont(ContentPipeline::getInstance().getComiciFont());
	instructions.setCharacterSize(70);
	instructions.setOutlineColor(Color::Black);
	instructions.setFillColor(Color(186, 62, 8, 255));
	instructions.setOutlineThickness(4.0f);
	instructions.setOrigin(instructions.getGlobalBounds().width / 2.0f, instructions.getGlobalBounds().height / 2.0f);
	instructions.setPosition(SCREEN_WIDTH / 2, 550);

	themeSong.setLoop(true);
	themeSong.play();
	return true;
}

void SceneTitle::getInputs()
{
	while (renderWindow.pollEvent(event))
	{
		if (event.type == Event::Closed)
			exitGame();

		if (event.type == Event::KeyPressed &&
			event.key.code == Keyboard::Enter)
		{
			isRunning = false;
			transitionToScene = scenes::TRANSITION;
		}
	}
}

void SceneTitle::update()
{
}

void SceneTitle::draw()
{
	renderWindow.clear();
	renderWindow.draw(titleScreen);
	renderWindow.draw(instructions);
	renderWindow.display();
}

bool SceneTitle::unload()
{
	//themeSong.stop();
	return true;
}