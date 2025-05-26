#include "Game.h"
#include "ContentPipeline.h"
#include "SceneTitle.h"
#include "SceneTransition.h"
#include "SceneGame.h"
#include "SceneEnd.h"
#include "HighScoreManager.h"


Game::Game()
{
	//On place dans le contructeur ce qui permet à la game elle-même de fonctionner
	renderWindow.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Tower Defense Game");  // , Style::Titlebar); / , Style::FullScreen);

	//Synchonisation coordonnée à l'écran!  Normalement 60 frames par secondes. À faire absolument
	//renderWindow.setVerticalSyncEnabled(true);  //De plus en plus d'écrans sont rafraichis à plus de 60 frames par seconde, alors attention.
	//renderWindow.setFramerateLimit(60);  //Équivalent... normalement, mais pas toujours. À utiliser si la synchonisation de l'écran fonctionne mal.
	//https://www.sfml-dev.org/tutorials/2.6/window-window.php

	renderWindow.setKeyRepeatEnabled(false);
	srand((int)time(0));

	//Nouveau: toujours la même chose pour avoir un icon dans l'explorateur Windows
	icon.loadFromFile("Ressources\\Sprites\\Misc\\Icon.png");
	renderWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

int Game::run()
{
	if (!ContentPipeline::getInstance().loadContent()) return EXIT_FAILURE;

	HighScoreManager::load();


	//Un enum et un pointeur de scene pour faire la manipulation de scène
	Scene::scenes sceneSelector = Scene::scenes::TITLE;
	Scene* activeScene = nullptr; //Pointeur de la super-classe, peut pointer sur n'importe quelle scène

	//Les variables de passage d'information entre scènes devraient être déclarés ici

	while (true)
	{
		//Seules conditions de sortie de toute l'app, une pour les sorties normales, une pour les erreurs
		//On est au seul point de sortie
		if (sceneSelector == Scene::scenes::EXIT) return EXIT_SUCCESS;
		if (sceneSelector == Scene::scenes::FAIL) return EXIT_FAILURE;

		if (sceneSelector == Scene::scenes::TITLE)
		{
			SceneTransition::phase = 0;
			savedScore = 0;
		}
		//Vous allez ajouter d'autre scènes, alors elles devront
		//être ajoutées ici
		switch (sceneSelector)
		{
		case Scene::scenes::TITLE:
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new SceneTitle(renderWindow, event);
			break;
		case Scene::scenes::TRANSITION:
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new SceneTransition(renderWindow, event);
			break;
		case Scene::scenes::LEVEL1:
			activeScene = new SceneGame(renderWindow, event, 1, savedScore);
			break;
		case Scene::scenes::LEVEL2:
			activeScene = new SceneGame(renderWindow, event, 2, savedScore);
			break;
		case Scene::scenes::END:
		{
			activeScene = new SceneEnd(renderWindow, event, victory);
			break;
		}
		}

		if (activeScene->init()) //Si l'initilisation s'est bien passé, on entre dans ce bloc
		{
			//Run est la boucle de jeu de la scène
			//À la fin de cette méthode, elle retourne la scène
			//Laquelle on transition
			sceneSelector = activeScene->run();

			if (SceneGame* game = dynamic_cast<SceneGame*>(activeScene))
			{
				savedScore = game->getScore();
			}

			if (sceneSelector == Scene::scenes::END) {
				SceneGame* sceneGame = dynamic_cast<SceneGame*>(activeScene);
				if (sceneGame) {
					HighScoreManager::updateIfBetter(
						sceneGame->getScore(),
						sceneGame->getLevel()
					);
				}
				victory = (sceneGame && sceneGame->getLevel() == 2 && sceneGame->getKills() >= SceneGame::KILL_TRESHOLD);
				delete activeScene;
				activeScene = new SceneEnd(renderWindow, event, victory);
			}
			else {
				delete activeScene;
				activeScene = nullptr;
			}

			//À la fin d'une scène, s'il y a des sauvegardes à faire
			//C'est possible de les faire là.
			/*SceneGame* tempScene = dynamic_cast<SceneGame*>(activeScene);
			if (tempScene != nullptr)//Donc si le cast a réussi.
			{

			}*/
		}
		else //Si l'initialisation rate (exemple: pour assets mal chargés), on fail et on nettoie ce qui est à nettoyer
		{
			sceneSelector = Scene::scenes::FAIL;
			//clean-up éventuel à faire pour s'assurer 
			//de ne pas avoir de leak (malgré l'échec)
		}

		//Nécessaire tout ce qui est crée avec new doit être effacé.
		delete activeScene;
		activeScene = nullptr;
	}
}