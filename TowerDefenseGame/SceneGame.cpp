#include "SceneGame.h"
#include "ContentPipeline.h"
#include "Waypoint.h"
#include "KingTower.h"	
#include <iostream> 
#include "TowerEmplacement.h"
#include "ArcherTower.h"
#include "MageTower.h"
#include "Hud.h"
#include "Spell.h"

SceneGame::SceneGame(RenderWindow& renderWindow, Event& event) : Scene(renderWindow, event)
{
	view = renderWindow.getDefaultView();
}

Scene::scenes SceneGame::run()
{
	while (isRunning)
	{
		inputs.reset();
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
	Subject::addObserver(this);
	currentAction = ActionMode::None;
	map.setTexture(ContentPipeline::getInstance().getMapTexture(Maps::map1));

	hud.hudInit(ContentPipeline::getInstance().getHudmaskTexture(), ContentPipeline::getInstance().getComiciFont());


	Waypoint* previous = nullptr;
	for (const Vector2f& pos : WAYPOINTS_MAP1) {
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();

		if (waypointCount > 0)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);

		waypointCount++;
	}
	for (const Vector2f& pos : EMPLACEMENTS_MAP1) {
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

	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < MAX_PER_TYPE; ++i) {
			projectiles[t][i].deactivate();
		}
		nextProjectile[t] = 0;
	}

	nextSpawnTime = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;

	return true;
}

void SceneGame::getInputs()
{
	sf::Event event;
	while (renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			exitGame();

		if (event.type == sf::Event::MouseButtonPressed &&
			event.mouseButton.button == sf::Mouse::Left)
		{
			inputs.mouseLeftButtonClicked = true;
			inputs.mousePosition = renderWindow.mapPixelToCoords(
				{ event.mouseButton.x, event.mouseButton.y }
			);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case Keyboard::Z: inputs.keyZPressed = true; break;
			case Keyboard::X: inputs.keyXPressed = true; break;
			case Keyboard::P: inputs.keyPPressed = true; break;
			default: break;
			}
			if (currentAction == ActionMode::PlagueSpell || currentAction == ActionMode::SacredLight) {
				std::cout << "[DEBUG] Spawn Spell clicked\n";
				Spell* spell = new Spell();
				spell->init();
				spell->activateSpell(inputs.mousePosition, currentAction == ActionMode::PlagueSpell ? SpellType::plague : SpellType::sacredLight);
				spells.push_back(spell);
			}

		}
	}

	if (inputs.keyPPressed)
	{
		paused = !paused;
		hud.setPauseState(paused);
	}
	else if (inputs.keyZPressed)
	{
		currentAction = ActionMode::CreateArcherTower;
	}
	else if (inputs.keyXPressed)
	{
		currentAction = ActionMode::CreateMageTower;
	}
}



void SceneGame::update()
{
	if (paused) return;

	for (int i = 0; i < emplacementCount; ++i)
	{
		Tower* t = emplacementToTower[i];
		if (t && !t->isAlive())
		{
			towerEmplacements[i].activate();
			towerEmplacements[i].init();

			emplacementToTower[i] = nullptr;
		}
	}


	if (inputs.mouseLeftButtonClicked &&
		(currentAction == ActionMode::CreateArcherTower ||
			currentAction == ActionMode::CreateMageTower))
	{
		for (int i = 0; i < emplacementCount; ++i)
		{
			if (emplacementToTower[i] == nullptr &&
				towerEmplacements[i].isMouseOver(inputs.mousePosition))
			{
				createTower(towerEmplacements[i].getPosition(), i);
				towerEmplacements[i].deactivate();
				break;
			}
		}
	}

	spawnTimer += deltaTime;
	manaRegenTimer += deltaTime;

	if (spawnTimer >= nextSpawnTime && spawnedDemons < MAX_DEMONS)
	{
		spawnTimer = 0.f;
		nextSpawnTime = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;

		demons[spawnedDemons].setFirstWaypoint(&waypoints[0]);
		spawnedDemons++;
	}
	if (manaRegenTimer >= 1.f) {
		mana += manaRegenRate * manaRegenTimer;
		manaRegenTimer = 0.f;

		if (mana > maxMana)
			mana = maxMana;
	}

	for (int i = 0; i < spawnedDemons; ++i)
	{
		Demon& demon = demons[i];
		demon.update(deltaTime);

		if (!demon.canAttack())
			continue;

		Tower* bestTarget = nullptr;
		float        bestDist = demon.getAttackRange();

		for (Tower* tower : towers)
		{
			if (!tower->isAlive())
				continue;
			float d = distance(demon, *tower);
			if (d < bestDist)
			{
				bestDist = d;
				bestTarget = tower;
			}
		}
		{
			float dKing = distance(demon, kingTower);
			if (dKing < bestDist)
			{
				bestDist = dKing;
				bestTarget = &kingTower;
			}
		}

		if (bestTarget && demon.shoot())
		{
			std::cout << "[DEBUG] Demon shoot at target\n";
			std::cout << "[DEBUG] Position Tower " << bestTarget->getPosition().x << " " << bestTarget->getPosition().y << "\n";
			spawnProjectile(
				ProjectileType::fireball,
				demon.getPosition(),
				bestTarget->getPosition(),
				demon.getDamage(),
				bestTarget
			);
		}
        hud.updateHud(mana, 0, kills, 0, 0, ActionInString());
	}


	Tower* towerClosestToKing = nullptr;
	float  minDistToKing = std::numeric_limits<float>::max();
	for (Tower* tower : towers) {
		float d = distance(kingTower, *tower);
		if (d < minDistToKing) {
			minDistToKing = d;
			towerClosestToKing = tower;
		}
	}

	kingTower.update(deltaTime);

	// Mise à jour des tours
	for (Tower* tower : towers)
	{
		if (!tower->isAlive()) continue;

		tower->update(deltaTime);

		Demon* bestTarget = nullptr;
		float  towerRange = tower->getAttackRange();
		float  kingRange = kingTower.getAttackRange();
		float  bestDistKing = std::numeric_limits<float>::max();
		float  bestDistTower = towerRange;

		for (int i = 0; i < spawnedDemons; ++i)
		{
			Demon& demon = demons[i];
			if (!demon.isAlive()) continue;

			float dTower = distance(*tower, demon);
			if (dTower > towerRange) continue;

			float dKing = distance(kingTower, demon);

			if (dKing <= kingRange)
			{
				if (dKing < bestDistKing)
				{
					bestDistKing = dKing;
					bestTarget = &demon;
				}
			}
			else if (bestTarget == nullptr)
			{
				if (dTower < bestDistTower)
				{
					bestDistTower = dTower;
					bestTarget = &demon;
				}
			}
		}

		if (bestTarget && tower->shoot())
		{
			ProjectileType type;
			if (dynamic_cast<ArcherTower*>(tower)) type = ProjectileType::arrow;
			else if (dynamic_cast<MageTower*>(tower))   type = ProjectileType::blast;

			spawnProjectile(
				type,
				tower->getPosition(),
				bestTarget->getPosition(),
				tower->getDamage(),
				bestTarget
			);
		}
	}

	for (int t = 0; t < 3; ++t) 
	{
		for (int i = 0; i < MAX_PER_TYPE; ++i) 
		{
			if (projectiles[t][i].isActive()) projectiles[t][i].update(deltaTime);
		}
	}

	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < MAX_PER_TYPE; ++i) {
			Projectile& projectile = projectiles[t][i];
			if (!projectile.isActive()) continue;

			ProjectileType type = static_cast<ProjectileType>(t);
			switch (type) 
			{
			case ProjectileType::arrow:
			case ProjectileType::blast:
				for (int d = 0; d < spawnedDemons; ++d) {
					Demon& demon = demons[d];
					if (!demon.isAlive()) continue;
					if (projectile.isCircleColliding(demon)) {
						projectile.applyDamage(&demon);
						break;
					}
				}
				break;

			case ProjectileType::fireball: {
				bool hit = false;
				for (Tower* tower : towers) {
					if (!tower->isAlive()) continue;
					if (projectile.isCircleColliding(*tower)) {
						std::cout << "[DEBUG] Projectile hit a tower!\n";
						projectile.applyDamage(tower);
						hit = true;
						break;
					}
				}
				if (!hit && projectile.isCircleColliding(kingTower)) {
					projectile.applyDamage(&kingTower);
				}
			} break;
			}
		}
	}
	if (manaRegenTimer >= 1.f) {
    mana += manaRegenRate * manaRegenTimer;
    manaRegenTimer = 0.f;

    if (mana > maxMana)
        mana = maxMana;
}
	for (Spell* spell : spells) {
		spell->update(deltaTime);
	}
	spells.erase(std::remove(spells.begin(), spells.end(), nullptr), spells.end());
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
		if(towerEmplacements[i].isActive()) towerEmplacements[i].draw(renderWindow);

	}
	for (Tower* tower : towers) {
		tower->draw(renderWindow);
	}

	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < MAX_PER_TYPE; ++i) {
			projectiles[t][i].draw(renderWindow);
		}
	}
	for (Spell* spell : spells) {
		spell->draw(renderWindow);
	}

	hud.draw(renderWindow);
	renderWindow.display();
}

bool SceneGame::unload()
{
	return true;
}

// SceneGame.cpp

void SceneGame::createTower(Vector2f position, int emplacementIndex)
{
	if (currentAction == ActionMode::CreateArcherTower) {
		ArcherTower* newTower = new ArcherTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower);

		emplacementToTower[emplacementIndex] = newTower;
		towerEmplacements[emplacementIndex].deactivate();

		std::cout << "[DEBUG] Archer Tower created at: "
			<< position.x << ", " << position.y << std::endl;
	}
	else if (currentAction == ActionMode::CreateMageTower) {
		MageTower* newTower = new MageTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower);

		emplacementToTower[emplacementIndex] = newTower;
		towerEmplacements[emplacementIndex].deactivate();

		std::cout << "[DEBUG] Mage Tower created at: "
			<< position.x << ", " << position.y << std::endl;
	}
	else {
		std::cout << "[DEBUG] Invalid action mode for tower creation." << std::endl;
	}
}


void SceneGame::notify(Subject* subject) {
	if (Demon* demon = dynamic_cast<Demon*>(subject)) {
		mana += manaPerKill;
		kills++;
	}
}

float SceneGame::distance(const GameObject& a, const GameObject& b) const
{
	Vector2f delta = a.getPosition() - b.getPosition();
	return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}

void SceneGame::spawnProjectile(ProjectileType type, const Vector2f& start, const Vector2f& target, int damage, Damageable* targetPtr)
{
	int typeIndex = static_cast<int>(type);
	int next = nextProjectile[typeIndex];

	projectiles[typeIndex][next].init(type, start, target, damage);
	projectiles[typeIndex][next].setTarget(targetPtr);
	nextProjectile[typeIndex] = (next + 1) % MAX_PER_TYPE;
}
String SceneGame::ActionInString() {
	{
		switch (currentAction)
		{
		case ActionMode::None: return "None";
		case ActionMode::CreateArcherTower: return "CreateArcherTower";
		case ActionMode::CreateMageTower: return "CreateMageTower";
		case ActionMode::PlagueSpell: return "PlagueSpell";
		case ActionMode::SacredLight: return "SacredLight";
		case ActionMode::Pause: return "Pause";
		default: return "Unknown";
		}
	}
}