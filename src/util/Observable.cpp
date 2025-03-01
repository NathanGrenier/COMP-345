#include <util/Observable.h>
#include <util/Observer.h>
#include <vector>

Observable::Observable()
{
	observers = new std::vector<Observer*>();
}

void Observable::attach(Observer* observer)
{
	observers->push_back(observer);
}

void Observable::detach(Observer* observer)
{
	for (int i = 0; i < observers->size(); i++)
	{
		if ((*observers)[i] == observer)
		{
			observers->erase(observers->begin() + i);
		}
	}
}

void Observable::notify()
{
	for (int i = 0; i < observers->size(); i++)
	{
		(*observers)[i]->update(this);
	}
}

Observable::~Observable()
{
	for (int i = 0; i < observers->size(); i++)
	{
		delete(&observers[i]);
	}

	observers->clear();
	
	delete(observers);
	observers = NULL;
}
