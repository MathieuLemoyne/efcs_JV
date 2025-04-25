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
		Waypoint* wp = new Waypoint(pos);
		if (previous) previous->setNext(wp);
		previous = wp;
		waypoints.push_back(wp);
	}

	kingTower.init();

	return true;
}

void SceneGame::getInputs()
{
	//On passe l'�v�nement en r�f�rence et celui-ci est charg� du dernier �v�nement re�u!
	while (renderWindow.pollEvent(event))
	{
		//x sur la fen�tre
		if (event.type == Event::Closed) exitGame();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
			std::cout << "[DEBUG] Touche W press�e " << std::endl;
			showWaypoints = !showWaypoints;
		}
	}
}

void SceneGame::update()
{

}

void SceneGame::draw()
{
	//Toujours important d'effacer l'�cran pr�c�dent
	renderWindow.clear();
	renderWindow.draw(map);
	kingTower.draw(renderWindow);

	if (showWaypoints) {
		for (const auto& wp : waypoints) {
			wp->draw(renderWindow);

		}
	}
	hud.draw(renderWindow);
	renderWindow.display();
}

bool SceneGame::unload()
{
	return true;
}