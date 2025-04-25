#include "Subject.h"
#include "IObserver.h"

std::vector<IObserver*> Subject::observers;

void Subject::addObserver(IObserver* observer)
{
	//Si l'observateur n'est pas déjà dans la liste...
	if (!(std::find(observers.begin(), observers.end(), observer) != observers.end()))
	{
		observers.push_back(observer);
	}
}

void Subject::removeAllObservers()
{
	observers.clear();
}

void Subject::removeObserver(IObserver* observer)
{
	for (int i = 0; i < observers.size(); i++)
	{
		if (observers[i] == observer)
		{
			observers.erase(observers.begin() + i);
			return;
		}
	}
}

void Subject::notifyAllObservers()
{
	for (int i = 0; i < observers.size(); i++)
	{
		observers[i]->notify(this);
	}
}