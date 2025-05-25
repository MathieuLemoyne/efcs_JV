#include "SceneTransition.h"
#include "ContentPipeline.h"

int SceneTransition::phase = 0;

SceneTransition::SceneTransition(RenderWindow& renderWindow, Event& event)
    : Scene(renderWindow, event)
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
    if (phase == 0)
    {
        message.setString(
            "Minions of the Dark Lord march on our lands.\n\n"
            "             Defend the King's Tower!\n\n"
            "                           Wave 1"
        );
    }
    else if (phase == 1)
    {
        message.setString("Wave 2");
    }

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
    while (renderWindow.pollEvent(event))
    {
        if (event.type == Event::Closed) exitGame();
    }
}

void SceneTransition::update()
{
    if (timer.getElapsedTime().asSeconds() >= 3.f)
    {
        isRunning = false;

        if (phase == 0)
        {
            transitionToScene = scenes::LEVEL1;
        }
        else if (phase == 1)
        {
            transitionToScene = scenes::LEVEL2;
        }
        ++phase;
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
