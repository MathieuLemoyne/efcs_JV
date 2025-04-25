#pragma once
#include <vector>

class IObserver;

//Universel; à récupérer tel quel.
class Subject
{
public:
	static void addObserver(IObserver* observer);
	static void removeObserver(IObserver* observer);
	static void removeAllObservers();   //NOUVEAU - Important à faire à la fin des scènes qui ont des sujets dedans, 
										//pour qu'il ne reste pas des pointeurs vides de l'autre scène d'avant.  
										//Unload pourrait enfin servir.

	virtual void notifyAllObservers();  //Virtuel seulement pour que typeid fonctionne avec IObserver.notify

private:
	static std::vector<IObserver*> observers;
};