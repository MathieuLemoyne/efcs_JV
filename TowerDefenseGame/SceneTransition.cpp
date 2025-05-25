#include "SceneTransition.h"
#include "ContentPipeline.h"

SceneTransition::SceneTransition(RenderWindow& renderWindow, Event& event) : Scene(renderWindow, event)
{
	view = renderWindow.getDefaultView();
}

Scene::scenes SceneTransition::run()
{
	while (isRunning)
	{
		calculateDeltaTime();
		getInputs();
		update();
		draw();
	}

	return transitionToScene;
}

bool SceneTransition::init()
{
    if (phase == 0) {
        message.setString(
            "Minions of the Dark Lord march on our lands.\n\n"
            "             Defend the King's Tower!\n\n"
            "                           Wave 1"
        );
    }
    else if (phase == 1) {
        message.setString("Wave 2");
    }
    //else {
    //    message.setString("End of Game");
    //}

    message.setFont(ContentPipeline::getInstance().getComiciFont());
    message.setCharacterSize(80);
    message.setFillColor(Color::White);
    message.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    message.setOrigin(
        message.getGlobalBounds().width / 2,
        message.getGlobalBounds().height / 2
    );

    timer.restart();
    return true;
}


void SceneTransition::getInputs()
{
	//On passe l'événement en référence et celui-ci est chargé du dernier événement reçu!
	while (renderWindow.pollEvent(event))
	{
		//x sur la fenêtre
		if (event.type == Event::Closed) exitGame();
	}
}

void SceneTransition::update()
{
    if (timer.getElapsedTime().asSeconds() >= 3.f)
    {
        isRunning = false;
        if (phase == 0)
            transitionToScene = scenes::LEVEL1;
        else if (phase == 1)
            transitionToScene = scenes::LEVEL2;
        /*else
            transitionToScene = scenes::END;*/
        phase++;
    }
}

void SceneTransition::draw()
{
	renderWindow.clear();
	renderWindow.draw(message);
	renderWindow.display();
}

bool SceneTransition::unload()
{
	return true;
}