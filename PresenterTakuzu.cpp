#include "PresenterTakuzu.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    model = new ModelTakuzu();
    view = new MainWindow();
    view->show();
}

PresenterTakuzu::~PresenterTakuzu()
{
    delete model;
}

