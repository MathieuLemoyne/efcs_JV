#include "SceneGame.h"
#include "ContentPipeline.h"
#include "Waypoint.h"
#include "KingTower.h"	
#include <iostream> 

SceneGame::SceneGame(RenderWindow& renderWindow, Event& event) : Scene(renderWindow, event)
{
	view = renderWindow.getDefaultView();
}

Scene::scenes SceneGame::run()
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

bool SceneGame::init()
{
	map.setTexture(ContentPipeline::getInstance().getMapTexture(Maps::map1));

	hud.hudInit(ContentPipeline::getInstance().getHudmaskTexture(), ContentPipeline::getInstance().getComiciFont());


	Waypoint* previous = nullptr;
	for (const auto& pos : WAYPOINTS_MAP1) {
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();

		if (waypointCount > 0)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);

		waypointCount++;
	}


	kingTower.init();

	if (waypointCount > 0) {
		demons[0] = Demon(1, Vector2f(610.f, -100.f));
		demons[0].setFirstWaypoint(&waypoints[0]);
		spawnedDemons = 1;
	}

	nextSpawnTime = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;
	

	return true;
}

void SceneGame::getInputs()
{
	//On passe l'événement en référence et celui-ci est chargé du dernier événement reçu!
	while (renderWindow.pollEvent(event))
	{
		//x sur la fenêtre
		if (event.type == Event::Closed) exitGame();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
			std::cout << "[DEBUG] Touche W pressée " << std::endl;
			showWaypoints = !showWaypoints;
		}
	}
}

void SceneGame::update()
{
	spawnTimer += deltaTime;

	if (spawnTimer >= nextSpawnTime && spawnedDemons < MAX_DEMONS)
	{
		spawnTimer = 0.f;
		nextSpawnTime = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;

		demons[spawnedDemons] = Demon(1, Vector2f(610.f, -100.f));
		demons[spawnedDemons].setFirstWaypoint(&waypoints[0]);
		spawnedDemons++;
	}

	for (int i = 0; i < spawnedDemons; ++i)
	{
		demons[i].update(deltaTime);
	}
}

void SceneGame::draw()
{
	//Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.draw(map);
	kingTower.draw(renderWindow);

	if (showWaypoints) {
		for (int i = 0; i < waypointCount; ++i)
			waypoints[i].draw(renderWindow);
	}

	for (int i = 0; i < spawnedDemons; ++i) {
		demons[i].draw(renderWindow);
	}

	hud.draw(renderWindow);
	renderWindow.display();
}

bool SceneGame::unload()
{
	return true;
}