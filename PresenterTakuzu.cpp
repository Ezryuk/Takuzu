#include "PresenterTakuzu.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    model = new ModelTakuzu();
    view = new MainWindow();
    view->show();
    connect(view->getGrid(), SIGNAL(coordinatesClicked(int,int)), model, SLOT(playAt(int,int)));

}

PresenterTakuzu::~PresenterTakuzu()
{
    delete model;
}

