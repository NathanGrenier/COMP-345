/**
 * @class Observable
 * @brief Implementation of the Observable class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 */

#include <util/Observable.h>
#include <util/Observer.h>
#include <vector>

/**
 * @brief Default constructor.
 */
Observable::Observable()
{
	observers = new std::vector<Observer*>();
}

/**
 * @brief Attaches an Observer object to the Observable
 * @param observer the Observer to attach to the Observable
 */
void Observable::attach(Observer* observer)
{
	observers->push_back(observer);
}

/**
 * @brief Detaches an Observer object to the Observable
 * @param observer the Observer to detach to the Observable
 */
void Observable::detach(Observer* observer)
{
	for (int i = 0; i < observers->size(); i++)
	{
		// finds the observer to remove
		if ((*observers)[i] == observer)
		{
			observers->erase(observers->begin() + i);
		}
	}
}

/**
 * @brief Notifies all observers
 * @details Uses the push model of Observer/Observable pair.
 * Notifies all attached Observers to update
 */
void Observable::notify()
{
	for (int i = 0; i < observers->size(); i++)
	{
		(*observers)[i]->update(this);
	}
}

/**
 * @brief Destructor for Observable, deleting attached Observers
 */
Observable::~Observable()
{
	for (int i = 0; i < observers->size(); i++)
	{
		delete(&observers[i]);
	}

	observers->clear();
	observers = NULL;
}
