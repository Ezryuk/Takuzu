#include "Observable.h"

Observable::Observable()
{
    observers = new std::set<IObserver*>;
}

Observable::~Observable()
{
    delete observers;
}

void Observable::notify(Datas *datas) const
{
    for (auto &observer : *observers) {
        observer->update(datas);
    }
}

void Observable::addObserver(IObserver *observer)
{
    observers->insert(observer);
}

void Observable::removeObserver(IObserver *observer)
{
    observers->erase(observer);
}
