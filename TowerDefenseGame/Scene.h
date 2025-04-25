#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

/// <summary>
/// Classe abstraite avec aucune méthode concrétisée, sauf le constructeur et le destructeur
/// (qui restent très simples)  Un .h suffit donc.
/// </summary>
class Scene
{

public:
	//Quand vos ajouterez des scènes, ajouter un enum ici.  Exit et Fail sont des marqueurs de sortie
	//GAME est très générique et est valable si on a une seule scène de jeu.
	//Sinon on pourrait parler de "LEVEL1, LEVEL2, ou encore GAME_SCENE1, GAME_SCENE2, etc.
	//EXIT et FAIL devraient toujours être présents.
	enum scenes { TITLE, LEVEL1, TRANSITION, END, EXIT, FAIL };

	//après le : on fait appel au constructeur de la super classe
	//qui doit être aussi dans la liste d'initialisation
	Scene(RenderWindow& renderWindow, Event& event) : renderWindow(renderWindow), event(event) {}

	virtual ~Scene() {}  //Pour que le destructeur de l'objet en mémoire soit apellé
	virtual scenes run() = 0;
	virtual bool init() = 0;

	void exitGame()
	{
		isRunning = false;
		transitionToScene = Scene::scenes::EXIT;
	}

protected:
	//La boucle de jeu est maintenant gérée par la scène
	void calculateDeltaTime() { deltaTime = clock.restart().asSeconds(); }
	virtual void getInputs() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool unload() = 0;

	//Chaque scène aura une référence de ces objets portée par la game elle-même.
	RenderWindow& renderWindow;
	Event& event;

	Clock clock;
	float deltaTime = 0.0f;

	/// <summary>
	/// À la fin de chaque scène, on indique à Game quelle nouvelle scène elle doit charger.
	/// </summary>
	Scene::scenes transitionToScene;
	bool isRunning = true;
};

