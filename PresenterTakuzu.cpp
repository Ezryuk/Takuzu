#include "PresenterTakuzu.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    model = new ModelTakuzu();
    view = new MainWindow();
    view->show();
    connect(view->getGrid(), SIGNAL(notifyCoordinatesClicked(int,int)), model, SLOT(registerPlayAt(int,int)));
    connect(model, SIGNAL(notifyCount(int,int,int,int,int,int)), view->getGrid(), SLOT(registerCount(int,int,int,int,int,int)));
    connect(model, SIGNAL(notifyPositionIsValid(int,int,bool)), view->getGrid(), SLOT(registerPositionIsValid(int,int,bool)));
}

PresenterTakuzu::~PresenterTakuzu()
{
    delete model;
}

