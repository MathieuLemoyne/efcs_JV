#include "SceneGame.h"
#include "ContentPipeline.h"
#include "Waypoint.h"
#include "KingTower.h"	
#include <iostream> 
#include "TowerEmplacement.h"
#include "ArcherTower.h"
#include "MageTower.h"

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
	currentAction = ActionMode::None;
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
	for (const auto& pos : EMPLACEMENTS_MAP1) {
		towerEmplacements[emplacementCount].setPosition(pos);
		towerEmplacements[emplacementCount].activate();
		towerEmplacements[emplacementCount].init();

		emplacementCount++;
	}

	kingTower.init();

	if (waypointCount > 0) {
		demons[0].setFirstWaypoint(&waypoints[0]);
		spawnedDemons = 1;
	}

	for (int i = 0; i < MAX_DEMONS; ++i) {
		demons[i].reset(1, Vector2f(610.f, -100.f));
	}
	spawnedDemons = 0;


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

		if (event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			Vector2f mousePos = renderWindow.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
			for (int i = 0; i < emplacementCount; ++i) {
				if (towerEmplacements[i].isMouseOver(mousePos)) {
					createTower(towerEmplacements[i].getPosition());
					towerEmplacements[i].deactivate();
				}
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Z:
				currentAction = ActionMode::CreateArcherTower;
				std::cout << "[DEBUG] ActionMode: Archer Tower\n";
				break;
			case sf::Keyboard::X:
				currentAction = ActionMode::CreateMageTower;
				std::cout << "[DEBUG] ActionMode: Mage Tower\n";
				break;
			case sf::Keyboard::A:
				currentAction = ActionMode::PlagueSpell;
				std::cout << "[DEBUG] ActionMode: Plague Spell\n";
				break;
			case sf::Keyboard::S:
				currentAction = ActionMode::SacredLight;
				std::cout << "[DEBUG] ActionMode: Sacred Light\n";
				break;
			case sf::Keyboard::P:
				currentAction = ActionMode::Pause;
				std::cout << "[DEBUG] ActionMode: Pause\n";
				break;
			default:
				break;
			}
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

		demons[spawnedDemons].setFirstWaypoint(&waypoints[0]);
		spawnedDemons++;
	}

	// Mise à jour des démons
	for (int i = 0; i < spawnedDemons; ++i)
	{
		Demon& demon = demons[i];
		demon.update(deltaTime);

		if (!demon.canAttack()) continue;

		Tower* closestTower = nullptr;
		float closestDist = demon.getAttackRange();

		for (Tower* tower : towers)
		{
			float dist = distance(demon, *tower);
			if (dist <= demon.getAttackRange() && dist < closestDist)
			{
				closestDist = dist;
				closestTower = tower;
			}
		}

		if (closestTower)
			demon.shoot(closestTower);
	}

	// Mise à jour des tours
	for (Tower* tower : towers)
	{
		tower->update(deltaTime);

		if (!tower->canAttack()) continue;

		Demon* closestDemon = nullptr;
		float closestDist = tower->getAttackRange();

		for (int i = 0; i < spawnedDemons; ++i)
		{
			Demon& demon = demons[i];
			if (!demon.isDemonAlive()) continue;

			float dist = distance(*tower, demon);
			if (dist <= tower->getAttackRange() && dist < closestDist)
			{
				closestDist = dist;
				closestDemon = &demon;
			}
		}

		if (closestDemon)
			std::cout << "[DEBUG] Tower is attacking a Demon!" << std::endl;

			tower->shoot(closestDemon);
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
	for (int i = 0; i < emplacementCount; ++i) {
		towerEmplacements[i].draw(renderWindow);
		
	}
	for (Tower* tower : towers) {
		tower->draw(renderWindow);
	}

	hud.draw(renderWindow);
	renderWindow.display();
}

bool SceneGame::unload()
{
	return true;
}
void SceneGame::createTower(sf::Vector2f position)
{
	if (currentAction == ActionMode::CreateArcherTower) {
		ArcherTower* newTower = new ArcherTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower); 
		std::cout << "[DEBUG] Archer Tower created at: " << position.x << ", " << position.y << std::endl;
	}
	else if (currentAction == ActionMode::CreateMageTower) {
		MageTower* newTower = new MageTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower); 
		std::cout << "[DEBUG] Mage Tower created at: " << position.x << ", " << position.y << std::endl;
	}
	else {
		std::cout << "[DEBUG] Invalid action mode for tower creation." << std::endl;
	}
}

float SceneGame::distance(const GameObject& a, const GameObject& b) const
{
	Vector2f delta = a.getPosition() - b.getPosition();
	return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}
