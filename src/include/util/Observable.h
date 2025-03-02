/**
 * @file Observable.h
 * @brief The header file for the Observable class.
 * @author Denmar Ermitano
 * @date 2025-03-02
 *
 * @details Observable class as required for assignment 2.
 * Uses in-class slides as reference.
 */

#pragma once
#include <util/Observer.h>
#include <vector>

class Observable
{
public:
    // destructor
    virtual ~Observable();

    // Observer methods
    virtual void attach(Observer* observer);
    virtual void detach(Observer* observer);
    virtual void notify();

protected:
    // constructor
    Observable();

private:
    // vector of observers
    std::vector<Observer*>* observers;
};

