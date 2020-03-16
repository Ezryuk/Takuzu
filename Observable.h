#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "IObserver.h"

#include <set>

class Observable
{
public:
    Observable();
    ~Observable();
    void notify(Datas *datas) const;
    void addObserver(IObserver *observer);
    void removeObserver(IObserver *observer);
private:
    std::set<IObserver*> *observers;

};

#endif // OBSERVABLE_H
