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

SceneGame::SceneGame(RenderWindow& renderWindow, Event& event, int level, int initialScore)
	: Scene(renderWindow, event)
	, level(level)
	, score(initialScore)
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
	waypointCount = 0;
	emplacementCount = 0;
	spawnedDemons = 0;

	hud.hudInit(
		ContentPipeline::getInstance().getHudmaskTexture(),
		ContentPipeline::getInstance().getComiciFont()
	);

	for (int i = 0; i < MAX_EMPLACEMENTS; ++i)
		emplacementToTower[i] = nullptr;

	for (int i = 0; i < MAX_EMPLACEMENTS; ++i)
		towerEmplacements[i].activate(), towerEmplacements[i].init();

	if (level == 1)
		loadLevel1();
	else
		loadLevel2();

	kingTower.init();

	for (int t = 0; t < 3; ++t)
		for (int i = 0; i < MAX_PER_TYPE; ++i)
			projectiles[t][i].deactivate();
	std::fill(std::begin(nextProjectile), std::end(nextProjectile), 0);

	nextSpawnTime = 1.f + static_cast<float>(std::rand()) / RAND_MAX * 2.f;

	int musicIndex = std::rand() % MUSIC_COUNT;
	const char* musicPath = GAME_MUSIC_PATHS[musicIndex];

	if (!gameMusic.openFromFile(musicPath))
		return false;
	gameMusic.setLoop(true);
	gameMusic.setVolume(60.f);
	gameMusic.play();


	return true;
}


void SceneGame::getInputs()
{
	inputs.reset();

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
			case sf::Keyboard::P:
				paused = !paused;
				currentAction = paused ? ActionMode::Pause : ActionMode::None;
				break;

			case sf::Keyboard::Z:
				currentAction = ActionMode::CreateArcherTower;
				break;

			case sf::Keyboard::X:
				currentAction = ActionMode::CreateMageTower;
				break;

			case sf::Keyboard::A:
				currentAction = ActionMode::PlagueSpell;
				break;

			case sf::Keyboard::S:
				currentAction = ActionMode::SacredLight;
				break;

			case sf::Keyboard::W:
				showWaypoints = !showWaypoints;
				break;

			default:
				currentAction = ActionMode::None;
				break;
			}
		}
	}
}


void SceneGame::update()
{
	checkAndUpdateHighScore();
	hud.updateHud(mana, score, kills, level, HighScoreManager::getHighScore(), ActionInString());
	if (paused) return;

	respawnDeadTowers();
	processTowerPlacement();
	
	updateSpawnAndMana();
	processDemonsAttacks();

	updateKingTowerLogic();
	checkKingTowerDeath();
	if (!isRunning) return;


	processTowersAttacks();
	updateProjectilesCollisions();

	updateSpellsLogic();
	processSpellCasting();

	checkLevelCompletion();
}

void SceneGame::respawnDeadTowers()
{
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
}

void SceneGame::processTowerPlacement()
{
	if (!(inputs.mouseLeftButtonClicked &&
		(currentAction == ActionMode::CreateArcherTower ||
			currentAction == ActionMode::CreateMageTower)))
		return;

	for (int i = 0; i < emplacementCount; ++i)
	{
		if (emplacementToTower[i] == nullptr &&
			towerEmplacements[i].isMouseOver(inputs.mousePosition))
		{
			createTower(towerEmplacements[i].getPosition(), i);
			break;
		}
	}
}

void SceneGame::updateSpawnAndMana()
{
	spawnTimer += deltaTime;
	manaRegenTimer += deltaTime;

	if (spawnTimer >= nextSpawnTime)
	{
		spawnTimer = 0.f;
		nextSpawnTime = 1.f + static_cast<float>(std::rand()) / RAND_MAX * 2.f;

		int slot = -1;
		if (spawnedDemons < MAX_DEMONS) {
			slot = spawnedDemons++;
		}
		else {
			for (int i = 0; i < MAX_DEMONS; ++i)
			{
				if (!demons[i].isAlive())
				{
					slot = i;
					break;
				}
			}
		}
		if (slot >= 0)
		{
			if (level == 2)
				demonPathChoice[slot] = (std::rand() % 2 == 0);

			demons[slot].reset(1,
				(level == 1 ? DEMON_SPAWN_MAP1 : DEMON_SPAWN_MAP2)
			);
			demons[slot].setFirstWaypoint(&waypoints[0]);
			if (level == 2)
				demons[slot].setBranching(
					&waypoints[splitNodeIndex],
					&waypoints[branchAStartIndex],
					&waypoints[branchBStartIndex],
					demonPathChoice[slot]
				);
		}
	}

	if (manaRegenTimer >= 1.f) {
		mana += manaRegenRate * manaRegenTimer;
		manaRegenTimer = 0.f;
		if (mana > maxMana) mana = maxMana;
	}
}


void SceneGame::processDemonsAttacks()
{
	demonAttackedKing = nullptr;
	for (int i = 0; i < spawnedDemons; ++i)
	{
		Demon& demon = demons[i];
		demon.update(deltaTime);
		if (!demon.canAttack()) continue;

		Tower* bestTarget = nullptr;
		float bestDist = demon.getAttackRange();

		for (Tower* tower : towers)
		{
			if (!tower->isAlive()) continue;
			float d = distance(demon, *tower);
			if (d < bestDist) { bestDist = d; bestTarget = tower; }
		}
		float dKing = distance(demon, kingTower);
		if (dKing < bestDist) { bestTarget = &kingTower; }

		if (bestTarget && demon.shoot())
		{
			spawnProjectile(
				ProjectileType::fireball,
				demon.getPosition(),
				bestTarget->getPosition(),
				demon.getDamage(),
				bestTarget
			);
		}
	}
}

void SceneGame::updateKingTowerLogic()
{
	kingTower.update(deltaTime);
}

void SceneGame::processTowersAttacks()
{
	for (Tower* tower : towers)
		processSingleTowerAttack(tower);
}

void SceneGame::processSingleTowerAttack(Tower* tower)
{
	if (!tower->isAlive()) return;
	tower->update(deltaTime);

	Demon* bestTarget = selectBestTowerTarget(tower);
	if (bestTarget && tower->shoot())
	{
		ProjectileType type =
			dynamic_cast<ArcherTower*>(tower)
			? ProjectileType::arrow
			: ProjectileType::blast;

		spawnProjectile(
			type,
			tower->getPosition(),
			bestTarget->getPosition(),
			tower->getDamage(),
			bestTarget
		);
	}
}

Demon* SceneGame::selectBestTowerTarget(Tower* tower)
{
	float range = tower->getAttackRange();

	if (demonAttackedKing &&
		demonAttackedKing->isAlive() &&
		distance(*tower, *demonAttackedKing) <= range)
	{
		return demonAttackedKing;
	}

	for (int i = 0; i < spawnedDemons; ++i)
	{
		Demon& demon = demons[i];
		if (demon.isAlive() && distance(*tower, demon) <= range)
			return &demon;
	}

	return nullptr;
}


void SceneGame::updateProjectilesCollisions()
{
	for (int t = 0; t < 3; ++t)
	{
		for (int i = 0; i < MAX_PER_TYPE; ++i)
		{
			Projectile& p = projectiles[t][i];
			if (!p.isActive()) continue;

			p.update(deltaTime);
			processSingleProjectile(p, static_cast<ProjectileType>(t));
		}
	}
}

void SceneGame::processSingleProjectile(Projectile& p, ProjectileType type)
{
	switch (type)
	{
	case ProjectileType::arrow:
	case ProjectileType::blast:
		handleArrowOrBlastCollision(p);
		break;

	case ProjectileType::fireball:
		handleFireballCollision(p);
		break;
	}
}

void SceneGame::handleArrowOrBlastCollision(Projectile& p)
{
	for (int d = 0; d < spawnedDemons; ++d)
	{
		Demon& demon = demons[d];
		if (!demon.isAlive()) continue;
		if (p.isCircleColliding(demon))
		{
			p.applyDamage(&demon);
			score += p.getDamage();
			break;
		}
	}
}

void SceneGame::handleFireballCollision(Projectile& p)
{
	bool hit = false;
	for (Tower* tower : towers)
	{
		if (!tower->isAlive()) continue;
		if (p.isCircleColliding(*tower))
		{
			std::cout << "[DEBUG] Projectile hit a tower!\n";
			p.applyDamage(tower);
			hit = true;
			break;
		}
	}
	if (!hit && p.isCircleColliding(kingTower))
	{
		p.applyDamage(&kingTower);
	}
}

void SceneGame::updateSpellsLogic()
{
	for (auto it = spells.begin(); it != spells.end(); )
	{
		Spell* spell = *it;
		spell->update(deltaTime);

		if (!spell->isActive())
		{
			delete spell;
			it = spells.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SceneGame::processSpellCasting()
{
	if (!inputs.mouseLeftButtonClicked)
		return;

	if (currentAction != ActionMode::PlagueSpell &&
		currentAction != ActionMode::SacredLight)
	{
		return;
	}

	SpellType requested = (currentAction == ActionMode::PlagueSpell)
		? SpellType::plague
		: SpellType::sacredLight;

	if (requested == SpellType::plague)
	{
		if (mana < plagueCost) return;
		mana -= plagueCost;
	}
	else
	{
		if (mana < sacredLightCost) return;
		mana -= sacredLightCost;
	}

	for (Spell* spell : spells)
		if (spell->isActive() && spell->getType() == requested)
			return;

	Spell* spell = new Spell();
	spell->init();
	spell->activateSpell(inputs.mousePosition, requested);

	View currentView = renderWindow.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();
	FloatRect viewBounds(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f, viewSize.x, viewSize.y);

	float range = spell->getRange();
	for (int i = 0; i < spawnedDemons; ++i)
	{
		Demon* demon = &demons[i];
		sf::Vector2f pos = demon->getPosition();
		if (demon->isAlive() &&
			distance(*demon, *spell) <= range &&
			viewBounds.contains(pos))
		{
			spell->addObserver(demon);
		}
	}
	for (Tower* tower : towers)
	{
		if (tower->isAlive() &&
			distance(*tower, *spell) <= range)
		{
			spell->addObserver(tower);
		}
	}


	spells.push_back(spell);
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
	gameMusic.stop();
	Subject::removeObserver(this);
	return true;
}


void SceneGame::createTower(Vector2f position, int emplacementIndex)
{
	if (currentAction == ActionMode::CreateArcherTower) {
		if (mana < archerCost) return;
		mana -= archerCost;

		ArcherTower* newTower = new ArcherTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower);

		emplacementToTower[emplacementIndex] = newTower;
		towerEmplacements[emplacementIndex].deactivate();
		Subject::addObserver(newTower);
	}
	else if (currentAction == ActionMode::CreateMageTower) {
		if (mana < mageCost) return;
		mana -= mageCost;

		MageTower* newTower = new MageTower();
		newTower->GameObject::setPosition(position);
		newTower->init();
		towers.push_back(newTower);

		emplacementToTower[emplacementIndex] = newTower;
		towerEmplacements[emplacementIndex].deactivate();
		Subject::addObserver(newTower);

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

void SceneGame::loadLevel1()
{
	map.setTexture(ContentPipeline::getInstance().getMapTexture(Maps::map1));
	kingTower.setPosition(KING_POS_MAP1);

	for (const sf::Vector2f& pos : WAYPOINTS_MAP1)
	{
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();
		if (waypointCount > 0)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);
		++waypointCount;
	}

	for (const sf::Vector2f& pos : EMPLACEMENTS_MAP1)
	{
		towerEmplacements[emplacementCount].setPosition(pos);
		towerEmplacements[emplacementCount].activate();
		towerEmplacements[emplacementCount].init();
		++emplacementCount;
	}

	for (int i = 0; i < MAX_DEMONS; ++i)
		demons[i].reset(1, DEMON_SPAWN_MAP1);
}


void SceneGame::loadLevel2()
{
	map.setTexture(ContentPipeline::getInstance().getMapTexture(Maps::map2));
	kingTower.setPosition(KING_POS_MAP2);

	for (const sf::Vector2f& pos : WAYPOINTS_MAP2_COMMON)
	{
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();
		if (waypointCount > 0)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);
		++waypointCount;
	}
	splitNodeIndex = waypointCount - 1;

	branchAStartIndex = waypointCount;
	for (const sf::Vector2f& pos : WAYPOINTS_MAP2_BRANCH_A)
	{
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();
		if (waypointCount > branchAStartIndex)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);
		++waypointCount;
	}

	branchBStartIndex = waypointCount;
	for (const sf::Vector2f& pos : WAYPOINTS_MAP2_BRANCH_B)
	{
		waypoints[waypointCount].setPosition(pos);
		waypoints[waypointCount].activate();
		waypoints[waypointCount].init();
		if (waypointCount > branchBStartIndex)
			waypoints[waypointCount - 1].setNext(&waypoints[waypointCount]);
		++waypointCount;
	}

	for (const Vector2f& pos : EMPLACEMENTS_MAP2)
	{
		towerEmplacements[emplacementCount].setPosition(pos);
		towerEmplacements[emplacementCount].activate();
		towerEmplacements[emplacementCount].init();
		++emplacementCount;
	}

	for (int i = 0; i < MAX_DEMONS; ++i)
	{
		demons[i].reset(1, DEMON_SPAWN_MAP2);
		demons[i].setFirstWaypoint(&waypoints[0]);

		bool choice = (std::rand() % 2) == 0;

		demons[i].setBranching(
			&waypoints[splitNodeIndex],
			&waypoints[branchAStartIndex],
			&waypoints[branchBStartIndex],
			choice
		);
	}
}

void SceneGame::checkAndUpdateHighScore() {
	int storedHS = HighScoreManager::getHighScore();
	if (score > storedHS) {
		HighScoreManager::updateIfBetter(score, level);
		highScore = score;
	}
	else {
		highScore = storedHS;
	}
}

void SceneGame::checkKingTowerDeath()
{
	if (!kingTower.isAlive()) {
		isRunning = false;
		transitionToScene = scenes::END;
	}
}

void SceneGame::checkLevelCompletion()
{
	if (level == 1 && kills >= KILL_TRESHOLD) {
		isRunning = false;
		transitionToScene = scenes::TRANSITION;
	}
	else if (level == 2 && kills >= KILL_TRESHOLD) {
		HighScoreManager::updateIfBetter(score, level);
		isRunning = false;
		transitionToScene = scenes::END;
	}

}