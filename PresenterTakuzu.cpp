#include "PresenterTakuzu.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    model = new ModelTakuzu();
    view = new MainWindow();
    view->show();
    connect(view->getGrid(), SIGNAL(notifyCoordinatesClicked(int,int)), model, SLOT(registerPlayAt(int,int)));

}

PresenterTakuzu::~PresenterTakuzu()
{
    delete model;
}

