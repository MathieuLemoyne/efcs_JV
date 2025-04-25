#pragma once
#include "IObserver.h"

class Subject;

class IObserver
{
	public:
		virtual void notify(Subject* subject) = 0;
};