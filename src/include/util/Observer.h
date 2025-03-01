/**
 * @file Observer.h
 * @brief
 * @author Denmar Ermitano
 * @date 2025-02-22
 *
 * @details
 */

#pragma once

class Observable;

class Observer {
public:
	virtual ~Observer();
	virtual void update(Observable* observable) = 0;

protected:
	Observer();
};