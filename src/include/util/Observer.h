/**
 * @file Observer.h
 * @brief The header file for the Observer class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details Observer class as required for assignment 2.
 * Uses in-class slides as reference.
 */

#pragma once

class Observable;

class Observer {
public:
	// destructor
	virtual ~Observer();

	// Observer update method after push from Observable
	virtual void update(Observable* observable) = 0;

protected:
	// constructor
	Observer();
};