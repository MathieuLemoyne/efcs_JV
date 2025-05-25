#include "SceneEnd.h"
#include "ContentPipeline.h"
#include "Constants.h"

SceneEnd::SceneEnd(RenderWindow& renderWindow, Event& event, bool victory)
    : Scene(renderWindow, event)
    , isVictory(victory)
{
    view = renderWindow.getDefaultView();
}

Scene::scenes SceneEnd::run()
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

bool SceneEnd::init()
{
    if (isVictory)
    {
        if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\VictoryTheme.ogg")) return false;
        endGameScreen.setTexture(ContentPipeline::getInstance().getVictoryScreenTexture());
    }
    else
    {
        if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\EndGameTheme.ogg")) return false;
        endGameScreen.setTexture(ContentPipeline::getInstance().getGameOverScreenTexture());
    }

    instructions[0].setFont(ContentPipeline::getInstance().getComiciFont());
    instructions[0].setCharacterSize(50);
    instructions[0].setOutlineThickness(4.f);

    if (isVictory)
    {
        instructions[0].setString("\nYou have defended the King's Tower!");
        instructions[0].setOutlineColor(Color(232, 161, 10, 255));
        instructions[0].setFillColor(Color::Yellow);
    }
    else
    {
        instructions[0].setString("\nThe King's Tower has fallen!");
        instructions[0].setOutlineColor(Color(93, 31, 4, 255));
        instructions[0].setFillColor(Color::Red);
    }

    for (int i = 1; i < INSTRUCTIONS_NUMBER; ++i)
        instructions[i] = instructions[0];

    instructions[1].setString("Press Enter to return to Title");
    instructions[2].setString("Press Escape to Exit");
    instructions[3].setString("");

    for (int i = 0; i < INSTRUCTIONS_NUMBER; ++i)
    {
        auto bounds = instructions[i].getLocalBounds();
        instructions[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        instructions[i].setPosition(SCREEN_WIDTH / 2.f, 200.f + i * 80.f);
    }
    themeSong.setLoop(true);
    themeSong.play();

    return true;
}

void SceneEnd::getInputs()
{
    while (renderWindow.pollEvent(event))
    {
        if (event.type == Event::Closed)
            exitGame();
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Enter)
            {
                isRunning = false;
                transitionToScene = scenes::TITLE;
            }
            else if (event.key.code == Keyboard::Escape)
            {
                exitGame();
            }
        }
    }
}

void SceneEnd::update() {}

void SceneEnd::draw()
{
    renderWindow.clear();
    renderWindow.draw(endGameScreen);
    for (int i = 0; i < INSTRUCTIONS_NUMBER; ++i)
        renderWindow.draw(instructions[i]);
    renderWindow.display();
}

bool SceneEnd::unload()
{
    themeSong.stop();
    return true;
}
