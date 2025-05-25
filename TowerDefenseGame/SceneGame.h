#pragma once
#include "Scene.h"
#include "Hud.h"
#include "Inputs.h"
#include "Waypoint.h"
#include "KingTower.h"
#include "Demon.h"
#include "TowerEmplacement.h"
#include "IObserver.h"
#include "Projectile.h"
#include "Spell.h"
/*
Metrics de sceneGame OU du level 1 (à effacer à la fin)
- Position de la tour du roi: 1138, 600
- Position des emplacements de tour: (470, 170), (770, 250), (440, 370), (650, 520), (120, 650), (470, 700), (850, 710), (660, 950)
- Position des waypoints: (610, 8), (630, 222), (595, 444), (478, 514), (320, 558), (260, 620), (280, 720), (348, 812), (720, 830), (968, 850), (1110, 682)
- Position de départ des démons: 610, -100
- Nombre de démons : 20 (même si on doit en spawer 50, à 20 à la fois, le joueur en a plein les bras).
- Nombre et de chacun des types de projectiles: à vous de le décider mais il doit être raisonnable ET on ne doit jamais en manquer
- Temps de spawn entre les démons: de 1 à 3 secondes (60 à 180 rafraichissements) au hasard.
- Mana: 500 au départ, + 1 par 0.2 secondes (12 rafraichissements), + 25 mana par élimination de démon.
- Score: 50 par démon abbattu.  les dommages fait par les projectiles sont transférés en points.

- 50 éliminations de démons (donc 50 spawns pour gagner la scène)
*/

/*
Metrics de du level 2 (à effacer à la fin)
- Position de la tour du roi: 1138, 564
- Position des emplacements de tour: (110, 620), (228, 320), (444, 780), (362, 530), (610, 222), (998, 270), (630, 460), (935, 520), (798, 760),
- Position des waypoints: (88.f, 412.f), (168.f, 465.f), (222.f, 588.f), (308.f, 670.f), (424.f, 668.f), (double sortie: 510.f, 590.f);  
                          (478.f, 468.f), (516.f, 380.f), (594.f, 360.f), (806.f, 368.f), (1140.f, 450.f), (660.f, 598.f), (804.f, 650.f), (1140.f, 680.f),
- Position de départ des démons: -100, 410

- Le reste est identique à la scène 1
*/

class SceneGame : public Scene, public IObserver
{
public:
	SceneGame(RenderWindow& renderWindow, Event& event, int level);
	scenes run() override;
	bool init() override;
	enum class ActionMode {
		None,
		CreateArcherTower,
		CreateMageTower,
		PlagueSpell,
		SacredLight,
		Pause
	};
private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	void createTower(Vector2f position, int emplacementIndex);

	float distance(const GameObject& a, const GameObject& b) const;

	void respawnDeadTowers();
	void processTowerPlacement();
	void updateSpawnAndMana();
	void processDemonsAttacks();
	void updateKingTowerLogic();
	void processTowersAttacks();
	void processSingleTowerAttack(Tower* tower);
	Demon* selectBestTowerTarget(Tower* tower);
	void updateProjectilesCollisions();
	void processSingleProjectile(Projectile& projectile, ProjectileType type);
	void handleArrowOrBlastCollision(Projectile& p);
	void handleFireballCollision(Projectile& p);
	void updateSpellsLogic();
	void processSpellCasting();

	void loadLevel1();
	void loadLevel2();

	View view;
	Hud hud;
	Inputs inputs;

	Sprite map;

	static constexpr int MAX_WAYPOINTS = 20;
	static constexpr int MAX_EMPLACEMENTS = 9;

	Waypoint waypoints[MAX_WAYPOINTS];
	TowerEmplacement towerEmplacements[MAX_EMPLACEMENTS];
	Tower* emplacementToTower[MAX_EMPLACEMENTS] = { nullptr };

	int waypointCount = 0;
	int emplacementCount = 0;

	bool showWaypoints = false;

	static constexpr int MAX_DEMONS = 20;
	Demon demons[MAX_DEMONS];
	int spawnedDemons = 0;
	float spawnTimer = 0.f;
	float nextSpawnTime = 0.f;

	KingTower kingTower;

	ActionMode currentAction;
	std::vector<Tower*> towers;

	static constexpr int MAX_PER_TYPE = 50;
	Projectile projectiles[3][MAX_PER_TYPE];
	int nextProjectile[3] = { 0, 0, 0 };

	void spawnProjectile(ProjectileType type, const Vector2f& start, const Vector2f& target, int damage, Damageable* targetPtr);
	String ActionInString();

	int towerCount = 0;


	float mana = 500.f;
	float maxMana = 10000.f;
	float manaRegenRate = 5.f; // mana/sec
	float manaRegenTimer = 0.f;
	float manaPerKill = 25.f;

	const int archerCost = 70;
	const int mageCost = 100;
	const int plagueCost = 20;
	const int sacredLightCost = 60;

	void notify(Subject* subject) override;

	int kills = 0;
	int score = 0;

	std::vector<Spell*> spells;

	bool paused = false;

	int level = 1;

	int splitNodeIndex = -1;
	int branchAStartIndex = -1;
	int branchBStartIndex = -1;
	bool demonPathChoice[MAX_DEMONS];


};