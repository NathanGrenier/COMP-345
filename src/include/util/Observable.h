/**
 * @file Observable.h
 * @brief 
 * @author Denmar Ermitano
 * @date 2025-02-28
 *
 * @details 
 */

#pragma once
#include <util/Observer.h>
#include <vector>

class Observable
{
public:
    virtual ~Observable();
    virtual void attach(Observer* observer);
    virtual void detach(Observer* observer);
    virtual void notify();

protected:
    Observable();

private:
    std::vector<Observer*> *observers;
};

